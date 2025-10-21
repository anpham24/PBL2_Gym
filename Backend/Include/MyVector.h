#pragma once
#include <cstddef>

template <typename T>
class MyVector {
    private:
        T* data;
        size_t size;
        size_t capacity;

        void resize (size_t);
    public:
        MyVector();
        MyVector(size_t);
        MyVector(const MyVector&);
        ~MyVector();

        MyVector& operator=(const MyVector&);

        void push_back(const T&);
        void pop_back();
        T& operator[](size_t);
        const T& operator[](size_t) const;

        size_t size() const;
        size_t capacity() const;
        bool empty() const;
        void clear();
};