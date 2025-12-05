#pragma once

#include <string>

#include "SearchEngine.h"
#include "HoiVien.h"
#include "MyVector.h"

using namespace std;

class SearchService {
private:
    static SearchEngine searchEngine; // ✅ CHỈ dùng cho HoiVien
    
public:
    // ✅ Index HoiVien (gọi 1 lần sau khi load file)
    static void indexAllHoiVien();
    
    // ✅ Index/Remove từng HoiVien (khi thêm/xóa)
    static void indexHoiVien(HoiVien* hv);
    static void removeIndexHoiVien(HoiVien* hv);
    
    // ✅ Tìm kiếm HoiVien
    static MyVector<HoiVien*> searchHoiVien(const string& namePrefix);
};