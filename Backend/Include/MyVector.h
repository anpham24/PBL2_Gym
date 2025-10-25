#pragma once
#include <stdexcept>
#include <cstddef> 
#include <iostream>

template <typename T>
class MyVector {
private:
    T* data;
    size_t capacity;
    size_t length;

    void resize(size_t newCap);

public:
    MyVector();
    MyVector(const MyVector& other);
    MyVector& operator=(const MyVector& other);
    ~MyVector();

    void push_back(const T& value);
    void pop_back();
    T& at(size_t index);
    const T& at(size_t index) const;
    size_t size() const;
    bool empty() const;
    void clear();
    void erase(size_t index);
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    void erase(T* it);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
};

#include "../Source/MyVector.tpp"
