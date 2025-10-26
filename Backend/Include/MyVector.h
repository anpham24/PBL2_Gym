#pragma once
#include <cstddef>
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data;
    size_t size;
    size_t capacity;
    void resize(size_t newCapacity);
public:
    // Ham dung, ham huy, toan tu gan
    MyVector();
    MyVector(const MyVector& other);
    ~MyVector();
    MyVector& operator=(const MyVector& other);

    // Thao tac dung luong:
    void clear();

    // Them, xoa phan tu
    void push_back(const T&);
    void pop_back();
    void erase(size_t);
    void erase(T*);

    // Truy cap phan tu
    T& operator[](size_t);
    T& at(size_t);
    const T& at(size_t) const;

    // Ham thong tin
    size_t size() const;
    bool empty() const;

    // Ham iterator 
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;
};

#include "../Source/MyVector.tpp"