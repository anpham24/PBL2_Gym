#pragma once
#include "MyVector.h"
#include <string>
#include <cstddef>
#include <stdexcept>

using namespace std;

template <typename T>
class MyHashTable {
private:
    struct Node {
        string key;
        T value;
        Node *next;
        Node(const string& k, const T& v) : key(k), value(v), next(nullptr) {}
    };

    MyVector<Node*> table;
    size_t m_size;
    size_t m_capacity;
    
    size_t hash_function(const string&) const;

public: 
    // Ham dung ham huy
    MyHashTable();
    MyHashTable(size_t);
    ~MyHashTable();

    // Ham them, xoa, tim kiem
    void insert(const string&, const T&);
    bool remove(const string&);
    T* search(const string&);

    // Ham thong tin
    size_t size() const;
    bool empty() const;

    MyVector<T> getAllValues() const;
};

#include "../Source/MyHashTable.tpp"