 #pragma once

#include "python_types.h"
#include <listobject.h>
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
#include <bit>
#include <list>
#include <unordered_map>

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



 // SWAR (SIMD Within A Register) ASCII check: tests 8 bytes at a time.
 // Returns true if all bytes in [data, data+len) are ASCII (bit 7 == 0).
 inline bool is_ascii_only_swar(const char* data, size_t len) noexcept {
     if (!data || len == 0) return true;
     size_t i = 0;
     uint64_t mask = 0;
     for (; i + sizeof(uint64_t) <= len; i += sizeof(uint64_t)) {
         uint64_t chunk = 0;
         std::memcpy(&chunk, data + i, sizeof(uint64_t));
         mask |= chunk;
     }
     if (mask & 0x8080808080808080ULL) return false;
     for (; i < len; ++i) {
         if (static_cast<unsigned char>(data[i]) & 0x80) return false;
     }
     return true;
 }

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
        // Always release cached keys and clear occupancy on reset to avoid
        // leaking references and using stale arena pointers.
        release_cached_keys();
        std::fill(bucket_generations_.begin(), bucket_generations_.end(), 0);
        generation_ = 1;
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

        // If the current capacity is much larger than desired, or if we need to grow
        if (desired > bucket_count_ || bucket_count_ > desired * 2) {
            release_cached_keys();
            init_storage(desired);
        }
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

     /**
      * Get a cached key together with its Python hash (for _PyDict_SetItem_KnownHash).
      * The hash is cached on the interned PyUnicode object; reading it is essentially free.
      * Returns the key (new reference) and writes the Python hash to *out_hash.
      * On failure returns nullptr.
      */
     PyObject* get_with_hash(std::string_view key, Py_hash_t* out_hash) {
         PyObject* py_key = get(key);
         if (LIKELY(py_key != nullptr)) {
             // PyObject_Hash on an interned/cached unicode string is a single
             // field read from PyASCIIObject->hash (computed and cached on first call).
             *out_hash = PyObject_Hash(py_key);
             if (UNLIKELY(*out_hash == -1 && PyErr_Occurred())) {
                 Py_DECREF(py_key);
                 return nullptr;
             }
         }
         return py_key;
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

        // Use swap to truly release memory if shrinking significantly
        if (bucket_count < buckets_.size() / 2) {
            std::vector<Bucket>().swap(buckets_);
            std::vector<uint8_t>().swap(distances_);
            std::vector<uint32_t>().swap(bucket_generations_);
        }

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
        for (auto& b : buckets_) {
            if (b.py_key) {
                Py_DECREF(b.py_key);
                b.py_key = nullptr;
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
         if (bucket.py_key) {
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
 * Per-parse-session object pool for Python dicts/lists and small strings.
 *
 * Pools are filled at parse start, handed out during parsing, and drained
 * (unused entries decref'd / arenas reset) after parsing finishes.
 *
 * - Dict pool: pre-created empty dicts sized to an adaptive presize.
 * - List pool: pre-created empty lists; capacity grows on first resize.
 * - String pool: per-parse short-string cache (<= 64B) backed by an arena and
 *   robin-hood hash map to avoid repeated PyUnicode allocations for repeats.
 *
 * Thread safety: NOT thread-safe (per-thread via thread_local in python_loads.cpp)
 */
class PythonObjectPool {
  public:
    static constexpr size_t kDefaultDictPoolSize = 1024;
    static constexpr size_t kDefaultListPoolSize = 256;
    static constexpr size_t kMaxPooledStringLength = 64;

    PythonObjectPool() = default;

    ~PythonObjectPool() { drain(); }

    PythonObjectPool(const PythonObjectPool&) = delete;
    PythonObjectPool& operator=(const PythonObjectPool&) = delete;

    void configure(size_t pool_size) {
        dict_pool_size_ = pool_size;
        list_pool_size_ = pool_size / 4 == 0 ? kDefaultListPoolSize : pool_size / 4;
    }

    void fill(size_t dict_presize = 0, size_t list_presize = 0) {
        drain();
        dict_presize_ = dict_presize;
        list_presize_ = list_presize;
        dict_pool_.reserve(dict_pool_size_);
        for (size_t i = 0; i < dict_pool_size_; ++i) {
            PyObject* d = dict_presize > 0
                              ? _PyDict_NewPresized(static_cast<Py_ssize_t>(dict_presize))
                              : PyDict_New();
            if (LIKELY(d != nullptr)) {
                dict_pool_.push_back(d);
            }
        }
        list_pool_.reserve(list_pool_size_);
        for (size_t i = 0; i < list_pool_size_; ++i) {
            size_t alloc = list_presize_ > 0 ? list_presize_ : 0;
            PyObject* l = PyList_New(static_cast<Py_ssize_t>(alloc));
            if (LIKELY(l != nullptr)) {
                list_pool_.push_back(l);
            }
        }

        dict_pos_ = 0;
        list_pos_ = 0;
        dicts_from_pool_ = 0;
        dicts_fallback_ = 0;
        lists_from_pool_ = 0;
        lists_fallback_ = 0;

        string_arena_.reset();
        string_pool_.reset(&string_arena_);
    }

    PyObject* acquire_dict(size_t presize) {
        if (LIKELY(dict_pos_ < dict_pool_.size())) {
            dicts_from_pool_++;
            return dict_pool_[dict_pos_++];
        }
        dicts_fallback_++;
        if (presize > 0) {
            return _PyDict_NewPresized(static_cast<Py_ssize_t>(presize));
        }
        return PyDict_New();
    }

    PyObject* acquire_list(size_t presize) {
        if (list_presize_ > 0 && presize == list_presize_ &&
            LIKELY(list_pos_ < list_pool_.size())) {
            lists_from_pool_++;
            return list_pool_[list_pos_++];
        }
        lists_fallback_++;
        return PyList_New(static_cast<Py_ssize_t>(presize));
    }

    PyObject* acquire_string(std::string_view sv, bool has_escapes) {
        if (sv.size() == 0 || sv.size() > kMaxPooledStringLength) {
            return nullptr;
        }
        return string_pool_.get(sv, has_escapes);
    }

    void drain() {
        if (!Py_IsInitialized()) {
            dict_pool_.clear();
            list_pool_.clear();
            string_pool_.release();
            return;
        }
        for (size_t i = dict_pos_; i < dict_pool_.size(); ++i) {
            Py_DECREF(dict_pool_[i]);
        }
        for (size_t i = list_pos_; i < list_pool_.size(); ++i) {
            Py_DECREF(list_pool_[i]);
        }

        // Release capacity if it has grown significantly to avoid high RSS plateau
        if (dict_pool_.capacity() > kDefaultDictPoolSize * 2) {
            std::vector<PyObject*>().swap(dict_pool_);
        } else {
            dict_pool_.clear();
        }

        if (list_pool_.capacity() > kDefaultListPoolSize * 2) {
            std::vector<PyObject*>().swap(list_pool_);
        } else {
            list_pool_.clear();
        }

        dict_pos_ = 0;
        list_pos_ = 0;
        string_pool_.release();
        string_arena_.reset();
    }

    bool is_active() const { return !dict_pool_.empty(); }
    bool list_pool_active() const { return !list_pool_.empty(); }
    bool string_pool_active() const { return string_pool_.is_active(); }

    size_t dicts_from_pool() const { return dicts_from_pool_; }
    size_t dicts_fallback() const { return dicts_fallback_; }
    size_t lists_from_pool() const { return lists_from_pool_; }
    size_t lists_fallback() const { return lists_fallback_; }
    size_t pool_capacity() const { return dict_pool_size_; }

  private:
    struct ShortStringPool {
        static constexpr size_t kMinBuckets = 512;
        static constexpr size_t kMaxBuckets = 32768;
        struct Bucket {
            uint64_t hash = 0;
            const char* data = nullptr;
            uint16_t len = 0;
            PyObject* py = nullptr;
        };

        void reset(strata::util::Arena* arena) {
            arena_ = arena;
            if (buckets_.empty()) {
                init(kMinBuckets);
            }
            // Always release cached objects and clear occupancy on reset.
            release();
            init(kMinBuckets); // Re-initialize after release
            arena_ = arena;    // Restore arena
        }

        PyObject* get(std::string_view sv, bool has_escapes) {
            if (!arena_) return nullptr;
            if (has_escapes) {
                LazyString lazy(sv, true);
                const std::string& unescaped = lazy.value();
                if (unescaped.size() > kMaxPooledStringLength) {
                    return PyUnicode_FromStringAndSize(unescaped.data(), unescaped.size());
                }
                return lookup_or_insert(unescaped);
            }
            return lookup_or_insert(sv);
        }

        PyObject* lookup_or_insert(std::string_view sv) {
            const uint64_t h = fnv1a_hash(sv);
            size_t idx = h & bucket_mask_;
            uint8_t dist = 0;
            while (dist < kMaxProbeDistance) {
                if (bucket_generations_[idx] != generation_) {
                    return insert(idx, h, copy(sv), dist);
                }
                Bucket& b = buckets_[idx];
                if (b.hash == h && b.len == sv.size() &&
                    std::memcmp(b.data, sv.data(), sv.size()) == 0) {
                    Py_INCREF(b.py);
                    return b.py;
                }
                if (dist > distances_[idx]) {
                    return insert(idx, h, copy(sv), dist);
                }
                idx = (idx + 1) & bucket_mask_;
                ++dist;
            }
            return PyUnicode_FromStringAndSize(sv.data(), sv.size());
        }

        bool is_active() const { return arena_ != nullptr; }

        void release() {
            if (Py_IsInitialized()) {
                for (auto& b : buckets_) {
                    if (b.py) {
                        Py_DECREF(b.py);
                        b.py = nullptr;
                    }
                }
            }
            buckets_.clear();
            bucket_generations_.clear();
            distances_.clear();
            bucket_count_ = 0;
            bucket_mask_ = 0;
            arena_ = nullptr;
        }

      private:
        static constexpr uint8_t kMaxProbeDistance = 32;
        strata::util::Arena* arena_ = nullptr;
        size_t bucket_count_ = 0;
        size_t bucket_mask_ = 0;
        std::vector<Bucket> buckets_;
        std::vector<uint8_t> distances_;
        std::vector<uint32_t> bucket_generations_;
        uint32_t generation_ = 0;

        void init(size_t count) {
            bucket_count_ = count;
            bucket_mask_ = bucket_count_ - 1;
            buckets_.assign(bucket_count_, {});
            distances_.assign(bucket_count_, 0);
            bucket_generations_.assign(bucket_count_, 0);
            generation_ = 1;
        }

        std::string_view copy(std::string_view sv) {
            if (!arena_) {
                return sv;
            }
            char* dst =
                static_cast<char*>(arena_->allocate(sv.size() + 1, alignof(char)));
            std::memcpy(dst, sv.data(), sv.size());
            dst[sv.size()] = '\0';
            return std::string_view(dst, sv.size());
        }

        PyObject* insert(size_t idx, uint64_t hash, std::string_view sv, uint8_t dist) {
            // Use direct ASCII allocation when possible — avoids UTF-8 validation overhead.
            PyObject* py;
            if (sv.size() > 0 && is_ascii_only_swar(sv.data(), sv.size())) {
                py = PyUnicode_New(static_cast<Py_ssize_t>(sv.size()), 127);
                if (UNLIKELY(!py)) return nullptr;
                std::memcpy(PyUnicode_1BYTE_DATA(py), sv.data(), sv.size());
            } else {
                py = PyUnicode_FromStringAndSize(sv.data(), sv.size());
            }
            if (!py) {
                return nullptr;
            }
            Bucket& b = buckets_[idx];
            if (b.py) {
                Py_DECREF(b.py);
            }
            b.hash = hash;
            b.data = sv.data();
            b.len = static_cast<uint16_t>(sv.size());
            b.py = py;
            Py_INCREF(py); // Keep one for the cache
            bucket_generations_[idx] = generation_;
            distances_[idx] = dist;
            return py;
        }
    };

    std::vector<PyObject*> dict_pool_;
    std::vector<PyObject*> list_pool_;

    size_t dict_pos_ = 0;
    size_t list_pos_ = 0;
    size_t dict_pool_size_ = kDefaultDictPoolSize;
    size_t list_pool_size_ = kDefaultListPoolSize;
    size_t dict_presize_ = 0;
    size_t list_presize_ = 0;

    size_t dicts_from_pool_ = 0;
    size_t dicts_fallback_ = 0;
    size_t lists_from_pool_ = 0;
    size_t lists_fallback_ = 0;

    strata::util::Arena string_arena_{32 * 1024};
    ShortStringPool string_pool_;
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
    static constexpr size_t kBatchSize = 64;

    using HashAllocator = strata::util::ArenaAllocator<Py_hash_t>;

    PythonObjectBuilder(strata::util::Arena* arena, KeyCache& cache,
                        PythonObjectPool* pool = nullptr)
        : arena_(arena), stack_(StackAllocator(arena)), keys_(StackAllocator(arena)),
          key_hashes_(HashAllocator(arena)),
          list_indices_(SizeAllocator(arena)), list_sizes_(SizeAllocator(arena)),
          dict_key_counts_(SizeAllocator(arena)), dict_estimates_(SizeAllocator(arena)),
          list_estimates_(SizeAllocator(arena)),
          list_object_counts_(SizeAllocator(arena)), list_object_key_sums_(SizeAllocator(arena)),
          list_list_counts_(SizeAllocator(arena)), list_list_size_sums_(SizeAllocator(arena)),
          cache_(cache), pool_(pool) {
        stack_.reserve(32);
        keys_.reserve(32);
        key_hashes_.reserve(32);
        list_indices_.reserve(32);
        list_sizes_.reserve(32);
        dict_key_counts_.reserve(32);
        dict_estimates_.reserve(32);
        list_estimates_.reserve(32);
        list_object_counts_.reserve(32);
        list_object_key_sums_.reserve(32);
        list_list_counts_.reserve(32);
        list_list_size_sums_.reserve(32);
        dict_batches_.reserve(32);

        // Enable detailed tracking only when requested to avoid hot-path overhead.
        const bool track =
            (std::getenv("STRATA_TRACK_PRESIZE") != nullptr) ||
            (std::getenv("STRATA_LOG_PRESIZE") != nullptr);
        estimator_.enable_tracking(track);
    }

    void set_pool(PythonObjectPool* pool) { pool_ = pool; }

    // Approach A: Delayed GC tracking.
    // When enabled, dicts and lists are untracked immediately after creation
    // and re-tracked just before being handed to their parent container.
    // This eliminates the gen0 linked-list insertion cost (~43 cycles/object)
    // during the construction phase while preserving full GC safety at the
    // point where the object becomes visible to other containers.
    void set_deferred_gc_track(bool on) { deferred_gc_track_ = on; }
    bool deferred_gc_track() const { return deferred_gc_track_; }

    bool has_root() const { return root_ != nullptr; }

    PyObject* take_root() {
        PyObject* res = root_;
        root_ = nullptr;
        return res;
    }

    void reset() {
        if (!Py_IsInitialized()) {
            root_ = nullptr;
            clear_vectors();
            current_list_depth_ = 0;
            return;
        }
        clear_pending_batches();
        if (root_) {
            Py_DECREF(root_);
            root_ = nullptr;
        }
        for (auto obj : stack_) {
            if (obj) Py_DECREF(obj);
        }
        for (auto key : keys_) {
            if (key) Py_DECREF(key);
        }
        clear_vectors();
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
         // Optimization: Use PyLong_FromLong for values that fit in a long.
         // This leverages CPython's small integer cache [-5, 256] and has a
         // more direct code path than PyLong_FromLongLong.
         // On 64-bit platforms (macOS arm64), long == int64_t so this always
         // takes the fast path. On 32-bit platforms, falls back correctly.
         // Matches the pattern already used in python_loads.cpp, python_tape.cpp,
         // and python_lazy_cursor.cpp.
         PyObject* obj;
         if (LIKELY(v >= LONG_MIN && v <= LONG_MAX)) {
             obj = PyLong_FromLong(static_cast<long>(v));
         } else {
             obj = PyLong_FromLongLong(v);
         }
         if (UNLIKELY(!obj)) {
             PyErr_SetString(PyExc_MemoryError, "Failed to create PyLong");
             return false;
         }
         return push_value(obj);
     }

     bool on_uint(uint64_t v) override {
         // Optimization: Use faster PyLong_FromLong for values that fit in a long.
         // This also enables use of Python's small integer cache for small values.
         if (LIKELY(v <= static_cast<uint64_t>(LONG_MAX))) {
             return push_value(PyLong_FromLong(static_cast<long>(v)));
         }
         return push_value(PyLong_FromUnsignedLongLong(v));
     }

     bool on_double(double v) override {
         PyObject* obj = PyFloat_FromDouble(v);
         if (!obj) {
             PyErr_SetString(PyExc_MemoryError, "Failed to create PyFloat");
             return false;
         }
         return push_value(obj);
     }

     bool on_string(std::string_view v, bool has_escapes = false) override {
         if (has_escapes) {
             LazyString lazy(v, true);
             const std::string& unescaped = lazy.value();
             // Approach A: use ShortStringPool for short escaped strings
             if (pool_ && pool_->string_pool_active() &&
                 unescaped.size() <= PythonObjectPool::kMaxPooledStringLength) {
                 PyObject* cached = pool_->acquire_string(unescaped, false);
                 if (cached) return push_value(cached);
             }
             return push_value(PyUnicode_FromStringAndSize(unescaped.data(), unescaped.size()));
         }
         // Approach A: use ShortStringPool for short repeated string values
         if (pool_ && pool_->string_pool_active() &&
             v.size() <= PythonObjectPool::kMaxPooledStringLength) {
             PyObject* cached = pool_->acquire_string(v, false);
             if (cached) return push_value(cached);
         }
         // Approach B: direct ASCII allocation bypasses CPython encoding detection
         // is_ascii_only_swar is a branchless 8-byte SWAR check — cheaper than
         // PyUnicode_FromStringAndSize's internal UTF-8 validation loop for ASCII strings.
         if (v.size() > 0 && is_ascii_only_swar(v.data(), v.size())) {
             PyObject* s = PyUnicode_New(static_cast<Py_ssize_t>(v.size()), 127);
             if (UNLIKELY(!s)) return false;
             std::memcpy(PyUnicode_1BYTE_DATA(s), v.data(), v.size());
             return push_value(s);
         }
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

        // Approach A: Delayed GC tracking.
        // PyDict_New/_PyDict_NewPresized immediately inserts the dict into gen0's
        // doubly-linked list (~43 cycles: 5 pointer stores + an atomic counter
        // increment).  We have exclusive ownership during construction so GC
        // cannot observe the object.  Untrack now; re-track in on_end_object()
        // before the dict is handed to its parent container.
        if (deferred_gc_track_) {
            PyObject_GC_UnTrack(dict);
        }

         stack_.push_back(dict);
         // Track that we're building a dict to count keys later
         dict_key_counts_.push_back(0);
         dict_estimates_.push_back(presize);
         dict_batches_.push_back({});
         return true;
     }

     bool on_key(std::string_view v, bool has_escapes = false) override {
         Py_hash_t hash;
         if (has_escapes) {
             // Use LazyString to unescape the key before caching
             LazyString lazy(v, true);
             const std::string& unescaped = lazy.value();
             PyObject* key = cache_.get_with_hash(std::string_view(unescaped), &hash);
             if (!key)
                 return false;
             keys_.push_back(key);
             key_hashes_.push_back(hash);
             // Track key count for adaptive estimation
             if (!dict_key_counts_.empty()) {
                 dict_key_counts_.back()++;
             }
             return true;
         }
         PyObject* key = cache_.get_with_hash(v, &hash);
         if (!key)
             return false;
         keys_.push_back(key);
         key_hashes_.push_back(hash);
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

        if (!flush_current_dict_batch(dict)) {
            Py_DECREF(dict);
            return false;
        }
        if (!dict_batches_.empty()) {
            dict_batches_.pop_back();
        }

        // Approach A: Re-enable GC tracking before the dict is handed to its
        // parent.  The dict is now fully constructed with correct refcounts;
        // GC can safely traverse it from this point forward.
        if (deferred_gc_track_) {
            PyObject_GC_Track(dict);
        }

        return push_value(dict);
    }

    bool on_start_array(size_t size_hint) override {
        size_t presize = compute_list_presize(size_hint);

        PyObject* list = nullptr;
        if (pool_ && pool_->list_pool_active()) {
            list = pool_->acquire_list(presize);
        } else {
            list = PyList_New(presize);
        }
        if (!list)
            return false;

        // Approach A: Delayed GC tracking for lists (same rationale as dicts).
        if (deferred_gc_track_) {
            PyObject_GC_UnTrack(list);
        }

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

        if (allocated_size > 0 && actual_size < allocated_size) {
            if (PyList_SetSlice(list, actual_size, allocated_size, NULL) < 0) {
                Py_DECREF(list);
                return false;
            }
        }

         // If parent is a list (array of arrays), track sibling average
         if (current_list_depth_ > 0 && current_list_depth_ <= list_list_counts_.size()) {
             size_t parent_idx = current_list_depth_ - 1;
             list_list_counts_[parent_idx] += 1;
             list_list_size_sums_[parent_idx] += actual_size;
         }

        // Approach A: Re-enable GC tracking before the list is handed to its parent.
        if (deferred_gc_track_) {
            PyObject_GC_Track(list);
        }

        return push_value(list);
    }

    // Get the adaptive estimator's current dict size estimate (for pool pre-sizing)
    size_t estimate_dict_presize() const {
        return estimator_.estimate_dict_size();
    }

    size_t estimate_list_presize() const {
        return estimator_.estimate_list_size();
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
    struct DictBatch {
        std::array<PyObject*, kBatchSize> keys{};
        std::array<PyObject*, kBatchSize> values{};
        std::array<Py_hash_t, kBatchSize> hashes{};
        size_t size = 0;
        // When needs_first_wins_fixup is true, we batched using LastWins semantics
        // (no per-key GetItem) but the policy is FirstWins.  flush_current_dict_batch
        // checks whether any keys were silently overwritten and restores the first
        // value if so.  In practice duplicates are vanishingly rare so this runs
        // as a cheap size-comparison check and exits immediately.
        bool needs_first_wins_fixup = false;
    };

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

    void clear_vectors() {
        // Using swap with a fresh vector to truly clear capacity and avoid
        // using stale pointers when the arena is reset.
        std::vector<PyObject*, StackAllocator>(StackAllocator(arena_)).swap(stack_);
        std::vector<PyObject*, StackAllocator>(StackAllocator(arena_)).swap(keys_);
        std::vector<Py_hash_t, HashAllocator>(HashAllocator(arena_)).swap(key_hashes_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_indices_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_sizes_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(dict_key_counts_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(dict_estimates_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_estimates_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_object_counts_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_object_key_sums_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_list_counts_);
        std::vector<size_t, SizeAllocator>(SizeAllocator(arena_)).swap(list_list_size_sums_);
        dict_batches_.clear();

        stack_.reserve(32);
        keys_.reserve(32);
        key_hashes_.reserve(32);
        list_indices_.reserve(32);
        list_sizes_.reserve(32);
        dict_key_counts_.reserve(32);
        dict_estimates_.reserve(32);
        list_estimates_.reserve(32);
    }

    void clear_pending_batches() {
        for (auto& batch : dict_batches_) {
            for (size_t i = 0; i < batch.size; ++i) {
                Py_DECREF(batch.keys[i]);
                Py_DECREF(batch.values[i]);
            }
            batch.size = 0;
        }
        dict_batches_.clear();
    }

    bool flush_current_dict_batch(PyObject* dict) {
        if (dict_batches_.empty()) {
            return true;
        }
        DictBatch& batch = dict_batches_.back();
        if (batch.size == 0) {
            return true;
        }

        if (batch.needs_first_wins_fixup) {
            // FirstWins policy: insert only if key is not already present.
            // We do GetItem+SetItem here (at flush) rather than in push_value,
            // which gives better cache locality — all dict ops are sequential
            // in this loop rather than interleaved with SAX parser callbacks.
            for (size_t i = 0; i < batch.size; ++i) {
                PyObject* key = batch.keys[i];
                PyObject* val = batch.values[i];
                PyObject* existing = _PyDict_GetItem_KnownHash(dict, key, batch.hashes[i]);
                if (existing) {
                    // Key already present — keep first value.
                    Py_DECREF(key);
                    Py_DECREF(val);
                    continue;
                }
                if (PyErr_Occurred()) {
                    for (size_t j = i; j < batch.size; ++j) {
                        Py_DECREF(batch.keys[j]);
                        Py_DECREF(batch.values[j]);
                    }
                    batch.size = 0;
                    return false;
                }
                if (_PyDict_SetItem_KnownHash(dict, key, val, batch.hashes[i]) < 0) {
                    for (size_t j = i; j < batch.size; ++j) {
                        Py_DECREF(batch.keys[j]);
                        Py_DECREF(batch.values[j]);
                    }
                    batch.size = 0;
                    return false;
                }
                Py_DECREF(key);
                Py_DECREF(val);
            }
            batch.needs_first_wins_fixup = false;
            batch.size = 0;
            return true;
        }

        // LastWins / no-fixup path: pure SetItem batch (no dup check).
        for (size_t i = 0; i < batch.size; ++i) {
            PyObject* key = batch.keys[i];
            PyObject* val = batch.values[i];
            // Use _PyDict_SetItem_KnownHash to skip redundant hash computation.
            // The hash was pre-computed when the key was cached/interned and stored
            // in batch.hashes[i].
            if (_PyDict_SetItem_KnownHash(dict, key, val, batch.hashes[i]) < 0) {
                for (size_t j = i; j < batch.size; ++j) {
                    Py_DECREF(batch.keys[j]);
                    Py_DECREF(batch.values[j]);
                }
                batch.size = 0;
                return false;
            }
            Py_DECREF(key);
            Py_DECREF(val);
        }
        batch.size = 0;
        return true;
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
             Py_hash_t hash = key_hashes_.back();
             key_hashes_.pop_back();

             const auto policy = strata::get_duplicate_key_policy();
             if (!dict_batches_.empty() &&
                 (policy == strata::DuplicateKeyPolicy::LastWins ||
                  policy == strata::DuplicateKeyPolicy::FirstWins)) {
                 // Batched fast path for LastWins and FirstWins.
                 // For LastWins: pure SetItem at flush (no dup check).
                 // For FirstWins: deferred GetItem+SetItem at flush for better
                 //   cache locality — all dict ops are sequential in flush_current_dict_batch
                 //   rather than interleaved with SAX parse callbacks.
                 //   Real-world JSON rarely has duplicates so the GetItem almost
                 //   always finds nothing, and running them in a tight flush loop
                 //   keeps them in L1/L2 cache.
                 DictBatch& batch = dict_batches_.back();
                 if (policy == strata::DuplicateKeyPolicy::FirstWins) {
                     batch.needs_first_wins_fixup = true;
                 }
                 batch.keys[batch.size] = key;
                 batch.values[batch.size] = val;
                 batch.hashes[batch.size] = hash;
                 batch.size++;
                 if (batch.size >= kBatchSize) {
                     if (!flush_current_dict_batch(top)) {
                         return false;
                     }
                 }
                 return true;
             }

             // Unbatched path: Warn / Error policies, or no batch available.
             // Check for duplicates inline.
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
                 case strata::DuplicateKeyPolicy::LastWins:
                     break;  // fall through to insert (overwrite)
                 case strata::DuplicateKeyPolicy::Warn: {
                     PyObject* key_repr = PyObject_Repr(key);
                     const char* key_str = PyUnicode_AsUTF8(key_repr);
                     std::string msg = "Duplicate key encountered: ";
                     msg += key_str;
                     PyErr_WarnEx(PyExc_RuntimeWarning, msg.c_str(), 1);
                     Py_XDECREF(key_repr);
                     // Warn keeps the first value (same as FirstWins).
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
             // Insert with known hash — eliminates redundant hash computation
             if (_PyDict_SetItem_KnownHash(top, key, val, hash) < 0) {
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
   std::vector<Py_hash_t, HashAllocator> key_hashes_;
  std::vector<size_t, SizeAllocator> list_indices_;
  std::vector<size_t, SizeAllocator> list_sizes_;
  std::vector<size_t, SizeAllocator> dict_key_counts_;  // Track keys per dict for estimation
  std::vector<size_t, SizeAllocator> dict_estimates_;
  std::vector<size_t, SizeAllocator> list_estimates_;
  std::vector<size_t, SizeAllocator> list_object_counts_;
  std::vector<size_t, SizeAllocator> list_object_key_sums_;
  std::vector<size_t, SizeAllocator> list_list_counts_;
  std::vector<size_t, SizeAllocator> list_list_size_sums_;
  std::vector<DictBatch> dict_batches_;
  size_t current_list_depth_ = 0;  // O(1) list depth tracking
  KeyCache& cache_;
  PythonObjectPool* pool_ = nullptr;  // Optional dict pool (per-parse-session)
  AdaptiveSizeEstimator estimator_;  // Learn from observed sizes
  bool deferred_gc_track_ = false;   // Approach A: untrack on create, retrack on end
};

 } // namespace bindings
 } // namespace strata
