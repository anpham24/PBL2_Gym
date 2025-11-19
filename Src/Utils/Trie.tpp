#include "Trie.h"
#include <cctype> // Để dùng tolower()

// --- Constructor ---
template <typename T>
Trie<T>::Trie() {
    root = new TrieNode<T>();
}

// --- Destructor ---
template <typename T>
Trie<T>::~Trie() {
    clear(root);
    // Không cần delete root ở đây vì clear(root) đã delete chính nó rồi
    root = nullptr; 
}

// --- Hàm hủy đệ quy (Chống Memory Leak) ---
template <typename T>
void Trie<T>::clear(TrieNode<T>* node) {
    if (node == nullptr) return;

    // 1. Xóa tất cả con trước (Post-order traversal)
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            clear(node->children[i]);
            node->children[i] = nullptr;
        }
    }

    // 2. Sau khi con đã chết hết, mới xóa cha
    delete node;
}

// --- Hàm Map ký tự sang Index (Chống Crash do tràn mảng) ---
template <typename T>
int Trie<T>::charToIndex(char c) {
    if (c == ' ') return 26;
    if (c >= 'a' && c <= 'z') return c - 'a';
    
    // Trả về -1 cho mọi ký tự lạ (số, ký tự đặc biệt, dấu tiếng Việt chưa lọc)
    return -1; 
}

// --- Hàm chuẩn hóa chuỗi ---
template <typename T>
string Trie<T>::normalize(const string& input) {
    string res = "";
    for (int i = 0; i < input.length(); i++) {
        char c = input[i];
        
        // Chuyển thường
        char lowerC = tolower(c);
        
        // Chỉ chấp nhận a-z và dấu cách
        // Ký tự lạ sẽ bị loại bỏ khỏi chuỗi tìm kiếm
        if ((lowerC >= 'a' && lowerC <= 'z') || lowerC == ' ') {
            res += lowerC;
        }
    }
    return res;
}

// --- Insert ---
template <typename T>
void Trie<T>::insert(const string& name, T* objectPtr) {
    if (name.empty()) return;

    string cleanName = normalize(name);
    TrieNode<T>* pCrawl = root;

    for (int i = 0; i < cleanName.length(); i++) {
        int index = charToIndex(cleanName[i]);

        // Bảo vệ crash: Nếu index = -1 (lỗi), bỏ qua ký tự đó
        if (index == -1) continue;

        if (pCrawl->children[index] == nullptr) {
            pCrawl->children[index] = new TrieNode<T>();
        }
        pCrawl = pCrawl->children[index];
    }

    // Thêm con trỏ vào node cuối
    pCrawl->dataList.push_back(objectPtr);
}

// --- Remove ---
template <typename T>
void Trie<T>::remove(const string& name, T* objectPtr) {
    if (name.empty()) return;

    string cleanName = normalize(name);
    TrieNode<T>* pCrawl = root;

    for (int i = 0; i < cleanName.length(); i++) {
        int index = charToIndex(cleanName[i]);

        // Bảo vệ crash: Nếu index = -1 (lỗi), bỏ qua ký tự đó
        if (index == -1) continue;

        // Nếu đường cụt -> Không tìm thấy
        if (pCrawl->children[index] == nullptr) {
            return;
        }
        pCrawl = pCrawl->children[index];
    }

    // Tìm và xóa con trỏ khỏi dataList tại node cuối
    pCrawl->dataList.eraseByValue(objectPtr);
}

// --- Search ---
template <typename T>
MyVector<T*> Trie<T>::searchPrefix(const string& prefix) {
    MyVector<T*> result;
    if (prefix.empty()) return result;

    string cleanPrefix = normalize(prefix);
    TrieNode<T>* pCrawl = root;

    // 1. Đi theo prefix
    for (int i = 0; i < cleanPrefix.length(); i++) {
        int index = charToIndex(cleanPrefix[i]);
        
        if (index == -1) continue;

        // Nếu đường cụt -> Không tìm thấy
        if (pCrawl->children[index] == nullptr) {
            return result;
        }
        pCrawl = pCrawl->children[index];
    }

    // 2. Thu thập toàn bộ dữ liệu con cháu
    collectAll(pCrawl, result);
    
    return result;
}

// --- Collect Data (DFS) ---
template <typename T>
void Trie<T>::collectAll(TrieNode<T>* node, MyVector<T*>& result) {
    if (node == nullptr) return;

    // Lấy dữ liệu tại node hiện tại
    for (int i = 0; i < node->dataList.size(); i++) {
        result.push_back(node->dataList[i]);
    }

    // Đệ quy xuống các con
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            collectAll(node->children[i], result);
        }
    }
}