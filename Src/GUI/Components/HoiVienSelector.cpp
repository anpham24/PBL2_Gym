#include "HoiVienSelector.h"
#include <iostream>

HoiVienSelector::HoiVienSelector(sf::Font& font, const std::string& placeholder)
    : Selector<HoiVien>(font, placeholder)
{}

void HoiVienSelector::filterList() {
    filteredItems.clear();
    std::string searchTerm = searchBox.getString();

    // Nếu rỗng → Hiện tất cả
    if (searchTerm.empty()) {
        for (auto& item : allItems) {
            filteredItems.push_back(&item);
        }
        return;
    }
    
    std::cout << "\n🔍 CartPopup HoiVien Search: \"" << searchTerm << "\"" << std::endl;
    
    // ✅ 1. Nếu toàn số → Tìm theo ID (Prefix Match)
    if (StringUtils::isNumber(searchTerm)) {
        std::cout << "   → Searching by ID (Prefix)..." << std::endl;
        
        for (auto& item : allItems) {
            HoiVien* hv = item.data;
            
            // Kiểm tra ID bắt đầu bằng searchTerm
            if (hv->getID().substr(0, searchTerm.length()) == searchTerm) {
                filteredItems.push_back(&item);
            }
        }
        
        std::cout << "   ✅ Found " << filteredItems.size() << " results" << std::endl;
    } 
    // ✅ 2. Nếu chữ → Tìm theo TÊN (Trie)
    else {
        std::cout << "   → Searching by Name (Trie)..." << std::endl;
        
        // Kiểm tra input hợp lệ
        bool hasInvalidChar = false;
        for (size_t i = 0; i < searchTerm.length(); ++i) {
            char c = searchTerm[i];
            if (! ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                hasInvalidChar = true;
                break;
            }
        }
        
        if (hasInvalidChar) {
            std::cout << "   ⚠️  Contains invalid chars" << std::endl;
            return;
        }
        
        // Tìm bằng Trie
        MyVector<HoiVien*> results = SearchService::searchHoiVien(searchTerm);
        
        for (size_t i = 0; i < results.size(); ++i) {
            HoiVien* hv = results[i];
            
            // Tìm SelectorItem tương ứng
            for (auto& item : allItems) {
                if (item.data == hv) {
                    filteredItems. push_back(&item);
                    break;
                }
            }
        }
        
        std::cout << "   ✅ Found " << filteredItems.size() << " results" << std::endl;
    }
    
    // Kiểm tra currentSelection có còn hợp lệ không
    bool stillValid = false;
    if (currentSelection != nullptr) {
        for (auto* filtered : filteredItems) {
            if (currentSelection == filtered) {
                stillValid = true;
                break;
            }
        }
    }
    
    if (! stillValid) {
        setSelected(nullptr);
    }
}