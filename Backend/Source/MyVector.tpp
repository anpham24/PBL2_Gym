#include "../Include/MyVector.h"

template <typename T>
MyVector<T>::MyVector() : data(nullptr), size(0), capacity(0) {}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) {
    size = other.size;
    capacity = other.capacity;
    if (capacity > 0)
        data = new T[capacity];
    else 
        data = nullptr;
    for (size_t i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        size = other.size;
        capacity = other.capacity;
        if (capacity > 0)
            data = new T[capacity];
        else 
            data = nullptr;
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
void MyVector<T>::clear() {
    size = 0;
}

template <typename T>
void MyVector<T>::resize(size_t newCapacity) {
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;    
}

template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (size == capacity) {
        if (capacity = 0)
            resize(1);
        else 
            resize(capacity * 2);
    }
    data[size++] = value;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (size == 0)
        throw std::out_of_range("Vector is empty");
    --size;
}

template <typename T>
void MyVector<T>::erase(size_t index) {
    if (index >= size) 
        throw std::out_of_range("Index out of range");
    for (size_t i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    --size;
}

template <typename T> 
void MyVector<T>::erase(T* it) {
    if (it == nullptr) 
        throw std::out_of_range("Iterator is null");
    if (it < data || it > data + size) 
        throw std::out_of_range("Iterator is out of range");
    size_t index = static_cast<size_t>(it - data);
    erase(index);
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    return data[index];
} 

template <typename T>
T& MyVector<T>::at(size_t index) {
    if (index >= size)
        throw std::out_of_range("Index out of range");
    return data[index];
} 

template <typename T>
const T& MyVector<T>::at(size_t index) const {
    if (index >= size)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
size_t MyVector<T>::size() const {
    return size;
}

template <typename T>
bool MyVector<T>::empty() const {
    return (size == 0);
}

template <typename T>
T* MyVector<T>::begin() {
    return data;
}

template <typename T>
T* MyVector<T>::end() {
    return data + size;
}

template <typename T>
const T* MyVector<T>::begin() const {
    return data;
}

template <typename T>
const T* MyVector<T>::end() const {
    return data + size;
}