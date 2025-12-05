#include "SearchService.h"
#include "QuanLy.h"
#include <iostream>

SearchEngine SearchService::searchEngine;

// ✅ CHỈ index HoiVien
void SearchService::indexAllHoiVien() {
    std::cout << "\n🔍 Indexing HoiVien for search..." << std::endl;
    
    QuanLy& ql = QuanLy::getInstance();
    
    MyVector<HoiVien*> allHV = ql.getDsHoiVien(). getAllValues();
    for (size_t i = 0; i < allHV.size(); ++i) {
        searchEngine.indexHoiVien(allHV[i]);
    }
    
    std::cout << "   ✅ Indexed " << allHV.size() << " HoiVien" << std::endl;
}

void SearchService::indexHoiVien(HoiVien* hv) {
    if (hv != nullptr) {
        searchEngine.indexHoiVien(hv);
    }
}

void SearchService::removeIndexHoiVien(HoiVien* hv) {
    if (hv != nullptr) {
        searchEngine.removeIndexHoiVien(hv);
    }
}

MyVector<HoiVien*> SearchService::searchHoiVien(const string& namePrefix) {
    return searchEngine.searchHoiVien(namePrefix);
}