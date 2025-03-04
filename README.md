# Vector with Allocator and Iterator Implementation

This is a custom implementation of a dynamic array-like container (vector) in C++ that supports memory allocation via an allocator and uses custom iterators to iterate over the elements.

```cpp
template <typename T, typename Alloc = std::allocator<T>>
```

## Features

- **Custom Allocator Support**: The implementation supports using custom memory allocators via `std::allocator`.
- **Custom Iterators**: A bidirectional iterator is implemented to traverse the vector.
- **Memory Management**: The vector dynamically manages memory, resizing and reallocating as needed.
- **Strong Exception Safety**: Guarantees that memory is properly managed even when an exception occurs during construction or resizing.

## Supported Operations

### 1. Constructor

- `vector(int size = 0)`  
- `vector(int size, const T& value)`  
- `vector(const std::initializer_list<T>& lis)`  
- `vector(InputIterator first, InputIterator last)`
- Copy constructor: `vector(const vector& other)`
- Move constructor: `vector(vector&& other) noexcept`

### 2. Assignment

- Copy assignment: `vector& operator=(vector other)`
- Move assignment: `vector& operator=(vector&& other) noexcept`

### 3. Destructor

- `~vector()`  

### 4. Capacity

- `int size() const`  
- `int capacity() const`  
- `void reserve(int new_cap)`  
- `void resize(int new_size)`  
- `bool empty() const`  
- `void shrink_to_fit()`  

### 5. Element Access

- `T& operator[](int index)`  
- `const T& operator[](int index) const`  
- `T& at(int index)`  
- `const T& at(int index) const`  
- `T& front()`  
- `const T& front() const`  
- `T& back()`  
- `const T& back() const`  

### 6. Modifiers

- `void push_back(const T& value)`  
- `void push_back(T&& value)`  
- `void pop_back()`  
- `void insert(const_iterator pos, const T& value)`  
- `void clear()`  
- `template <typename... Args> void emplace_back(Args&&... args)`  

### 7. Iterators

- `iterator begin() const`  
- `iterator end() const`  
- `const_iterator cbegin() const`  
- `const_iterator cend() const`  

## Building and Running Tests

To build the project and run tests, follow these steps:

```txt
mkdir build_for_test

cd build_for_test

cmake ..

make
```

## Example Usage

```cpp
#include <iostream>
#include "vector.cpp"

int main() {
    vector<int> v;
    
    // Push elements
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    // Access elements
    std::cout << "First element: " << v.front() << std::endl;
    std::cout << "Last element: " << v.back() << std::endl;
    
    // Iterate through elements
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Insert element
    v.insert(v.begin() + 1, 15);
    
    // Print after modifications
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
