#include <iostream>
#include <initializer_list>
#include <type_traits>
#include <exception>


template <typename T, typename Alloc = std::allocator<T>>
class vector {
private:
    using AllocTraits = std::allocator_traits<Alloc>;
    T* m_arr;
    int m_size;
    int m_cap;
    Alloc m_alloc;

    // common iterator class
    template <bool b, typename U, typename V>
    using conditional_t = std::conditional<b, U, V>; // available since C++14

    template <bool is_const>
    class common_iterator {
    private:
        using ConditionalPtr = std::conditional_t<is_const, const T*, T*>;
        using ConditionalReference = std::conditional_t<is_const, const T&, T&>;

        ConditionalPtr ptr;

    public:
        common_iterator() {

        }
        common_iterator(ConditionalPtr ptr) : ptr(ptr) {
            
        }

        ConditionalReference operator*() const { return *ptr; }

        ConditionalPtr operator->() { return ptr; }

        common_iterator& operator+=(int n) {
            ptr += n;
            return *this;
        }
        common_iterator& operator-=(int n) {
            ptr -= n;
            return *this;
        }

        common_iterator operator+(int n) const {
            auto copy = *this;
            copy += n;
            return copy;
        }

        common_iterator operator-(int n) const {
            auto copy = *this;
            copy -= n;
            return copy;
        }

        common_iterator& operator++() {
            ++ptr;
            return *this;
        }

        common_iterator operator++(int) {
            common_iterator temp(*this);
            ++ptr;
            return temp;
        }

        common_iterator& operator--() {
            --ptr;
            return *this;
        }

        common_iterator operator--(int) {
            common_iterator temp(*this);
            --ptr;
            return temp;
        }

        int operator-(common_iterator it) const {
            return (ptr - it.ptr);
        }
        
        bool operator==(const common_iterator& other) const {
            return (ptr == other.ptr);
        }

        bool operator!=(const common_iterator& other) const {
            return (ptr != other.ptr);
        }

        bool operator>(const common_iterator& other) const {
            return (ptr > other.ptr);
        }

        bool operator<(const common_iterator& other) const {
            return (ptr < other.ptr);
        }

        ConditionalPtr get_pointer() const {
            return ptr;
        }
    };

    void Swap(vector& other) noexcept;

public:
    explicit vector(int size = 0);

    template <typename InputIterator>
    vector(InputIterator first, InputIterator last);
    
    vector(const std::initializer_list<T>& lis);
    vector(int size, const T& value);
    vector(const vector& other);
    vector(vector&& other) noexcept;
    vector& operator=(vector other);
    vector& operator=(vector&& other) noexcept;
    ~vector();

    
    // ================== Iterators ==================
    using iterator = common_iterator<true>;
    class const_iterator : public common_iterator<true> {
        public:
        const_iterator(const iterator& it) : common_iterator<true>(it.get_pointer()) { // should be implicitly constrcutable from iterator

        }
    };

    iterator begin() const {
        return iterator(m_arr);
    }
    iterator end() const {
        return iterator(m_arr+m_size);
    }

    const_iterator cbegin() const {
        return const_iterator(m_arr);
    }
    const_iterator cend() const {
        return const_iterator(m_arr+m_size);
    }
    
    
    // ================== Capacity ==================
    void resize(int new_size);
    void reserve(int new_cap);
    
    int size() const;
    int capacity() const;

    bool empty() const;

    void shrink_to_fit();


    // ================== Element access ==================
    const T& operator[](int index) const;
    T& operator[](int index);

    const T& at(int index) const;
    T& at(int index);

    const T& front() const;
    T& front();

    const T& back() const;
    T& back();
    

    // ================== Modifiers ==================
    void push_back(const T& value);
    void push_back(T&& value);

    void pop_back();
    
    void insert(const_iterator pos, const T& value);

    iterator erase(const_iterator pos);

    void clear();

    template <typename... Args>
    void emplace_back(Args&&... args); // universal reference

};


// ==================================== Implementations ====================================



template <typename T, typename Alloc>
vector<T, Alloc>::vector(int size) : m_arr(nullptr), m_size(size), m_cap(size), m_alloc() {
    m_arr = AllocTraits::allocate(m_alloc, m_cap);
    int i = 0;
    // strong exception guarantee
    // if an exception throwen during object
    // construction, nothing changes and no memory leaks.
    try {
        for(i; i < m_size; i++) {
            AllocTraits::construct(m_alloc, m_arr + i);
        }
    }
    catch(...) {
        for(int j = 0; j < i; j++) {
            AllocTraits::destroy(m_alloc, m_arr+j);
        }
        AllocTraits::deallocate(m_alloc, m_arr, m_cap);
        throw;
    } 
}


template <typename T, typename Alloc>
template <typename InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last) : m_alloc() {
    m_size = last - first;
    m_cap = m_size;
    m_arr = AllocTraits::allocate(m_alloc, m_cap);
    int i = 0;
    try {
        for(; first != last; ++first) {
            AllocTraits::construct(m_alloc, m_arr+i, *first);
            ++i;
        } 
    }
    catch(...) {
        for(int j = 0; j < i; j++) {
            AllocTraits::destroy(m_alloc, m_arr+j);
        }
        AllocTraits::deallocate(m_alloc, m_arr, m_cap);
        throw;
    }

}

template <typename T, typename Alloc>
vector<T, Alloc>::vector(const std::initializer_list<T>& lis) : vector(lis.begin(), lis.end()) {    

}

template <typename T, typename Alloc>
vector<T, Alloc>::vector(int size, const T& value) : m_arr(nullptr), m_size(size), m_cap(size), m_alloc() {
    m_arr = AllocTraits::allocate(m_alloc, m_cap);
    int i = 0;
    try {
        for(i; i < m_size; i++) {
            AllocTraits::construct(m_alloc, m_arr+i, value);
        }
    }
    catch(...) {
        for(int j = 0; j < i; j++) {
            AllocTraits::destroy(m_alloc, m_arr+j);
        }
        AllocTraits::deallocate(m_alloc, m_arr, m_cap);
        throw;
    }

}

// copy constructor
template <typename T, typename Alloc>
vector<T, Alloc>::vector(const vector& other) : m_arr(nullptr), m_size(other.m_size), m_cap(other.m_cap), m_alloc() {
    m_arr = AllocTraits::allocate(m_alloc, m_cap);
    int i = 0;
    try {
        for(; i < m_size; i++) {
            AllocTraits::construct(m_alloc, m_arr+i, other.m_arr[i]);
        }
    }
    catch(...) {
        for(; i >= 0; i++) {
            AllocTraits::destroy(m_alloc, m_arr+i);
        }
        AllocTraits::deallocate(m_alloc, m_arr, m_cap);
        throw;
    }
}

// move constructor 
template <typename T, typename Alloc>
vector<T, Alloc>::vector(vector&& other) noexcept : m_arr(other.m_arr), m_size(other.m_size), m_cap(other.m_cap), m_alloc() {
    other.m_arr = nullptr;
    other.m_size = 0;
    other.m_cap = 0;
}

// Swap
template <typename T, typename Alloc>
void vector<T, Alloc>::Swap(vector<T, Alloc>& other) noexcept {
    std::swap(m_arr, other.m_arr);
    std::swap(m_size, other.m_size);
    std::swap(m_cap, other.m_cap);
    std::swap(m_alloc, other.m_alloc);
}

// copy assignment operator 
template <typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc> other) {
    if(this == &other) return *this;
    Swap(other);
    return *this;
}

// move assignment operator 
template <typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc>&& other) noexcept {
    if(this == &other) return *this;
    Swap(other);
    return *this;
}


// destructor
template <typename T, typename Alloc>
vector<T, Alloc>::~vector() {
    for(int i = 0; i < m_size; i++) {
        AllocTraits::destroy(m_alloc, m_arr+i);
    }
    AllocTraits::deallocate(m_alloc, m_arr, m_cap);
}


// ==================================== Capacity ====================================

// size
template <typename T, typename Alloc>
int vector<T, Alloc>::size() const {
    return m_size;
}

// capacity
template <typename T, typename Alloc>
int vector<T, Alloc>::capacity() const {
    return m_cap;
}

// reserve
template <typename T, typename Alloc>
void vector<T, Alloc>::reserve(int new_cap) {
    if(m_cap >= new_cap) return;
    T* new_arr = AllocTraits::allocate(m_alloc, new_cap);
    // we use move_if_noexcept() instead of move(), becouse
    // the T's move constructor can throw an exception so we
    // can not provide strong exception guarantee, some data
    // can be changed. 
    int i = 0;
    try {
        for(i; i < m_size; i++) {
            AllocTraits::construct(m_alloc, new_arr + i, std::move_if_noexcept(m_arr[i]));
        }
    }
    catch(...) {
        for(int j = 0; j < i; j++) {
            AllocTraits::destroy(m_alloc, new_arr + j);
        }
        AllocTraits::deallocate(m_alloc, new_arr, new_cap);
        
        throw;
    }

    for(int i = 0; i < m_size; i++) {
        AllocTraits::destroy(m_alloc, m_arr+i);
    }
    AllocTraits::deallocate(m_alloc, m_arr, m_cap);
    m_arr = new_arr;
    m_cap = new_cap;
}

// resize
template <typename T, typename Alloc>
void vector<T, Alloc>::resize(int new_size) {
    if(new_size > m_cap) {
        reserve(new_size);
        m_cap = new_size;
    }
    
    for(int i = m_size; i < new_size; i++) {
        AllocTraits::construct(m_alloc, m_arr + i);
    }
    for(int i = new_size; i < m_size; i++) {
        AllocTraits::destroy(m_alloc, m_arr + i);
    }
    m_size = new_size;
}

// empty
template <typename T, typename Alloc>
bool vector<T, Alloc>::empty() const {
    return (m_size == 0);
}

// shrink_to_fit
template <typename T, typename Alloc>
void vector<T, Alloc>::shrink_to_fit() {
    if(m_size == m_cap) return;
    if(m_size == 0) {
        AllocTraits::deallocate(m_alloc, m_arr, m_cap);
        m_cap = 0;
        return;
    }
    T* new_arr = AllocTraits::allocate(m_alloc, m_size);
    
    for(int i = 0; i < m_size; i++) {
        AllocTraits::construct(m_alloc, new_arr+i, std::move(m_arr[i]));
    }
    for(int i = 0; i < m_size; i++) {
        AllocTraits::destroy(m_alloc, m_arr+i);
    }
    AllocTraits::deallocate(m_alloc, m_arr, m_cap);
    
    m_cap = m_size;
    m_arr = new_arr;
} 



// ==================================== Element access ====================================




// operator[] const
template <typename T, typename Alloc>
const T& vector<T, Alloc>::operator[](int index) const {
    return m_arr[index];
}

// operator[]
template <typename T, typename Alloc>
T& vector<T, Alloc>::operator[](int index) {
    return m_arr[index];
}

// at
template <typename T, typename Alloc>
T& vector<T, Alloc>::at(int index) {
    if(index >= m_size || index < 0) {
        throw std::out_of_range("out of range\n");
    }
    return m_arr[index];
}

// at const
template <typename T, typename Alloc>
const T& vector<T, Alloc>::at(int index) const {
    if(index >= m_size || index < 0) {
        throw std::out_of_range("out of range\n");
    }
    return m_arr[index];
} 

// front const
template <typename T, typename Alloc>
const T& vector<T, Alloc>::front() const {
    return m_arr[0];
}

// front 
template <typename T, typename Alloc>
T& vector<T, Alloc>::front() {
    return m_arr[0];
}

// back const 
template <typename T, typename Alloc>
const T& vector<T, Alloc>::back() const {
    return m_arr[m_size-1];
}

// back
template <typename T, typename Alloc>
T& vector<T, Alloc>::back() {
    return m_arr[m_size-1];
}




// ==================================== Modifiers ====================================

// push_back
template <typename T, typename Alloc>
void vector<T, Alloc>::push_back(const T& value) {
    if(m_cap == m_size) {
        reserve(2 * m_size);
    }
    AllocTraits::construct(m_alloc, m_arr+m_size, value);
    m_size++;
}


// push_back
// this overload is for rvalues, so we can move the value.
template <typename T, typename Alloc>
void vector<T, Alloc>::push_back(T&& value) {
    if(m_cap == m_size) {
        reserve(2 * m_size);    
    }
    AllocTraits::construct(m_alloc, m_arr+m_size, std::move(value));
    m_size++;
}


// pop_back
template <typename T, typename Alloc>
void vector<T, Alloc>::pop_back() {
    if(m_size == 0) return;
    AllocTraits::destroy(m_alloc, m_arr+m_size-1);
    m_size--;
}



// insert
template <typename T, typename Alloc>
void vector<T, Alloc>::insert(vector<T, Alloc>::const_iterator pos, const T& value) {
    int index = pos - cbegin();
    resize(m_size+1);
    int i = m_size-2;
    while(i >= index) {
        m_arr[i+1] = std::move(m_arr[i]); 
        i--;
    }
    m_arr[index] = value;
}


// erase
template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(vector<T, Alloc>::const_iterator pos) {
    if(pos == end()) return end();
    int index = pos - cbegin();
    for(int i = index; i < m_size-1; i++) {
        m_arr[i] = m_arr[i+1];
    }
    AllocTraits::destroy(m_alloc, m_arr+m_size-1);
    m_size--;
    return iterator(m_arr + index);
}



// clear
template <typename T, typename Alloc>
void vector<T, Alloc>::clear() {
    for(int i = 0; i < m_size; i++) {
        AllocTraits::destroy(m_alloc, m_arr+i);
    }
    m_size = 0;
}



// emplace_back
// in emplace_back as arguments we pass that values
// that we pass in constructor of T, so we can construct 
// the object directly in place (in method construct).
// we use std::forward for maintaining the value category 
// (rvalue, lvalue) of arguments that we pass in emplace_back().    
template <typename T, typename Alloc>
template <typename... Args>
void vector<T, Alloc>::emplace_back(Args&&... args) {
    if(m_cap == m_size) {
        reserve(2 * m_size);
    }
    AllocTraits::construct(m_alloc, m_arr+m_size, std::forward<Args>(args)...);
    m_size++;
}


