# Security Hardening Checklist

## ✅ **Already Implemented** (Strong Foundation)

1. **UTF-8 Validation**: SIMD-optimized validation in `src/strata/util/simd_utf8.cpp`
2. **Memory Safety**: RAII, smart pointers, arena allocators
3. **Error Handling**: Consistent error propagation from C++ to Python
4. **Input Validation**: Proper JSON syntax validation

## 🔧 **Recommended Enhancements**

### 1. Resource Limits (DoS Protection)
```cpp
// Add to src/strata/json/json_parse.hpp
struct ParseLimits {
    size_t max_depth = 1000;           // Prevent stack overflow
    size_t max_string_length = 1<<20;  // 1MB string limit
    size_t max_array_size = 1<<20;     // 1M elements max
    size_t max_object_keys = 1<<16;    // 64K keys max
    size_t max_total_size = 1<<26;     // 64MB total JSON limit
};

class JsonParser {
    ParseLimits limits_;
    size_t current_depth_ = 0;
    
    void check_limits();  // Called at critical points
};
```

### 2. Fuzzing Integration Enhancement
```bash
# Expand scripts/fuzz.sh
fuzz-all:
    # Add more fuzz targets
    @bash scripts/fuzz.sh build
    @bash scripts/fuzz.sh run-loads
    @bash scripts/fuzz.sh run-ndjson  
    @bash scripts/fuzz.sh run-jsonpath  # New
    @bash scripts/fuzz.sh run-serialize # New
```

### 3. Integer Overflow Protection
```cpp
// Add to src/strata/util/safe_arithmetic.hpp
template<typename T>
bool safe_add(T a, T b, T& result) {
    if (a > std::numeric_limits<T>::max() - b) return false;
    result = a + b;
    return true;
}
```

### 4. Security Documentation
**Add**: `docs/security/security_guide.md`

```markdown
# Security Guide

## Safe Usage Patterns
1. **Set resource limits** when parsing untrusted input
2. **Validate input size** before parsing large files
3. **Use timeouts** for long-running operations

## Security Best Practices
- Never parse untrusted JSON without limits
- Use memory-mapped files carefully with untrusted input
- Monitor memory usage in production
```

## 🛡️ **Defense in Depth Strategy**

1. **Input Layer**: Size limits, syntax validation
2. **Parser Layer**: Depth limits, resource tracking  
3. **Memory Layer**: Arena allocation, bounds checking
4. **Output Layer**: UTF-8 validation, escape verification

---

**Implementation Priority**: Resource limits (immediate), then fuzzing expansion.