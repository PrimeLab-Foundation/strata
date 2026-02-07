# API Enhancement Roadmap

**Status**: Current API is solid and complete for core JSON operations  
**Opportunity**: Add modern features that competitors offer

## 🆕 **Proposed API Additions**

### 1. Schema Validation (High Value)
```python
import strata

# JSON Schema validation
schema = {
    "type": "object", 
    "properties": {"name": {"type": "string"}, "age": {"type": "number"}}
}

# Validate during parsing
data = strata.loads(json_str, schema=schema)  # Raises ValidationError if invalid

# Pre-compile schema for reuse
validator = strata.compile_schema(schema)
data = strata.loads(json_str, validator=validator)
```

**Implementation**: Add `src/strata/validation/` module

### 2. Async/Streaming API (Medium Value)
```python
import strata

# Async NDJSON processing
async for record in strata.iter_ndjson_async("large_file.ndjson"):
    await process(record)

# Streaming parser for huge JSON files
async for chunk in strata.parse_json_stream("huge_file.json"):
    process_chunk(chunk)
```

### 3. Enhanced Configuration (Low-Medium Value)
```python
import strata

# More granular control
config = strata.Config(
    max_depth=1000,          # Prevent stack overflow
    max_string_length=1<<20, # Security: limit string size
    allow_comments=True,     # Non-standard JSON with comments
    allow_trailing_commas=True, # Relaxed parsing
    preserve_order=True,     # Use OrderedDict instead of dict
)

data = strata.loads(json_str, config=config)
```

### 4. Performance Profiling Hooks (Developer Value)
```python
import strata

# Built-in performance monitoring
with strata.ProfileContext() as prof:
    data = strata.loads(large_json)
    
print(f"Parse time: {prof.parse_time_ms}ms")
print(f"Memory used: {prof.peak_memory_mb}MB")
print(f"Objects created: {prof.object_count}")
```

## 🎯 **Implementation Priority**

### Phase A: Schema Validation (High Impact)
- **Business Value**: Enterprise feature, validation is critical
- **Differentiation**: Not all JSON libraries have this
- **Implementation**: 2-3 weeks

### Phase B: Enhanced Configuration (Medium Impact)  
- **Business Value**: Developer experience improvement
- **Implementation**: 1 week

### Phase C: Performance Profiling (Developer Value)
- **Business Value**: Debugging and optimization tool
- **Implementation**: 1 week

### Phase D: Async API (Future)
- **Business Value**: Modern async/await paradigm
- **Implementation**: 3-4 weeks (major undertaking)

## 🚫 **Explicitly NOT Adding** (Maintains Focus)

1. **XML Support**: Outside core mission
2. **YAML Support**: Different format, not JSON
3. **Custom Encoders/Decoders**: Adds complexity, limited value
4. **Pandas Integration**: Keep dependencies minimal per Rule 2

## 📏 **API Design Principles**

1. **Backward Compatible**: All existing code continues working
2. **Optional Features**: New features are opt-in, don't slow down core path
3. **Consistent**: Follow existing naming patterns
4. **Minimal**: Only add features with clear business value
5. **Rule Compliant**: All logic in C++, Python is thin wrapper

## 🛠️ **Implementation Guidelines**

### Schema Validation Example:
```cpp
// src/strata/validation/json_schema.hpp
namespace strata::validation {
    class JsonSchema {
        bool validate(const JsonValue& value) const;
        std::vector<ValidationError> get_errors() const;
    };
}

// Python binding in src/strata/bindings/python_validation.cpp
PyObject* strata_compile_schema(PyObject* self, PyObject* args) {
    // Thin wrapper around C++ JsonSchema
}
```

---

**Recommendation**: Start with Schema Validation (Phase A) as it provides the highest business value and differentiates Strata in the enterprise market.