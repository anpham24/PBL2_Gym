#pragma once
#include "MyVector.h" 
#include <string>

using namespace std;

// Kích thước bảng chữ cái: 26 chữ cái (a-z) + 1 dấu cách (index 26)
const int ALPHABET_SIZE = 27;

// --- CẤU TRÚC NODE ---
template <typename T>
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    
    // Lưu danh sách con trỏ tới đối tượng thật
    MyVector<T*> dataList; 

    TrieNode() {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
    
    // Không cần destructor ở đây vì Trie sẽ lo việc dọn dẹp
};

// --- LỚP QUẢN LÝ TRIE ---
template <typename T>
class Trie {
private:
    TrieNode<T>* root;

    // Hàm chuyển ký tự sang index 
    int charToIndex(char c);

    // Hàm chuẩn hóa chuỗi: Chỉ giữ lại a-z và space
    string normalize(const string& input);

    // Hàm đệ quy xóa tất cả node con
    void clear(TrieNode<T>* node);

    // Hàm đệ quy thu thập dữ liệu
    void collectAll(TrieNode<T>* node, MyVector<T*>& result);

public:
    Trie();
    ~Trie();

    // Cấm copy Trie để tránh lỗi double-free (Crash chương trình)
    // Vì Trie quản lý con trỏ động, copy nông (shallow copy) là thảm họa.
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    // Thêm đối tượng
    void insert(const string& name, T* objectPtr);

    // Xóa đối tượng
    void remove(const string& name, T* objectPtr);

    // Tìm kiếm
    MyVector<T*> searchPrefix(const string& prefix);
};

#include "../../Src/Utils/Trie.tpp"