#pragma once
#include "../Include/MyVector.h"
#include <iostream>
using namespace std;

template <typename T> 
class QL {
private:
    MyVector<T> ds;
public:
    QL();
    ~QL();

    void add(const T& obj);
    void remove(size_t index);
    T& get(size_t index);
    const T& get(size_t index) const;
    size_t size() const;
    void clear();

    void displayAll() const;
};

#include "../Source/Ql.cpp"