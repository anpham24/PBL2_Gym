#include "MyVector.h"

template <typename T>
MyVector<T>::MyVector() : data(nullptr), m_size(0), m_capacity(0) {}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) {
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    if (m_capacity > 0)
        data = new T[m_capacity];
    else 
        data = nullptr;
    for (size_t i = 0; i < m_size; i++) {
        data[i] = other.data[i];
    }
}

template <typename T>
MyVector<T>::~MyVector() {
    clear();
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        // Giải phóng bộ nhớ cũ
        delete[] data;
        
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        if (m_capacity > 0)
            data = new T[m_capacity];
        else 
            data = nullptr;
        for (size_t i = 0; i < m_size; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
void MyVector<T>::clear() {
    m_size = 0;
}

template <typename T>
void MyVector<T>::reserve(size_t newCapacity) {
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < m_size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    m_capacity = newCapacity;    
}

template <typename T>
void MyVector<T>::resize(size_t newSize) {
    if (newSize > m_size) {
        if (newSize > m_capacity) {
            reserve(newSize);
        }
        for (size_t i = m_size; i < newSize; i++) {
            data[i] = T();
        }
    }
    m_size = newSize;
}


template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (m_size == m_capacity) {
        if (m_capacity == 0)
            reserve(1);
        else
            reserve(m_capacity * 2);
    }
    data[m_size++] = value;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (m_size == 0)
        throw std::out_of_range("Vector is empty");
    --m_size;
}

template <typename T>
bool MyVector<T>::erase(size_t index) {
    if (index >= m_size) 
        return false;

    for (size_t i = index; i < m_size - 1; i++) {
        data[i] = data[i + 1];
    }
    --m_size;
    return true;
}

template <typename T> 
bool MyVector<T>::erase(T* it) {
    if (it == nullptr) 
        return false;
    if (it < data || it > data + m_size) 
        return false;
    size_t index = static_cast<size_t>(it - data);
    return erase(index);
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    return data[index];
} 

template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    return data[index];
}

template <typename T>
T& MyVector<T>::at(size_t index) {
    if (index >= m_size)
        throw std::out_of_range("Index out of range");
    return data[index];
} 

template <typename T>
const T& MyVector<T>::at(size_t index) const {
    if (index >= m_size)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
size_t MyVector<T>::size() const {
    return m_size;
}

template <typename T>
bool MyVector<T>::empty() const {
    return (m_size == 0);
}

template <typename T>
T* MyVector<T>::begin() {
    return data;
}

template <typename T>
T* MyVector<T>::end() {
    return data + m_size;
}

template <typename T>
const T* MyVector<T>::begin() const {
    return data;
}

template <typename T>
const T* MyVector<T>::end() const {
    return data + m_size;
}