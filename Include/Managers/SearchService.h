// SearchService.h
#pragma once
#include "MyVector.h"
#include <string>
#include <functional> // De dung std::function

namespace SearchService {

    /**
     * Tim kiem (Search)
     * Loc ra cac phan tu ma 'searchTerm' xuat hien trong chuoi tra ve tu 'extractor'
     */
    template <typename T>
    MyVector<T*> search(const MyVector<T*>& allItems, const std::string& searchTerm,
                        std::function<std::string(const T*)> extractor) {
        
        MyVector<T*> results;
        if (searchTerm.empty()) {
            // Tra ve toan bo neu khong tim kiem
            for(T* item : allItems) results.push_back(item);
            return results;
        }

        std::string lowerSearchTerm = searchTerm; // Chuyen ve chu thuong de tim kiem
        // ... (them code de convert `lowerSearchTerm` ve chu thuong)

        for (T* item : allItems) {
            std::string extractedValue = extractor(item);
            // ... (them code de convert `extractedValue` ve chu thuong)
            
            if (extractedValue.find(lowerSearchTerm) != std::string::npos) {
                results.push_back(item);
            }
        }
        return results;
    }

    /**
     * Loc (Filter)
     * Loc ra cac phan tu thoa man dieu kien 'predicate'
     */
    template <typename T>
    MyVector<T*> filter(const MyVector<T*>& items, 
                        std::function<bool(const T*)> predicate) {
        MyVector<T*> results;
        for (T* item : items) {
            if (predicate(item)) {
                results.push_back(item);
            }
        }
        return results;
    }

    /**
     * Sap xep (Sort)
     * Sap xep danh sach 'items' bang phep so sanh 'comparator'
     */
    template <typename T>
    void sort(MyVector<T*>& items, 
              std::function<bool(const T*, const T*)> comparator) {
        
        // Don gian la dung std::sort (ban can #include <algorithm>)
        // Hoac tu viet QuickSort/MergeSort cho MyVector
        // Vi du voi Bubble Sort don gian:
        for (size_t i = 0; i < items.size(); ++i) {
            for (size_t j = 0; j < items.size() - 1 - i; ++j) {
                if (!comparator(items[j], items[j+1])) {
                    // Swap
                    T* temp = items[j];
                    items[j] = items[j+1];
                    items[j+1] = temp;
                }
            }
        }
    }
}