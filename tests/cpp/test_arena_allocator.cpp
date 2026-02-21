// Tests for Arena, ArenaAllocator, and ArenaStringBuilder
// Brings arena_allocator.hpp from 0% to ~100% line coverage.

#include "strata/util/arena_allocator.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace strata::util;

// ---------------------------------------------------------------------------
// Arena tests
// ---------------------------------------------------------------------------

static void test_arena_basic_alloc() {
    Arena arena;
    void* p = arena.allocate(64);
    assert(p != nullptr);
    // Write and read back to confirm it's usable memory
    std::memset(p, 0xAB, 64);
    assert(static_cast<unsigned char*>(p)[0] == 0xAB);
    std::cout << "  arena_basic_alloc ok\n";
}

static void test_arena_alignment() {
    Arena arena;
    // Allocate 1 byte to misalign pos, then allocate with 8-byte alignment
    void* p1 = arena.allocate(1, 1);
    assert(p1 != nullptr);
    void* p2 = arena.allocate(8, 8);
    assert(p2 != nullptr);
    assert(reinterpret_cast<uintptr_t>(p2) % 8 == 0);
    std::cout << "  arena_alignment ok\n";
}

static void test_arena_overflow_new_block() {
    // Use a tiny initial size to force overflow on the first allocation
    Arena arena(32);
    // First allocation fits
    void* p1 = arena.allocate(16, 1);
    assert(p1 != nullptr);
    // This allocation won't fit in 32 bytes — forces a new block
    void* p2 = arena.allocate(64, 1);
    assert(p2 != nullptr);
    std::memset(p2, 0x55, 64);
    assert(static_cast<unsigned char*>(p2)[63] == 0x55);
    std::cout << "  arena_overflow_new_block ok\n";
}

static void test_arena_total_size() {
    Arena arena(64);
    size_t t0 = arena.total_size();
    assert(t0 == 64);
    // Force a second block
    arena.allocate(128, 1);
    size_t t1 = arena.total_size();
    assert(t1 > 64);
    std::cout << "  arena_total_size ok\n";
}

static void test_arena_reset_single_block() {
    Arena arena(256);
    arena.allocate(100, 1);
    arena.allocate(100, 1);
    // reset() when there is only one block
    arena.reset();
    // Should be reusable after reset
    void* p = arena.allocate(200, 1);
    assert(p != nullptr);
    std::cout << "  arena_reset_single_block ok\n";
}

static void test_arena_reset_multi_block() {
    Arena arena(32);
    // Force multiple blocks
    for (int i = 0; i < 10; ++i) {
        arena.allocate(64, 1);
    }
    size_t before = arena.total_size();
    assert(before > 32);
    // reset() must shrink back to first block only
    arena.reset();
    size_t after = arena.total_size();
    assert(after <= before);
    // Still usable
    void* p = arena.allocate(16, 1);
    assert(p != nullptr);
    std::cout << "  arena_reset_multi_block ok\n";
}

static void test_arena_large_single_allocation() {
    Arena arena(64);
    // Single allocation larger than initial block
    void* p = arena.allocate(1024 * 1024, 1);
    assert(p != nullptr);
    std::memset(p, 0, 1024 * 1024); // write to whole region
    std::cout << "  arena_large_single_allocation ok\n";
}

// ---------------------------------------------------------------------------
// ArenaAllocator<T> tests
// ---------------------------------------------------------------------------

static void test_arena_allocator_basic() {
    Arena arena;
    ArenaAllocator<int> alloc(&arena);
    int* p = alloc.allocate(10);
    assert(p != nullptr);
    for (int i = 0; i < 10; ++i) p[i] = i;
    for (int i = 0; i < 10; ++i) assert(p[i] == i);
    // deallocate is a no-op — must not crash
    alloc.deallocate(p, 10);
    std::cout << "  arena_allocator_basic ok\n";
}

static void test_arena_allocator_rebind() {
    Arena arena;
    ArenaAllocator<int> alloc_int(&arena);
    // Rebind to char
    ArenaAllocator<int>::rebind<char>::other alloc_char(alloc_int);
    char* p = alloc_char.allocate(100);
    assert(p != nullptr);
    std::memset(p, 'X', 100);
    assert(p[50] == 'X');
    std::cout << "  arena_allocator_rebind ok\n";
}

static void test_arena_allocator_equality() {
    Arena a1, a2;
    ArenaAllocator<int> al1(&a1);
    ArenaAllocator<int> al2(&a1); // same arena
    ArenaAllocator<int> al3(&a2); // different arena
    assert(al1 == al2);
    assert(al1 != al3);
    assert(!(al1 != al2));
    assert(!(al1 == al3));
    std::cout << "  arena_allocator_equality ok\n";
}

static void test_arena_allocator_copy_ctor() {
    Arena arena;
    ArenaAllocator<double> a1(&arena);
    ArenaAllocator<double> a2(a1); // copy ctor
    assert(a2.arena() == a1.arena());
    double* p = a2.allocate(4);
    assert(p != nullptr);
    std::cout << "  arena_allocator_copy_ctor ok\n";
}

static void test_arena_allocator_null_throws() {
    ArenaAllocator<int> alloc(nullptr);
    bool threw = false;
    try {
        alloc.allocate(1);
    } catch (const std::bad_alloc&) {
        threw = true;
    }
    assert(threw);
    std::cout << "  arena_allocator_null_throws ok\n";
}

static void test_arena_allocator_with_vector() {
    Arena arena;
    ArenaAllocator<int> alloc(&arena);
    std::vector<int, ArenaAllocator<int>> v(alloc);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    assert(v.size() == 3);
    assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
    std::cout << "  arena_allocator_with_vector ok\n";
}

// ---------------------------------------------------------------------------
// ArenaStringBuilder tests
// ---------------------------------------------------------------------------

static void test_arena_string_builder_basic() {
    Arena arena;
    ArenaStringBuilder sb(arena);
    sb.push_back('H');
    sb.push_back('i');
    assert(sb.size() == 2);
    assert(sb.view() == "Hi");
    assert(sb.to_string() == "Hi");
    std::cout << "  arena_string_builder_basic ok\n";
}

static void test_arena_string_builder_append_str() {
    Arena arena;
    ArenaStringBuilder sb(arena, 8);
    sb.append("Hello", 5);
    sb.append(", ");  // uses strlen overload
    sb.append("world!", 6);
    assert(sb.view() == "Hello, world!");
    assert(sb.size() == 13);
    std::cout << "  arena_string_builder_append_str ok\n";
}

static void test_arena_string_builder_growth() {
    Arena arena;
    // Start small to force growth
    ArenaStringBuilder sb(arena, 4);
    std::string expected;
    for (int i = 0; i < 200; ++i) {
        sb.push_back('A' + (i % 26));
        expected += char('A' + (i % 26));
    }
    assert(sb.view() == expected);
    std::cout << "  arena_string_builder_growth ok\n";
}

static void test_arena_string_builder_reserve() {
    Arena arena;
    ArenaStringBuilder sb(arena, 8);
    // Reserve larger than current capacity
    sb.reserve(512);
    sb.append("test", 4);
    assert(sb.view() == "test");
    // Reserve smaller than current capacity (no-op)
    sb.reserve(4);
    assert(sb.view() == "test");
    std::cout << "  arena_string_builder_reserve ok\n";
}

static void test_arena_string_builder_data() {
    Arena arena;
    ArenaStringBuilder sb(arena, 16);
    sb.append("abc", 3);
    assert(sb.data() != nullptr);
    assert(std::strncmp(sb.data(), "abc", 3) == 0);
    std::cout << "  arena_string_builder_data ok\n";
}

int main() {
    std::cout << "Running Arena allocator tests...\n";

    test_arena_basic_alloc();
    test_arena_alignment();
    test_arena_overflow_new_block();
    test_arena_total_size();
    test_arena_reset_single_block();
    test_arena_reset_multi_block();
    test_arena_large_single_allocation();

    test_arena_allocator_basic();
    test_arena_allocator_rebind();
    test_arena_allocator_equality();
    test_arena_allocator_copy_ctor();
    test_arena_allocator_null_throws();
    test_arena_allocator_with_vector();

    test_arena_string_builder_basic();
    test_arena_string_builder_append_str();
    test_arena_string_builder_growth();
    test_arena_string_builder_reserve();
    test_arena_string_builder_data();

    std::cout << "\nAll Arena allocator tests passed!\n";
    return 0;
}
