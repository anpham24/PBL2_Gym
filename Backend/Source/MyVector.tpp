#include "../Include/MyVector.h"

template <typename T>
MyVector<T>::MyVector() : data(nullptr), capacity(0), length(0) {}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) {
    capacity = other.capacity;
    length = other.length;
    data = new T[capacity];
    for (size_t i = 0; i < length; ++i)
        data[i] = other.data[i];
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (size_t i = 0; i < length; ++i)
            data[i] = other.data[i];
    }
    return *this;
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}

template <typename T>
void MyVector<T>::resize(size_t newCap) {
    T* newData = new T[newCap];
    for (size_t i = 0; i < length; ++i)
        newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCap;
}

template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (length == capacity)
        resize(capacity == 0 ? 1 : capacity * 2);
    data[length++] = value;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (length == 0)
        throw std::out_of_range("Vector is empty");
    --length;
}

template <typename T>
T& MyVector<T>::at(size_t index) {
    if (index >= length)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
const T& MyVector<T>::at(size_t index) const {
    if (index >= length)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
size_t MyVector<T>::size() const { return length; }

template <typename T>
bool MyVector<T>::empty() const { return length == 0; }

template <typename T>
void MyVector<T>::clear() { length = 0; }

template <typename T>
T& MyVector<T>::operator[](size_t index) { return data[index]; }

template <typename T>
const T& MyVector<T>::operator[](size_t index) const { return data[index]; }
