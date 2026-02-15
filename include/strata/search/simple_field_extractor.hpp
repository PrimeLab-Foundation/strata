#pragma once

#include "strata/json/json_parse.hpp"
#include "strata/util/lazy_string.hpp"
#include "strata/search/search_ndjson_fused.hpp"

#include <string_view>
#include <vector>

namespace strata {
namespace internal {

enum class ContainerKind { Object, Array };

template <typename Builder, typename ResultType>
class SimpleFieldExtractorBase : public JsonSaxHandler {
  public:
    SimpleFieldExtractorBase(std::string_view target, SimpleFieldMode mode,
                             std::vector<ResultType>& matches, Builder& builder)
        : target_(target), mode_(mode), matches_(matches), builder_(builder) {
        abort_on_match_ = (mode_ == SimpleFieldMode::RootField);
        reset_state();
    }

    bool on_null() override { return handle_primitive([&]() { return builder_.on_null(); }); }
    bool on_bool(bool v) override {
        return handle_primitive([&]() { return builder_.on_bool(v); });
    }
    bool on_int(int64_t v) override {
        return handle_primitive([&]() { return builder_.on_int(v); });
    }
    bool on_uint(uint64_t v) override {
        return handle_primitive([&]() { return builder_.on_uint(v); });
    }
    bool on_double(double v) override {
        return handle_primitive([&]() { return builder_.on_double(v); });
    }

    bool on_string(std::string_view v, bool has_escapes = false) override {
        return handle_primitive([&]() { return builder_.on_string(v, has_escapes); });
    }

    bool on_start_object(size_t size_hint = 0) override {
        bool is_root = stack_.empty() && !root_seen_;
        if (is_root) {
            root_seen_ = true;
            root_container_ = ContainerKind::Object;
            if (mode_ == SimpleFieldMode::RootField) {
                start_candidate();
            }
        } else {
            if (candidate_depth_ >= 0) {
                candidate_depth_++;
            } else if (mode_ == SimpleFieldMode::RootWildcardField &&
                       is_root_child_candidate()) {
                start_candidate();
            }
        }

        if (!handle_start_container([&]() { return builder_.on_start_object(size_hint); })) {
            return false;
        }

        stack_.push_back(ContainerKind::Object);
        return true;
    }

    bool on_key(std::string_view v, bool has_escapes = false) override {
        if (capturing_) {
            if (!builder_.on_key(v, has_escapes)) {
                return false;
            }
        }
        if (capturing_ || pending_match_ || candidate_depth_ != 0 || candidate_matched_) {
            return true;
        }

        if (!has_escapes) {
            if (v == target_) {
                pending_match_ = true;
            }
            return true;
        }

        LazyString lazy(v, true);
        if (lazy.value() == target_) {
            pending_match_ = true;
        }
        return true;
    }

    bool on_end_object() override {
        if (!handle_end_container([&]() { return builder_.on_end_object(); })) {
            return false;
        }

        if (candidate_depth_ >= 0) {
            if (candidate_depth_ == 0) {
                candidate_depth_ = -1;
                candidate_matched_ = false;
                pending_match_ = false;
            } else {
                candidate_depth_--;
            }
        }

        if (!stack_.empty()) {
            stack_.pop_back();
        }
        return true;
    }

    bool on_start_array(size_t size_hint = 0) override {
        bool is_root = stack_.empty() && !root_seen_;
        if (is_root) {
            root_seen_ = true;
            root_container_ = ContainerKind::Array;
        } else if (candidate_depth_ >= 0) {
            candidate_depth_++;
        }

        if (!handle_start_container([&]() { return builder_.on_start_array(size_hint); })) {
            return false;
        }

        stack_.push_back(ContainerKind::Array);
        return true;
    }

    bool on_end_array() override {
        if (!handle_end_container([&]() { return builder_.on_end_array(); })) {
            return false;
        }

        if (candidate_depth_ > 0) {
            candidate_depth_--;
        }

        if (!stack_.empty()) {
            stack_.pop_back();
        }
        return true;
    }

  private:
    void reset_state() {
        stack_.clear();
        root_seen_ = false;
        candidate_depth_ = -1;
        candidate_matched_ = false;
        pending_match_ = false;
        capturing_ = false;
        capture_depth_ = 0;
        builder_.reset();
    }

    void start_candidate() {
        candidate_depth_ = 0;
        candidate_matched_ = false;
        pending_match_ = false;
    }

    bool is_root_child_candidate() const {
        if (stack_.size() != 1) {
            return false;
        }
        if (root_container_ == ContainerKind::Array && stack_.back() == ContainerKind::Array) {
            return true;
        }
        if (root_container_ == ContainerKind::Object && stack_.back() == ContainerKind::Object) {
            return true;
        }
        return false;
    }

    bool start_capture() {
        capturing_ = true;
        capture_depth_ = 0;
        pending_match_ = false;
        candidate_matched_ = true;
        builder_.reset();
        return true;
    }

    bool finalize_capture() {
        if (builder_.has_root()) {
            matches_.push_back(builder_.take_root());
        }
        builder_.reset();
        capturing_ = false;
        pending_match_ = false;
        if (abort_on_match_) {
            return false;
        }
        return true;
    }

    template <typename Fn>
    bool handle_primitive(Fn&& fn) {
        if (pending_match_) {
            start_capture();
            if (!fn()) {
                return false;
            }
            return finalize_capture();
        }
        if (capturing_) {
            if (!fn()) {
                return false;
            }
            if (capture_depth_ == 0) {
                return finalize_capture();
            }
        }
        return true;
    }

    template <typename Fn>
    bool handle_start_container(Fn&& fn) {
        if (pending_match_) {
            start_capture();
            capture_depth_ = 1;
            if (!fn()) {
                return false;
            }
            return true;
        }
        if (capturing_) {
            capture_depth_++;
            return fn();
        }
        return true;
    }

    template <typename Fn>
    bool handle_end_container(Fn&& fn) {
        if (!capturing_) {
            return true;
        }
        if (!fn()) {
            return false;
        }
        if (capture_depth_ > 0) {
            capture_depth_--;
        }
        if (capture_depth_ == 0) {
            return finalize_capture();
        }
        return true;
    }

    std::string_view target_;
    SimpleFieldMode mode_;
    std::vector<ResultType>& matches_;
    Builder& builder_;
    std::vector<ContainerKind> stack_;
    ContainerKind root_container_ = ContainerKind::Object;
    bool root_seen_ = false;
    int candidate_depth_ = -1;
    bool candidate_matched_ = false;
    bool pending_match_ = false;
    bool capturing_ = false;
    int capture_depth_ = 0;
    bool abort_on_match_ = false;
};

} // namespace internal
} // namespace strata
