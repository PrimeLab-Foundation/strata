// Extended tests for OutputBuffer and FixedOutputBuffer.
// Covers: arena ctor, reset_with_arena, data()/capacity() accessors,
//         ensure_extra/push_back_unchecked, try_grow_arena, grow_heap
//         (arena→heap transition), and full FixedOutputBuffer paths.

#include "strata/util/output_buffer.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

using namespace strata::util;

static std::string buf_str(const OutputBuffer& b) {
    return std::string(b.data(), b.size());
}

// ---------------------------------------------------------------------------
// OutputBuffer — arena constructor and reset_with_arena
// ---------------------------------------------------------------------------

static void test_output_buffer_arena_ctor() {
    Arena arena;
    OutputBuffer buf(&arena);
    // data()/capacity() accessors
    assert(buf.size() == 0);
    buf.append("hello", 5);
    assert(buf.size() == 5);
    assert(buf_str(buf) == "hello");
    std::cout << "  output_buffer_arena_ctor ok\n";
}

static void test_output_buffer_reset_with_arena() {
    OutputBuffer buf; // heap mode
    buf.append("heap data", 9);

    Arena arena;
    buf.reset_with_arena(&arena); // clears to arena mode
    assert(buf.size() == 0);
    // data() and capacity() should be zero after reset
    assert(buf.capacity() == 0);

    buf.append("arena data", 10);
    assert(buf_str(buf) == "arena data");
    std::cout << "  output_buffer_reset_with_arena ok\n";
}

// ---------------------------------------------------------------------------
// OutputBuffer — data() and capacity() accessors
// ---------------------------------------------------------------------------

static void test_output_buffer_data_capacity() {
    OutputBuffer buf;
    buf.reserve(128);
    // capacity() should reflect the reservation
    assert(buf.capacity() >= 128);
    // data() should return a non-null pointer after reservation
    assert(buf.data() != nullptr);
    // non-const data()
    char* d = buf.data();
    assert(d != nullptr);
    std::cout << "  output_buffer_data_capacity ok\n";
}

// ---------------------------------------------------------------------------
// OutputBuffer — ensure_extra / push_back_unchecked
// ---------------------------------------------------------------------------

static void test_output_buffer_ensure_extra_and_unchecked() {
    OutputBuffer buf;
    buf.append("AB", 2);

    // Reserve space for 4 bytes of structural chars
    buf.ensure_extra(4);

    // push_back_unchecked writes without calling ensure()
    buf.push_back_unchecked('{');
    buf.push_back_unchecked('"');
    buf.push_back_unchecked('x');
    buf.push_back_unchecked('}');

    assert(buf.size() == 6);
    assert(buf_str(buf) == "AB{\"x}");
    std::cout << "  output_buffer_ensure_extra_and_unchecked ok\n";
}

static void test_output_buffer_ensure_extra_zero() {
    OutputBuffer buf;
    // ensure_extra(0) is a no-op
    buf.ensure_extra(0);
    buf.append("ok", 2);
    assert(buf_str(buf) == "ok");
    std::cout << "  output_buffer_ensure_extra_zero ok\n";
}

// ---------------------------------------------------------------------------
// OutputBuffer — arena→heap fallback (try_grow_arena / grow_heap)
// ---------------------------------------------------------------------------

static void test_output_buffer_arena_grows_large() {
    // Start with small arena, grow buffer beyond arena capacity to trigger
    // the arena doubling path inside try_grow_arena.
    Arena arena(256); // tiny arena
    OutputBuffer buf(&arena);

    // Repeatedly append to force arena growth (doubling)
    for (int i = 0; i < 10; ++i) {
        std::string chunk(64, 'A' + i);
        buf.append(chunk.data(), chunk.size());
    }
    assert(buf.size() == 640);
    // All data should be readable
    for (int i = 0; i < 10; ++i) {
        assert(buf.data()[i * 64] == char('A' + i));
    }
    std::cout << "  output_buffer_arena_grows_large ok\n";
}

static void test_output_buffer_reserve_with_reserved_extra() {
    OutputBuffer buf;
    buf.append("hello", 5);
    // ensure_extra sets reserved_extra_; reserve() factors it in
    buf.ensure_extra(16);
    buf.reserve(10); // less than size_ + reserved_extra_ — should expand
    assert(buf.capacity() >= 5 + 16);
    std::cout << "  output_buffer_reserve_with_reserved_extra ok\n";
}

static void test_output_buffer_grow_beyond_initial() {
    // Force the heap-path grow by appending >1KB in one shot
    OutputBuffer buf;
    std::string big(4096, 'Z');
    buf.append(big.data(), big.size());
    assert(buf.size() == 4096);
    assert(buf.data()[0] == 'Z');
    assert(buf.data()[4095] == 'Z');
    std::cout << "  output_buffer_grow_beyond_initial ok\n";
}

// ---------------------------------------------------------------------------
// FixedOutputBuffer — full path coverage
// ---------------------------------------------------------------------------

static void test_fixed_output_buffer_basic() {
    char storage[32];
    FixedOutputBuffer fb(storage, sizeof(storage));

    assert(fb.size() == 0);
    // data() (const and non-const), capacity(), overflowed()
    assert(fb.capacity() == 32);
    assert(!fb.overflowed());
    // non-const data
    char* d = fb.data();
    assert(d == storage);
    // const data
    const FixedOutputBuffer& cfb = fb;
    const char* cd = cfb.data();
    assert(cd == storage);

    fb.append("hello", 5);
    assert(fb.size() == 5);
    assert(!fb.overflowed());
    std::cout << "  fixed_output_buffer_basic ok\n";
}

static void test_fixed_output_buffer_overflow_append() {
    char storage[8];
    FixedOutputBuffer fb(storage, sizeof(storage));
    fb.append("12345678", 8); // exactly fills
    assert(!fb.overflowed());
    // One more byte overflows
    fb.append("X", 1);
    assert(fb.overflowed());
    // After overflow, further appends are no-ops
    size_t sz = fb.size();
    fb.append("more", 4);
    assert(fb.size() == sz);
    std::cout << "  fixed_output_buffer_overflow_append ok\n";
}

static void test_fixed_output_buffer_overflow_push_back() {
    char storage[4];
    FixedOutputBuffer fb(storage, sizeof(storage));
    fb.push_back('a');
    fb.push_back('b');
    fb.push_back('c');
    fb.push_back('d');
    assert(!fb.overflowed());
    fb.push_back('e'); // overflow
    assert(fb.overflowed());
    // push_back is a no-op after overflow
    size_t sz = fb.size();
    fb.push_back('f');
    assert(fb.size() == sz);
    std::cout << "  fixed_output_buffer_overflow_push_back ok\n";
}

static void test_fixed_output_buffer_push_back_unchecked() {
    char storage[16];
    FixedOutputBuffer fb(storage, sizeof(storage));
    // push_back_unchecked delegates to push_back in FixedOutputBuffer
    fb.push_back_unchecked('X');
    fb.push_back_unchecked('Y');
    assert(fb.size() == 2);
    assert(storage[0] == 'X' && storage[1] == 'Y');
    std::cout << "  fixed_output_buffer_push_back_unchecked ok\n";
}

static void test_fixed_output_buffer_clear() {
    char storage[8];
    FixedOutputBuffer fb(storage, sizeof(storage));
    fb.append("hello", 5);
    fb.append("XXX", 3); // fills it
    fb.append("!",   1); // overflow
    assert(fb.overflowed());
    fb.clear();
    assert(fb.size() == 0);
    assert(!fb.overflowed());
    fb.append("ok", 2);
    assert(fb.size() == 2);
    std::cout << "  fixed_output_buffer_clear ok\n";
}

static void test_fixed_output_buffer_reserve_ensure_extra_noop() {
    char storage[16];
    FixedOutputBuffer fb(storage, sizeof(storage));
    // Both are no-ops in FixedOutputBuffer
    fb.reserve(100);
    fb.ensure_extra(100);
    fb.append("test", 4);
    assert(fb.size() == 4);
    assert(!fb.overflowed());
    std::cout << "  fixed_output_buffer_reserve_ensure_extra_noop ok\n";
}

static void test_fixed_output_buffer_overflowed_state() {
    char storage[4];
    FixedOutputBuffer fb(storage, 4);
    // Trigger overflow with a single large append
    fb.append("12345", 5); // > capacity
    assert(fb.overflowed());
    // size stays at 0 (overflow happened before any bytes were written)
    assert(fb.size() == 0);
    // Further ops are no-ops
    fb.push_back('x');
    assert(fb.size() == 0);
    std::cout << "  fixed_output_buffer_overflowed_state ok\n";
}

int main() {
    std::cout << "Running extended OutputBuffer tests...\n";

    test_output_buffer_arena_ctor();
    test_output_buffer_reset_with_arena();
    test_output_buffer_data_capacity();
    test_output_buffer_ensure_extra_and_unchecked();
    test_output_buffer_ensure_extra_zero();
    test_output_buffer_arena_grows_large();
    test_output_buffer_reserve_with_reserved_extra();
    test_output_buffer_grow_beyond_initial();

    test_fixed_output_buffer_basic();
    test_fixed_output_buffer_overflow_append();
    test_fixed_output_buffer_overflow_push_back();
    test_fixed_output_buffer_push_back_unchecked();
    test_fixed_output_buffer_clear();
    test_fixed_output_buffer_reserve_ensure_extra_noop();
    test_fixed_output_buffer_overflowed_state();

    std::cout << "\nAll extended OutputBuffer tests passed!\n";
    return 0;
}
