#include "MyHashTable.h"

template <typename T>
MyHashTable<T>::MyHashTable(size_t capacity) : m_size(0), m_capacity(capacity) {
    table.reserve(capacity);
    for (size_t i = 0; i < capacity; i++) 
        table.push_back(nullptr);
}

template <typename T>
MyHashTable<T>::~MyHashTable() {
    for (size_t i = 0; i < table.size(); i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
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
void MyHashTable<T>::insert(const string& key, const T& value) {
    size_t index = hash_function(key);
    
    // Kiểm tra tồn tại trước để update
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return; // Update xong thì return ngay, không cần tạo node mới
        }
        current = current->next;
    }

    // Nếu không tìm thấy thì mới tạo Node mới
    Node* newNode = new Node(key, value);
    newNode->next = table[index]; // Chèn vào đầu danh sách (nhanh hơn chèn cuối)
    table[index] = newNode;
    m_size++;
}

template <typename T>
bool MyHashTable<T>::del(const string& key) {
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
const T* MyHashTable<T>::search(const std::string& key) const {
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
