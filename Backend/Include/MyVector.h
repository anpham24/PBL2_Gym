#pragma once
#include <cstddef>
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data;
    size_t m_size;
    size_t m_capacity;

    // Ham thay doi capacity
    void reserve(size_t);
    // Ham thay doi size
    void resize(size_t);

public:
    // Ham dung, ham huy, toan tu gan
    MyVector();
    MyVector(const MyVector&);
    ~MyVector();
    MyVector& operator=(const MyVector&);

    // Thao tac dung luong:
    void clear();

    // Them, xoa phan tu
    void push_back(const T&);
    void pop_back();
    bool erase(size_t);
    bool erase(T*);

    // Truy cap phan tu
    T& operator[](size_t);
    const T& operator[](size_t) const;
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