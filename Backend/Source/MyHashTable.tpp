#include "../Include/MyHashTable.h"

template <typename T>
MyHashTable<T>::MyHashTable(size_t capacity) : m_size(0), m_capacity(capacity) {
    table.reserve(capacity);
    for (size_t i = 0; i < capacity; i++) 
        table.push_back(nullptr);
}

template <typename T>
MyHashTable<T>::~MyHashTable() {
    for (size_t i = 0; i < m_capacity; i++) {
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
    unsigned long long hashValue = stoull(key);
    return (hashValue * 2654435761ULL) % m_capacity;
}

template <typename T>
void MyHashTable<T>::insert(const std::string& key, const T& value) {
    size_t index = hash_function(key);
    Node* newNode = new Node(key, value);
    
    if (table[index] == nullptr) {
        table[index] = newNode;
        m_size++;
        return;
    }

    Node* current = table[index];
    while (true) {
        if (current->key == key) {
            current->value = value; // update
            delete newNode;
            return;
        }
        if (current->next == nullptr) break;
        current = current->next;
    }

    current->next = newNode; // thêm node mới
    m_size++;
}

template <typename T>
bool MyHashTable<T>::del(const std::string& key) {
    size_t index = hash_function(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev) prev->next = current->next;
            else table[index] = current->next; // xóa node đầu
            delete current;
            m_size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false; // không tìm thấy
}

template <typename T>
T* MyHashTable<T>::search(const std::string& key) {
    size_t index = hash_function(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) return &(current->value);
        current = current->next;
    }
    return nullptr; // không tìm thấy
}

template <typename T>
size_t MyHashTable<T>::size() const {
    return m_size;
}

template <typename T>
bool MyHashTable<T>::empty() const {
    return (m_size == 0);
}

template <typename T>
MyVector<T> MyHashTable<T>::getAllValues() const {
    MyVector<T> result;
    for (size_t i = 0; i < m_capacity; ++i) {
        Node* current = table[i];
        while (current) {
            result.push_back(current->value);
            current = current->next;
        }
    }
    return result;
}
