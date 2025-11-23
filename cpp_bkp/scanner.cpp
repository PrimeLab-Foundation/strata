#include "tjp/scanner.hpp"

#include "tjp/utf8.hpp"

namespace tjp {

Result<Tape> scan_structurals(const char* data, size_t len, const ScanOptions& opts) {
    if (opts.validate_utf8) {
        auto e = validate_utf8(data, len);
        if (e.code != ErrorCode::Ok)
            return e;
    }
    Tape t;
    t.entries.reserve(len / 8);
    bool in_string = false;
    bool escape = false;
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        if (in_string) {
            if (escape) {
                escape = false;
                continue;
            }
            if (c == '\\') {
                escape = true;
                continue;
            }
            if (c == '"') {
                in_string = false;
                t.entries.push_back({TapeType::Quote, static_cast<uint32_t>(i)});
            }
            continue;
        } else {
            if (c == '"') {
                in_string = true;
                t.entries.push_back({TapeType::Quote, static_cast<uint32_t>(i)});
                continue;
            }
        }
        TapeType type;
        switch (c) {
        case '{':
            type = TapeType::LBrace;
            break;
        case '}':
            type = TapeType::RBrace;
            break;
        case '[':
            type = TapeType::LBracket;
            break;
        case ']':
            type = TapeType::RBracket;
            break;
        case ':':
            type = TapeType::Colon;
            break;
        case ',':
            type = TapeType::Comma;
            break;
        default:
            continue;
        }
        t.entries.push_back({type, static_cast<uint32_t>(i)});
    }
    // If ended inside a string, treat as error in future; for MVP we ignore.
    return t;
}

} // namespace tjp
