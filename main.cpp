#include <iostream>
#include <cassert>
#include "vector.cpp"  // Include your vector implementation

int main() {
    // Constructor tests
    vector<int> v1(5);
    assert(v1.size() == 5);
    assert(v1.capacity() == 5);

    vector<int> v2 = {1, 2, 3};
    assert(v2.size() == 3);
    assert(v2[0] == 1 && v2[1] == 2 && v2[2] == 3);

    vector<int> v3(v2.begin(), v2.end());
    assert(v3.size() == 3);
    assert(v3[2] == 3);

    vector<int> v4(4, 10);
    assert(v4.size() == 4);
    assert(v4.front() == 10 && v4.back() == 10);

    // Push and Pop
    v2.push_back(4);
    assert(v2.size() == 4);
    assert(v2.back() == 4);

    v2.pop_back();
    assert(v2.size() == 3);
    assert(v2.back() == 3);

    // Resize and Reserve
    v1.resize(10);
    assert(v1.size() == 10);

    v1.reserve(20);
    assert(v1.capacity() == 20);

    v1.shrink_to_fit();
    assert(v1.capacity() == 10);

    // Element access
    v2[1] = 99;
    assert(v2.at(1) == 99);
    
    try {
        v2.at(10);  // Should throw
    } catch (const std::out_of_range&) {
        std::cout << "Out of range exception caught as expected.\n";
    }

    // Iterators
    auto it = v2.begin();
    assert(*it == 1);
    it++;
    assert(*it == 99);

    // Insert and Erase
    v2.insert(v2.begin() + 1, 42);
    assert(v2[1] == 42);

    v2.erase(v2.begin() + 1);
    assert(v2[1] == 99);

    // Emplace
    v2.emplace_back(55);
    assert(v2.back() == 55);

    // Clear and Empty
    v2.clear();
    assert(v2.empty());
    assert(v2.size() == 0);

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
