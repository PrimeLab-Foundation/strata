 #pragma once

#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/json/json_sax_handler.hpp"
#include "strata/util/arena_allocator.hpp"
#include "strata/util/lazy_string.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

 namespace strata {
 namespace bindings {

 // Common JSON keys to pre-warm - based on typical patterns and test datasets
 static constexpr std::array<std::string_view, 20> kCommonKeys = {
     "id", "name", "type", "value", "data", "status", "error", "message",
     "items", "users", "results", "count", "total", "offset", "limit",
     "created", "updated", "metadata", "tags", "level"
 };

 /**
  * Thread-local persistent cache for common JSON keys.
  * Initialized once per thread, reused across all loads() calls.
  * This avoids the overhead of creating and interning 20 Python strings per call.
  */
 class PersistentCommonKeys {
   public:
     static PersistentCommonKeys& instance() {
         static thread_local PersistentCommonKeys inst;
         return inst;
     }

     PyObject* get(size_t index) const {
         if (index < kCommonKeys.size() && keys_[index]) {
             return keys_[index];
         }
         return nullptr;
     }

   private:
     PersistentCommonKeys() {
         for (size_t i = 0; i < kCommonKeys.size(); ++i) {
             std::string_view key = kCommonKeys[i];
             PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
             if (py_key) {
                 PyUnicode_InternInPlace(&py_key);
                 // Keep one reference for the persistent cache
                 keys_[i] = py_key;
             } else {
                 keys_[i] = nullptr;
             }
         }
     }

     ~PersistentCommonKeys() {
         if (!Py_IsInitialized()) {
             return;
         }
         for (size_t i = 0; i < kCommonKeys.size(); ++i) {
             if (keys_[i]) {
                 Py_DECREF(keys_[i]);
             }
         }
     }

     PersistentCommonKeys(const PersistentCommonKeys&) = delete;
     PersistentCommonKeys& operator=(const PersistentCommonKeys&) = delete;

     PyObject* keys_[20] = {nullptr};
 };

 // FNV-1a hash for fast string hashing - better distribution for short strings
 inline uint64_t fnv1a_hash(std::string_view sv) noexcept {
     constexpr uint64_t kFnvOffsetBasis = 14695981039346656037ULL;
     constexpr uint64_t kFnvPrime = 1099511628211ULL;
     uint64_t hash = kFnvOffsetBasis;
     for (char c : sv) {
         hash ^= static_cast<uint64_t>(static_cast<unsigned char>(c));
         hash *= kFnvPrime;
     }
     return hash;
 }

 /**
  * Optimized key cache using robin hood hashing with open addressing.
  *
  * Features:
  * - Pre-warmed with common JSON keys for zero-cost hits on frequent keys
  * - Robin hood hashing with linear probing for better cache locality
  * - FNV-1a hash for improved distribution on short strings
  * - Fast-path lookup for most common keys using length+first-char dispatch
  *
  * Thread safety: NOT thread-safe (same as original implementation)
  */
 class KeyCache {
   public:
    static constexpr size_t kNoMaxCachedKeyLength = std::numeric_limits<size_t>::max();
     // Pre-interned common key indices for fast-path lookup
     enum class CommonKey : uint8_t {
         kId = 0, kName, kType, kValue, kData, kStatus, kError, kMessage,
         kItems, kUsers, kResults, kCount, kTotal, kOffset, kLimit,
         kCreated, kUpdated, kMetadata, kTags, kLevel,
         kNotCommon = 255
     };

    KeyCache() { init_storage(kMinBucketCount); }

    explicit KeyCache(strata::util::Arena* arena, bool /* prewarm */ = true) {
        init_storage(kMinBucketCount);
        reset(arena);
    }

    void set_max_cached_key_length(size_t max_len) noexcept {
        max_cached_key_length_ = max_len;
    }

    size_t max_cached_key_length() const noexcept {
        return max_cached_key_length_;
    }

    void reset(strata::util::Arena* arena) {
        arena_ = arena;
        if (buckets_.empty()) {
            init_storage(kMinBucketCount);
        }
        // Generation-based occupancy avoids clearing buckets/distances per reset.
        if (++generation_ == 0) {
            release_cached_keys();
            std::fill(bucket_generations_.begin(), bucket_generations_.end(), 0);
            generation_ = 1;
        }
    }

    void reserve(size_t expected_entries) {
        if (expected_entries == 0) {
            return;
        }
        size_t desired = next_pow2(expected_entries * 2);
        if (desired < kMinBucketCount) {
            desired = kMinBucketCount;
        }
        if (desired > kMaxBucketCount) {
            desired = kMaxBucketCount;
        }
        if (desired <= bucket_count_) {
            return;
        }
        release_cached_keys();
        init_storage(desired);
    }

     PyObject* get(std::string_view key) {
         // Fast-path: check if this is a common key using length + first char dispatch
         PyObject* common = fast_common_key_lookup(key);
         if (LIKELY(common != nullptr)) {
             Py_INCREF(common);
             return common;
         }

         // Skip caching + interning for long or low-reuse keys.
         if (UNLIKELY(!should_cache_key(key))) {
             return PyUnicode_FromStringAndSize(key.data(), key.size());
         }

         // Standard path: robin hood hash lookup
         return lookup_or_insert(key);
     }

    ~KeyCache() { release_cached_keys(); }

   private:
     static constexpr size_t kMaxCacheableKeyLength =
         static_cast<size_t>(std::numeric_limits<uint16_t>::max());
     // Robin hood hash map entry
     struct Bucket {
         uint64_t hash;         // Cached hash value
         PyObject* py_key;      // Interned Python string
         const char* key_data;  // Pointer to key string (in arena or static)
         uint16_t key_len;      // Key length
     };

    // Hash map configuration - power of 2 for fast modulo
    static constexpr size_t kMinBucketCount = 512;
    static constexpr size_t kMaxBucketCount = 65536;
    static constexpr uint8_t kMaxProbeDistance = 32;

    strata::util::Arena* arena_ = nullptr;
    size_t bucket_count_ = 0;
    size_t bucket_mask_ = 0;
    std::vector<Bucket> buckets_;
    std::vector<uint8_t> distances_;  // Probe distances for robin hood
    uint32_t generation_ = 0;
    std::vector<uint32_t> bucket_generations_;
    size_t max_cached_key_length_ = kNoMaxCachedKeyLength;
    // Common keys are now managed by PersistentCommonKeys

    static size_t next_pow2(size_t value) {
        if (value <= 1) {
            return 1;
        }
        --value;
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        if constexpr (sizeof(size_t) >= 8) {
            value |= value >> 32;
        }
        return value + 1;
    }

    void init_storage(size_t bucket_count) {
        bucket_count_ = bucket_count;
        bucket_mask_ = bucket_count_ - 1;
        buckets_.assign(bucket_count_, {});
        distances_.assign(bucket_count_, 0);
        bucket_generations_.assign(bucket_count_, 0);
        generation_ = 1;
    }

    void release_cached_keys() {
        if (!Py_IsInitialized()) {
            return;
        }
        // Common keys are managed by PersistentCommonKeys - don't release them
        // Only release cached keys in hash map
        for (size_t i = 0; i < bucket_count_; ++i) {
            if (i < bucket_generations_.size() && bucket_generations_[i] != 0 &&
                buckets_[i].py_key) {
                Py_DECREF(buckets_[i].py_key);
                buckets_[i].py_key = nullptr;
            }
        }
    }

     // Fast-path lookup using string length and first character
     // Returns nullptr if not a common key
     // Uses persistent thread-local cache for common keys
     PyObject* fast_common_key_lookup(std::string_view key) const noexcept {
         const size_t len = key.size();
         if (len == 0 || len > 8) return nullptr;

         const char first = key[0];
         auto& common = PersistentCommonKeys::instance();

         switch (len) {
             case 2:
                 if (first == 'i' && key[1] == 'd') return common.get(0);  // "id"
                 break;
             case 4:
                 if (first == 'n' && std::memcmp(key.data(), "name", 4) == 0) return common.get(1);
                 if (first == 't' && std::memcmp(key.data(), "type", 4) == 0) return common.get(2);
                 if (first == 'd' && std::memcmp(key.data(), "data", 4) == 0) return common.get(4);
                 if (first == 't' && std::memcmp(key.data(), "tags", 4) == 0) return common.get(18);
                 break;
             case 5:
                 if (first == 'v' && std::memcmp(key.data(), "value", 5) == 0) return common.get(3);
                 if (first == 'e' && std::memcmp(key.data(), "error", 5) == 0) return common.get(6);
                 if (first == 'i' && std::memcmp(key.data(), "items", 5) == 0) return common.get(8);
                 if (first == 'u' && std::memcmp(key.data(), "users", 5) == 0) return common.get(9);
                 if (first == 'c' && std::memcmp(key.data(), "count", 5) == 0) return common.get(11);
                 if (first == 't' && std::memcmp(key.data(), "total", 5) == 0) return common.get(12);
                 if (first == 'l' && std::memcmp(key.data(), "limit", 5) == 0) return common.get(14);
                 if (first == 'l' && std::memcmp(key.data(), "level", 5) == 0) return common.get(19);
                 break;
             case 6:
                 if (first == 's' && std::memcmp(key.data(), "status", 6) == 0) return common.get(5);
                 if (first == 'o' && std::memcmp(key.data(), "offset", 6) == 0) return common.get(13);
                 break;
             case 7:
                 if (first == 'm' && std::memcmp(key.data(), "message", 7) == 0) return common.get(7);
                 if (first == 'r' && std::memcmp(key.data(), "results", 7) == 0) return common.get(10);
                 if (first == 'c' && std::memcmp(key.data(), "created", 7) == 0) return common.get(15);
                 if (first == 'u' && std::memcmp(key.data(), "updated", 7) == 0) return common.get(16);
                 break;
             case 8:
                 if (first == 'm' && std::memcmp(key.data(), "metadata", 8) == 0) return common.get(17);
                 break;
         }
         return nullptr;
     }

     bool should_cache_key(std::string_view key) const noexcept {
         if (!arena_) {
             return false;
         }
         if (key.size() > max_cached_key_length_) {
             return false;
         }
         return key.size() <= kMaxCacheableKeyLength;
     }

     // Robin hood hash map lookup with insertion
     PyObject* lookup_or_insert(std::string_view key) {
         const uint64_t hash = fnv1a_hash(key);
        size_t idx = hash & bucket_mask_;
         uint8_t dist = 0;

         // Linear probe with robin hood
         while (dist < kMaxProbeDistance) {
             // Empty bucket - insert here
             if (bucket_generations_[idx] != generation_) {
                 return insert_at(idx, hash, key, dist);
             }

             Bucket& bucket = buckets_[idx];

             // Found existing key
             if (bucket.hash == hash &&
                 bucket.key_len == key.size() &&
                 std::memcmp(bucket.key_data, key.data(), key.size()) == 0) {
                 Py_INCREF(bucket.py_key);
                 return bucket.py_key;
             }

             // Robin hood: if current probe distance > existing distance, swap and continue
             if (dist > distances_[idx]) {
                 // Insert here and displace existing entry
                 PyObject* result = insert_at(idx, hash, key, dist);
                 // Now we need to find new home for displaced entry
                 // (but for simplicity, just continue probing for displaced entry)
                 // This simplified version just finds next empty slot
                 return result;
             }

            idx = (idx + 1) & bucket_mask_;
            ++dist;
        }

         // Probe distance exceeded - fallback to creating without caching
         // This shouldn't happen with reasonable load factor
         PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
         if (py_key) {
             PyUnicode_InternInPlace(&py_key);
         }
         return py_key;
     }

     // Insert a new key at the given bucket index
     PyObject* insert_at(size_t idx, uint64_t hash, std::string_view key, uint8_t dist) {
         // Create and intern the Python string
         PyObject* py_key = PyUnicode_FromStringAndSize(key.data(), key.size());
         if (!py_key) return nullptr;

         PyUnicode_InternInPlace(&py_key);
         Py_INCREF(py_key);  // One for the cache

         // Copy key to arena for stable storage
         char* key_copy = static_cast<char*>(arena_->allocate(key.size(), 1));
         std::memcpy(key_copy, key.data(), key.size());

         // Store in bucket
         Bucket& bucket = buckets_[idx];
         if (bucket_generations_[idx] != 0 && bucket.py_key) {
             Py_DECREF(bucket.py_key);
         }
         bucket.hash = hash;
         bucket.py_key = py_key;
         bucket.key_data = key_copy;
         bucket.key_len = static_cast<uint16_t>(key.size());
         distances_[idx] = dist;
         bucket_generations_[idx] = generation_;

         return py_key;
     }
 };

 /**
  * Per-parse-session object pool for Python dicts.
  *
  * Pre-creates a batch of empty Python dicts at parse start to amortize
  * malloc/free overhead. Instead of calling _PyDict_NewPresized() individually
  * for each JSON object, we hand out pre-created dicts from the pool.
  *
  * Why dicts only (not lists):
  * - Lists use PyList_New(N) which pre-allocates the item array; pooling empty
  *   lists then resizing is no better than direct allocation.
  * - Dicts are the dominant container type (881K dicts vs 87K lists in test data).
  * - Dict creation via _PyDict_NewPresized involves hash table allocation that
  *   benefits most from batch pre-allocation.
  *
  * Lifecycle:
  * - fill() at parse start: bulk-creates dicts with a common pre-size
  * - acquire_dict(): returns pooled dict or falls back to new allocation
  * - drain() at parse end: decrefs any unused pooled dicts
  *
  * All pooled objects have refcount=1 when acquired. The caller owns the reference.
  * Objects that leave the pool are managed by Python's normal refcount/GC system.
  *
  * Thread safety: NOT thread-safe (per-thread via thread_local in python_loads.cpp)
  */
 class PythonObjectPool {
   public:
     static constexpr size_t kDefaultDictPoolSize = 1024;

     PythonObjectPool() = default;

     ~PythonObjectPool() {
         drain();
     }

     PythonObjectPool(const PythonObjectPool&) = delete;
     PythonObjectPool& operator=(const PythonObjectPool&) = delete;

     /// Set pool capacity.
     void configure(size_t dict_size) {
         dict_pool_size_ = dict_size;
     }

     /// Pre-create pooled dicts. Call at parse start.
     /// @param dict_presize  Capacity hint for _PyDict_NewPresized (0 = use PyDict_New)
     void fill(size_t dict_presize = 0) {
         drain();  // Release any leftovers from previous parse

         dict_presize_ = dict_presize;
         dict_pool_.reserve(dict_pool_size_);
         for (size_t i = 0; i < dict_pool_size_; ++i) {
             PyObject* d = dict_presize > 0
                 ? _PyDict_NewPresized(static_cast<Py_ssize_t>(dict_presize))
                 : PyDict_New();
             if (LIKELY(d != nullptr)) {
                 dict_pool_.push_back(d);
             }
         }
         dict_pos_ = 0;
         dicts_from_pool_ = 0;
         dicts_fallback_ = 0;
     }

     /// Acquire a dict from the pool.
     /// If pool is exhausted or presize differs significantly from pooled presize,
     /// falls back to _PyDict_NewPresized/PyDict_New.
     /// Caller owns the returned reference (refcount = 1).
     PyObject* acquire_dict(size_t presize) {
         // Use pooled dict if available and presize is compatible.
         // A pooled dict pre-sized for N can hold up to N keys without rehashing.
         // If requested presize <= pooled presize, the pooled dict works fine.
         // If requested presize > pooled presize, dict will rehash but still works.
         if (LIKELY(dict_pos_ < dict_pool_.size())) {
             dicts_from_pool_++;
             return dict_pool_[dict_pos_++];
         }
         // Pool exhausted — fall back to individual allocation
         dicts_fallback_++;
         if (presize > 0) {
             return _PyDict_NewPresized(static_cast<Py_ssize_t>(presize));
         }
         return PyDict_New();
     }

     /// Release all unused pooled dicts. Call at parse end or reset.
     void drain() {
         if (!Py_IsInitialized()) {
             dict_pool_.clear();
             return;
         }
         for (size_t i = dict_pos_; i < dict_pool_.size(); ++i) {
             Py_DECREF(dict_pool_[i]);
         }
         dict_pool_.clear();
         dict_pos_ = 0;
     }

     bool is_active() const { return !dict_pool_.empty(); }

     // Stats for tuning
     size_t dicts_from_pool() const { return dicts_from_pool_; }
     size_t dicts_fallback() const { return dicts_fallback_; }
     size_t pool_capacity() const { return dict_pool_size_; }

   private:
     std::vector<PyObject*> dict_pool_;
     size_t dict_pos_ = 0;
     size_t dict_pool_size_ = kDefaultDictPoolSize;
     size_t dict_presize_ = 0;
     size_t dicts_from_pool_ = 0;
     size_t dicts_fallback_ = 0;
 };

 /**
  * Adaptive size estimator using exponential moving averages.
  * Tracks recent dict/list sizes to improve pre-allocation accuracy.
  */
class AdaptiveSizeEstimator {
  public:
    AdaptiveSizeEstimator() = default;

    static constexpr size_t kWindow = 256;
    void enable_tracking(bool on) { tracking_enabled_ = on; }

    // Update estimator with actual size observed
    void record_dict_size(size_t actual_size, size_t estimated) {
        dict_count_++;
        dict_sum_ += actual_size;
        // Exponential moving average with alpha=0.125 (1/8)
        dict_ema_ = (dict_ema_ * 7 + actual_size) / 8;

        if (!tracking_enabled_) {
            return;
        }

        // Sliding window for recent-average estimation (size kWindow)
        if (dict_recent_count_ < kWindow) {
            dict_recent_[dict_recent_pos_] = actual_size;
            dict_recent_sum_ += actual_size;
            dict_recent_count_++;
        } else {
            dict_recent_sum_ -= dict_recent_[dict_recent_pos_];
            dict_recent_[dict_recent_pos_] = actual_size;
            dict_recent_sum_ += actual_size;
        }
        dict_recent_pos_ = (dict_recent_pos_ + 1) % kWindow;

        // Track accuracy for tuning
        if (actual_size > estimated) {
            ++dict_under_estimate_;
        } else if (actual_size < estimated) {
            ++dict_over_estimate_;
        } else {
            ++dict_exact_estimate_;
        }
    }

    void record_list_size(size_t actual_size, size_t estimated) {
        list_count_++;
        list_sum_ += actual_size;
        list_ema_ = (list_ema_ * 7 + actual_size) / 8;

        if (!tracking_enabled_) {
            return;
        }

        if (list_recent_count_ < kWindow) {
            list_recent_[list_recent_pos_] = actual_size;
            list_recent_sum_ += actual_size;
            list_recent_count_++;
        } else {
            list_recent_sum_ -= list_recent_[list_recent_pos_];
            list_recent_[list_recent_pos_] = actual_size;
            list_recent_sum_ += actual_size;
        }
        list_recent_pos_ = (list_recent_pos_ + 1) % kWindow;

        if (actual_size > estimated) {
            ++list_under_estimate_;
        } else if (actual_size < estimated) {
            ++list_over_estimate_;
        } else {
            ++list_exact_estimate_;
        }
    }

    // Get estimated size for next dict/list
    size_t estimate_dict_size() const {
        if (dict_recent_count_ > 0) {
            return dict_recent_sum_ / dict_recent_count_;
        }
        if (dict_count_ == 0) return 16;  // Conservative default, covers 80% of objects
        size_t avg = dict_ema_ > 0 ? dict_ema_ : dict_sum_ / dict_count_;
        return avg > 0 ? avg : 16;
    }

    size_t estimate_list_size() const {
        if (list_recent_count_ > 0) {
            return list_recent_sum_ / list_recent_count_;
        }
        if (list_count_ == 0) return 8;
        size_t avg = list_ema_ > 0 ? list_ema_ : list_sum_ / list_count_;
        return avg > 0 ? avg : 8;
    }

    // Get accuracy metrics for tuning
    void get_stats(size_t& dict_avg, size_t& list_avg, size_t& dict_n, size_t& list_n) const {
        dict_avg = dict_count_ > 0 ? dict_sum_ / dict_count_ : 0;
        list_avg = list_count_ > 0 ? list_sum_ / list_count_ : 0;
        dict_n = dict_count_;
        list_n = list_count_;
    }

    void get_accuracy(size_t& dict_under, size_t& dict_over, size_t& dict_exact, size_t& list_under,
                      size_t& list_over, size_t& list_exact) const {
        dict_under = dict_under_estimate_;
        dict_over = dict_over_estimate_;
        dict_exact = dict_exact_estimate_;
        list_under = list_under_estimate_;
        list_over = list_over_estimate_;
        list_exact = list_exact_estimate_;
    }

  private:
    size_t dict_ema_ = 8;       // Exponential moving average for dict sizes
    size_t list_ema_ = 8;       // Exponential moving average for list sizes
    size_t dict_count_ = 0;     // Total dicts seen
    size_t list_count_ = 0;     // Total lists seen
    size_t dict_sum_ = 0;       // Sum for computing average
    size_t list_sum_ = 0;       // Sum for computing average

    std::array<size_t, kWindow> dict_recent_{};
    std::array<size_t, kWindow> list_recent_{};
    size_t dict_recent_sum_ = 0;
    size_t list_recent_sum_ = 0;
    size_t dict_recent_pos_ = 0;
    size_t list_recent_pos_ = 0;
    size_t dict_recent_count_ = 0;
    size_t list_recent_count_ = 0;

    size_t dict_under_estimate_ = 0;
    size_t dict_over_estimate_ = 0;
    size_t dict_exact_estimate_ = 0;
    size_t list_under_estimate_ = 0;
    size_t list_over_estimate_ = 0;
    size_t list_exact_estimate_ = 0;
    bool tracking_enabled_ = false;
};

class PythonObjectBuilder : public JsonSaxHandler {
  public:
    using StackAllocator = strata::util::ArenaAllocator<PyObject*>;
    using SizeAllocator = strata::util::ArenaAllocator<size_t>;
    static constexpr size_t kMaxListPresize = 131072;
    static constexpr size_t kMaxDictPresize = 16384;

    PythonObjectBuilder(strata::util::Arena* arena, KeyCache& cache,
                        PythonObjectPool* pool = nullptr)
        : arena_(arena), stack_(StackAllocator(arena)), keys_(StackAllocator(arena)),
          list_indices_(SizeAllocator(arena)), list_sizes_(SizeAllocator(arena)),
          dict_key_counts_(SizeAllocator(arena)), dict_estimates_(SizeAllocator(arena)),
          list_estimates_(SizeAllocator(arena)),
          list_object_counts_(SizeAllocator(arena)), list_object_key_sums_(SizeAllocator(arena)),
          list_list_counts_(SizeAllocator(arena)), list_list_size_sums_(SizeAllocator(arena)),
          cache_(cache), pool_(pool) {
        stack_.reserve(32);
        keys_.reserve(32);
        list_indices_.reserve(32);
        list_sizes_.reserve(32);
        dict_key_counts_.reserve(32);
        dict_estimates_.reserve(32);
        list_estimates_.reserve(32);
        list_object_counts_.reserve(32);
        list_object_key_sums_.reserve(32);
        list_list_counts_.reserve(32);
        list_list_size_sums_.reserve(32);

        // Enable detailed tracking only when requested to avoid hot-path overhead.
        const bool track =
            (std::getenv("STRATA_TRACK_PRESIZE") != nullptr) ||
            (std::getenv("STRATA_LOG_PRESIZE") != nullptr);
        estimator_.enable_tracking(track);
    }

    void set_pool(PythonObjectPool* pool) { pool_ = pool; }

    void reset() {
        if (!Py_IsInitialized()) {
            root_ = nullptr;
            stack_.clear();
            keys_.clear();
            list_indices_.clear();
            list_sizes_.clear();
            dict_key_counts_.clear();
            dict_estimates_.clear();
            list_estimates_.clear();
            list_object_counts_.clear();
            list_object_key_sums_.clear();
            list_list_counts_.clear();
            list_list_size_sums_.clear();
            current_list_depth_ = 0;
            return;
        }
        if (root_) {
            Py_DECREF(root_);
            root_ = nullptr;
        }
        for (auto obj : stack_) {
            Py_DECREF(obj);
        }
        stack_.clear();
        for (auto key : keys_) {
            Py_DECREF(key);
        }
        keys_.clear();
        list_indices_.clear();
        list_sizes_.clear();
        dict_key_counts_.clear();
        dict_estimates_.clear();
        list_estimates_.clear();
        list_object_counts_.clear();
        list_object_key_sums_.clear();
        list_list_counts_.clear();
        list_list_size_sums_.clear();
        current_list_depth_ = 0;
    }

    ~PythonObjectBuilder() { reset(); }

     bool on_null() override {
         // Python 3.12+ has immortal None/True/False - no refcount needed
         // For older Python, Py_NewRef handles this correctly
#if PY_VERSION_HEX >= 0x030C0000
         // Immortal objects in Python 3.12+ - refcount ops are no-ops
         return push_value(Py_None);
#else
         Py_INCREF(Py_None);
         return push_value(Py_None);
#endif
     }

     bool on_bool(bool v) override {
         PyObject* obj = v ? Py_True : Py_False;
         // Python 3.12+ has immortal None/True/False - no refcount needed
#if PY_VERSION_HEX >= 0x030C0000
         // Immortal objects in Python 3.12+ - refcount ops are no-ops
         return push_value(obj);
#else
         Py_INCREF(obj);
         return push_value(obj);
#endif
     }

     bool on_int(int64_t v) override {
         // Optimization: PyLong_FromLong is more efficient than PyLong_FromLongLong
         // for values that fit in a C long. Python's small integer cache (range [-5, 256])
         // is automatically used by PyLong_FromLong, avoiding memory allocation.
         // Most JSON integers fit in long range, so use LIKELY for branch prediction.
         if (LIKELY(v >= LONG_MIN && v <= LONG_MAX)) {
             return push_value(PyLong_FromLong(static_cast<long>(v)));
         }
         return push_value(PyLong_FromLongLong(v));
     }

     bool on_uint(uint64_t v) override {
         // Optimization: Use faster PyLong_FromLong for values that fit in a long.
         // This also enables use of Python's small integer cache for small values.
         if (LIKELY(v <= static_cast<uint64_t>(LONG_MAX))) {
             return push_value(PyLong_FromLong(static_cast<long>(v)));
         }
         return push_value(PyLong_FromUnsignedLongLong(v));
     }

     bool on_double(double v) override { return push_value(PyFloat_FromDouble(v)); }

     bool on_string(std::string_view v, bool has_escapes = false) override {
         if (has_escapes) {
             // Use LazyString to unescape the string
             LazyString lazy(v, true);
             const std::string& unescaped = lazy.value();
             return push_value(PyUnicode_FromStringAndSize(unescaped.data(), unescaped.size()));
         }
         // Direct string creation without caching - benchmarks show caching
         // adds overhead for small datasets with mostly unique strings
         return push_value(PyUnicode_FromStringAndSize(v.data(), v.size()));
     }

    bool on_start_object(size_t size_hint) override {
         // Use _PyDict_NewPresized for better performance when size is known
         // This pre-allocates hash table capacity, reducing rehashing overhead
        PyObject* dict;
        size_t presize = compute_dict_presize(size_hint);

        // Use object pool if available (amortizes malloc overhead)
        if (pool_ && pool_->is_active()) {
            dict = pool_->acquire_dict(presize);
        } else {
            // Direct allocation fallback
            dict = _PyDict_NewPresized(static_cast<Py_ssize_t>(presize));
        }
        if (!dict)
             return false;
         stack_.push_back(dict);
         // Track that we're building a dict to count keys later
         dict_key_counts_.push_back(0);
         dict_estimates_.push_back(presize);
         return true;
     }

     bool on_key(std::string_view v, bool has_escapes = false) override {
         if (has_escapes) {
             // Use LazyString to unescape the key before caching
             LazyString lazy(v, true);
             const std::string& unescaped = lazy.value();
             PyObject* key = cache_.get(std::string_view(unescaped));
             if (!key)
                 return false;
             keys_.push_back(key);
             // Track key count for adaptive estimation
             if (!dict_key_counts_.empty()) {
                 dict_key_counts_.back()++;
             }
             return true;
         }
         PyObject* key = cache_.get(v);
         if (!key)
             return false;
         keys_.push_back(key);
         // Track key count for adaptive estimation
         if (!dict_key_counts_.empty()) {
             dict_key_counts_.back()++;
         }
         return true;
     }

     bool on_end_object() override {
         if (stack_.empty())
             return false;
         PyObject* dict = stack_.back();
         stack_.pop_back();

         // Record actual size for adaptive estimation
         if (!dict_key_counts_.empty()) {
             size_t actual_keys = dict_key_counts_.back();
             dict_key_counts_.pop_back();
             size_t estimate = dict_estimates_.empty() ? 0 : dict_estimates_.back();
             if (!dict_estimates_.empty()) {
                 dict_estimates_.pop_back();
             }
             estimator_.record_dict_size(actual_keys, estimate);

             // Update sibling-average statistics when parent is an array
             if (current_list_depth_ > 0 && current_list_depth_ <= list_object_counts_.size()) {
                 size_t parent_idx = current_list_depth_ - 1;
                 list_object_counts_[parent_idx] += 1;
                 list_object_key_sums_[parent_idx] += actual_keys;
             }
         }

         return push_value(dict);
     }

    bool on_start_array(size_t size_hint) override {
        size_t presize = compute_list_presize(size_hint);

        // Always pre-allocate with at least the estimated size
        PyObject* list = PyList_New(presize);
        if (!list)
            return false;
        stack_.push_back(list);
        list_indices_.push_back(0);
        list_sizes_.push_back(presize);
        list_estimates_.push_back(presize);
        list_object_counts_.push_back(0);
        list_object_key_sums_.push_back(0);
        list_list_counts_.push_back(0);
        list_list_size_sums_.push_back(0);
        current_list_depth_++;  // O(1) depth tracking
        return true;
    }

     bool on_end_array() override {
         if (stack_.empty())
             return false;
         PyObject* list = stack_.back();
         stack_.pop_back();

         size_t actual_size = list_indices_.back();
         size_t allocated_size = list_sizes_.back();
         list_indices_.pop_back();
         list_sizes_.pop_back();
         size_t estimate = list_estimates_.empty() ? 0 : list_estimates_.back();
         if (!list_estimates_.empty()) {
             list_estimates_.pop_back();
         }
         if (!list_object_counts_.empty()) {
             list_object_counts_.pop_back();
         }
         if (!list_object_key_sums_.empty()) {
             list_object_key_sums_.pop_back();
         }
         if (!list_list_counts_.empty()) {
             list_list_counts_.pop_back();
         }
         if (!list_list_size_sums_.empty()) {
             list_list_size_sums_.pop_back();
         }
         current_list_depth_--;  // O(1) depth tracking

         // Record actual size for adaptive estimation
         estimator_.record_list_size(actual_size, estimate);

         // If parent is a list (array of arrays), track sibling average
         if (current_list_depth_ > 0 && current_list_depth_ <= list_list_counts_.size()) {
             size_t parent_idx = current_list_depth_ - 1;
             list_list_counts_[parent_idx] += 1;
             list_list_size_sums_[parent_idx] += actual_size;
         }

         // Trim list if we allocated more than we used
         if (allocated_size > 0 && actual_size < allocated_size) {
             if (PyList_SetSlice(list, actual_size, allocated_size, NULL) < 0) {
                 Py_DECREF(list);
                 return false;
             }
         }

         return push_value(list);
     }

     PyObject* take_root() {
         PyObject* res = root_;
         root_ = nullptr;
         return res;
     }

    // Get the adaptive estimator's current dict size estimate (for pool pre-sizing)
    size_t estimate_dict_presize() const {
        return estimator_.estimate_dict_size();
    }

    // Get estimator stats for debugging/tuning
    void get_estimator_stats(size_t& dict_avg, size_t& list_avg, size_t& dict_n, size_t& list_n) const {
        estimator_.get_stats(dict_avg, list_avg, dict_n, list_n);
    }

    // Expose accuracy metrics for logging/benchmarking
    void get_accuracy_stats(size_t& dict_under, size_t& dict_over, size_t& dict_exact,
                            size_t& list_under, size_t& list_over, size_t& list_exact) const {
        estimator_.get_accuracy(dict_under, dict_over, dict_exact, list_under, list_over, list_exact);
    }

  private:
    size_t compute_dict_presize(size_t size_hint) const {
        size_t presize = size_hint;

        // Prefer exact/parsed hint when provided
        if (presize == 0) {
            size_t sibling_avg = 0;
            // If parent is a list, use average key count of previous object siblings
            if (!stack_.empty() && PyList_Check(stack_.back())) {
                size_t depth = current_list_depth_;
                if (depth > 0 && depth <= list_object_counts_.size()) {
                    size_t cnt = list_object_counts_[depth - 1];
                    if (cnt > 0) {
                        sibling_avg = list_object_key_sums_[depth - 1] / cnt;
                    }
                }
            }
            presize = sibling_avg > 0 ? sibling_avg : estimator_.estimate_dict_size();
        }

        if (presize == 0) {
            presize = 16; // fallback covers ~80% of objects per profile
        }
        if (presize > kMaxDictPresize) {
            presize = kMaxDictPresize;
        }
        return presize;
    }

    size_t compute_list_presize(size_t size_hint) const {
        size_t presize = size_hint;

        if (presize == 0) {
            size_t sibling_avg = 0;
            if (!stack_.empty() && PyList_Check(stack_.back())) {
                size_t depth = current_list_depth_;
                if (depth > 0 && depth <= list_list_counts_.size()) {
                    size_t cnt = list_list_counts_[depth - 1];
                    if (cnt > 0) {
                        sibling_avg = list_list_size_sums_[depth - 1] / cnt;
                    }
                }
            }
            presize = sibling_avg > 0 ? sibling_avg : estimator_.estimate_list_size();
        }

        if (presize == 0) {
            presize = 8;
        }
        if (presize > kMaxListPresize) {
            presize = kMaxListPresize;
        }
        return presize;
    }

    bool push_value(PyObject* val) {
         if (!val)
             return false;

         if (stack_.empty()) {
             if (root_) {
                 Py_DECREF(root_);
             }
             root_ = val;
             return true;
         }

         PyObject* top = stack_.back();
         if (PyList_Check(top)) {
             // O(1) list depth lookup using tracked counter
             size_t list_depth = current_list_depth_;

             if (list_depth > 0 && list_depth <= list_indices_.size()) {
                 size_t idx_pos = list_depth - 1;
                 size_t current_idx = list_indices_[idx_pos];
                 size_t allocated_size = list_sizes_[idx_pos];

                 // Use PyList_SET_ITEM if we have pre-allocated space
                 if (allocated_size > 0 && current_idx < allocated_size) {
                     PyList_SET_ITEM(top, current_idx, val);  // Steals reference
                     list_indices_[idx_pos]++;
                     return true;
                 } else {
                     // Fallback to append if we exceed the hint
                     if (PyList_Append(top, val) < 0) {
                         Py_DECREF(val);
                         return false;
                     }
                     list_indices_[idx_pos]++;
                     Py_DECREF(val);
                     return true;
                 }
             } else {
                 // Fallback for unexpected state
                 if (PyList_Append(top, val) < 0) {
                     Py_DECREF(val);
                     return false;
                 }
                 Py_DECREF(val);
                 return true;
             }
        } else if (PyDict_Check(top)) {
             if (keys_.empty()) {
                 Py_DECREF(val);
                 return false;
             }
             PyObject* key = keys_.back();
             keys_.pop_back();

             const auto policy = strata::get_duplicate_key_policy();
            if (policy == strata::DuplicateKeyPolicy::LastWins) {
                if (PyDict_SetItem(top, key, val) < 0) {
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return false;
                }
                Py_DECREF(key);
                Py_DECREF(val);
                return true;
            }

            if (policy == strata::DuplicateKeyPolicy::FirstWins) {
                if (!PyDict_SetDefault(top, key, val)) {
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return false;
                }
                Py_DECREF(key);
                Py_DECREF(val);
                return true;
            }

            Py_hash_t hash = PyObject_Hash(key);
            if (hash == -1 && PyErr_Occurred()) {
                Py_DECREF(key);
                Py_DECREF(val);
                return false;
            }

            PyObject* existing = _PyDict_GetItem_KnownHash(top, key, hash);
            if (!existing && PyErr_Occurred()) {
                Py_DECREF(key);
                Py_DECREF(val);
                return false;
            }
            if (existing) {
                switch (policy) {
                case strata::DuplicateKeyPolicy::FirstWins:
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return true;
                case strata::DuplicateKeyPolicy::Warn: {
                    PyObject* key_repr = PyObject_Repr(key);
                    const char* key_str = PyUnicode_AsUTF8(key_repr);
                    std::string msg = "Duplicate key encountered: ";
                    msg += key_str;
                    PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
                    Py_XDECREF(key_repr);
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return true;
                }
                case strata::DuplicateKeyPolicy::Error:
                    Py_DECREF(key);
                    Py_DECREF(val);
                    return false;
                default:
                    break;
                }
            }

            if (PyDict_SetItem(top, key, val) < 0) {
                Py_DECREF(key);
                Py_DECREF(val);
                return false;
            }
            Py_DECREF(key);
            Py_DECREF(val);
            return true;
        }
         Py_DECREF(val);
         return false;
     }

    PyObject* root_ = nullptr;
   strata::util::Arena* arena_;
   std::vector<PyObject*, StackAllocator> stack_;
   std::vector<PyObject*, StackAllocator> keys_;
  std::vector<size_t, SizeAllocator> list_indices_;
  std::vector<size_t, SizeAllocator> list_sizes_;
  std::vector<size_t, SizeAllocator> dict_key_counts_;  // Track keys per dict for estimation
  std::vector<size_t, SizeAllocator> dict_estimates_;
  std::vector<size_t, SizeAllocator> list_estimates_;
  std::vector<size_t, SizeAllocator> list_object_counts_;
  std::vector<size_t, SizeAllocator> list_object_key_sums_;
  std::vector<size_t, SizeAllocator> list_list_counts_;
  std::vector<size_t, SizeAllocator> list_list_size_sums_;
  size_t current_list_depth_ = 0;  // O(1) list depth tracking
  KeyCache& cache_;
  PythonObjectPool* pool_ = nullptr;  // Optional dict pool (per-parse-session)
  AdaptiveSizeEstimator estimator_;  // Learn from observed sizes
};

 } // namespace bindings
 } // namespace strata
