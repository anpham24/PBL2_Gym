#include "../Include/QL.h"

// --- Constructor ---
template <typename T>
QL<T>::QL() {}

// --- Destructor ---
template <typename T>
QL<T>::~QL() {}

// --- Thêm đối tượng ---
template <typename T>
void QL<T>::add(const T& obj) {
    ds.push_back(obj);
}

// --- Xoá đối tượng theo chỉ số ---
template <typename T>
void QL<T>::remove(size_t index) {
    if (index >= ds.size()) {
        cout << "⚠️ Index không hợp lệ!\n";
        return;
    }
    for (size_t i = index; i + 1 < ds.size(); ++i)
        ds[i] = ds[i + 1];
    ds.pop_back();
}

// --- Lấy đối tượng ---
template <typename T>
T& QL<T>::get(size_t index) {
    return ds[index];
}

template <typename T>
const T& QL<T>::get(size_t index) const {
    return ds[index];
}

// --- Số lượng đối tượng ---
template <typename T>
size_t QL<T>::size() const {
    return ds.size();
}

// --- Xoá toàn bộ danh sách ---
template <typename T>
void QL<T>::clear() {
    ds.clear();
}

// --- Xuất toàn bộ danh sách ---
template <typename T>
void QL<T>::displayAll() const {
    for (size_t i = 0; i < ds.size(); ++i) {
        cout << "[" << i << "] " << ds[i].read() << endl;
    }
}
