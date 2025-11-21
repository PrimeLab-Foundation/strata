#include "dummy.hpp"

#include <iostream>
#include <string>

int main() {
    // Test add
    assert(add(1, 2) == 3);
    assert(add(-5, 5) == 0);

    // Test hello
    std::string name = "Alice";
    std::string expected = "Hello, Alice!";
    assert(hello(name) == expected);

    std::cout << "All dummy tests passed.\n";
    return 0;
}
