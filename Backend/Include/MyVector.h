#pragma once
#include <stdexcept>
#include <cstddef> // size_t
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

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
};

#include "../Source/MyVector.tpp"
