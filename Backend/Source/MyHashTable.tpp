#include "../Include/MyHashTable.h"

template <typename T>
MyHashTable<T>::MyHashTable(size_t capacity) : size(0), capacity(capacity) {
    table.reserve(capacity);
    for (size_t i = 0; i < capacity; i++) 
        table.push_back(nullptr);
}

template <typename T>
MyHashTable<T>::~MyHashTable() {
    for (size_t i = 0; i < capacity; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            delete temp;
            current = current->next;
        }
        table[i] = nullptr;
    }
}

template <typename T>
size_t MyHashTable<T>::hash_function(const string& key) const {
    key 
}