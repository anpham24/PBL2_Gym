#pragma once
#include "Trie.h"
#include "HoiVien.h"
#include "HLV.h"
#include "NhanVien.h"

class SearchEngine {
private:
    Trie<HoiVien> trieHoiVien;
    Trie<HLV> trieHLV;
    Trie<NhanVien> trieNhanVien;
public:
    void indexHoiVien(HoiVien* hv);
    void removeIndexHoiVien(HoiVien* hv);
    MyVector<HoiVien*> searchHoiVien(const string& namePrefix);

    void indexHLV(HLV* hlv);
    void removeIndexHLV(HLV* hlv);
    MyVector<HLV*> searchHLV(const string& namePrefix);

    void indexNhanVien(NhanVien* nv);
    void removeIndexNhanVien(NhanVien* nv);
    MyVector<NhanVien*> searchNhanVien(const string& namePrefix);    
};