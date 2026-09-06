// The serializer's static tables and per-thread state, in bytes.
//
// The cold-state probe reads `dumps mixed` ahead of orjson hot and behind it
// cold on the Neoverse-N2, with strata's cold penalty the larger by several
// microseconds, and the standing conclusion ("the serializer's footprint on
// smaller caches") has never had a number attached. This prints the data half
// of that footprint: every constant-sized table the writers index, and the
// per-thread schema state a dumps() call touches at each nesting depth. The
// code half is the extension's own text size, which `size`/`nm` report on the
// built object in the same job.
//
// Build (needs the CPython headers the bindings include):
//   clang++ -O2 -std=c++20 -I include -I src/strata/bindings \
//       $(python3-config --includes) experiments/footprint/footprint_probe.cpp \
//       -o footprint_probe
// Never linked into production (docs/context/convention.md, Experiments).

#include "python_dumps_output.h"
#include "strata/util/dtoa.hpp"
#include "strata/util/fast_parse.hpp"

#include <cstdio>

using strata::bindings::SchemaCacheLease;
using strata::bindings::StagedOutput;

namespace {

void line(const char* name, size_t bytes) { std::printf("%-46s %8zu bytes\n", name, bytes); }

} // namespace

int main() {
    std::printf("== static tables the writers index\n");
    line("util::detail::kDigitPairs (itoa)", sizeof(strata::util::detail::kDigitPairs));
    line("util::detail::kPow10 (digit count)", sizeof(strata::util::detail::kPow10));
    line("util::kRunPow10 (parse digit runs)", sizeof(strata::util::detail::kRunPow10));
    line("util::kClingerPow10 (parse fast path)", sizeof(strata::util::detail::kClingerPow10));

    std::printf("\n== per-call output state (stack)\n");
    line("StagedOutput (whole object)", sizeof(StagedOutput));
    line("  of which the stage", StagedOutput::kStageBytes);
    line("StagedOutput::kMaxReservation", StagedOutput::kMaxReservation);
    line("SchemaCacheLease (the lease itself)", sizeof(SchemaCacheLease));

    std::printf("\n== per-thread schema cache (heap, one vector per thread)\n");
    line("Schema (one way)", sizeof(SchemaCacheLease::Schema));
    line("  of which slots[24][16]", SchemaCacheLease::kSchemaSlots * SchemaCacheLease::kSlotBytes);
    line("  of which key_row[24]", SchemaCacheLease::kSchemaSlots * sizeof(PyObject*));
    line("  of which spans[24]", SchemaCacheLease::kSchemaSlots);
    line("DepthSchemas (four ways + the scan row)", sizeof(SchemaCacheLease::DepthSchemas));
    line("  of which the scanned counts[4]+first_keys[4]",
         SchemaCacheLease::DepthSchemas::kWays * (sizeof(Py_ssize_t) + sizeof(PyObject*)));
    for (size_t depths = 1; depths <= 4; ++depths) {
        char label[64];
        std::snprintf(label, sizeof label, "live state at %zu dict depth(s)", depths);
        line(label, depths * sizeof(SchemaCacheLease::DepthSchemas));
    }
    std::printf("\n(mixed.json nests dicts two deep: the top record, and the\n"
                "payload/meta object inside two of its four shapes.)\n");
    return 0;
}
