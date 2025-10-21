#include "../Include/MyVector.h"

template <typename T>
MyVector<T>::MyVector() : data(nullptr), size(0), capacity(0) {

}

template <typename T>
MyVector<T>::MyVector(size_t initialCapacity)
    : data(new T[initialCapacity]), size(0), capacity(initialCapacity) {

}