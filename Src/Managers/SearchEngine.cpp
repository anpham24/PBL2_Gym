#include "SearchEngine.h"

void SearchEngine::indexHoiVien(HoiVien* hv) {
    if (hv != nullptr) {
        trieHoiVien.insert(hv->getHoTen(), hv);
    }
}

void SearchEngine::removeIndexHoiVien(HoiVien* hv) {
    if (hv != nullptr) {
        trieHoiVien.remove(hv->getHoTen(), hv);
    }
}

MyVector<HoiVien*> SearchEngine::searchHoiVien(const string& namePrefix) {
    MyVector<HoiVien*> rawResult = trieHoiVien.searchPrefix(namePrefix);
    MyVector<HoiVien*> finalResult;

    for (size_t i = 0; i < rawResult.size(); ++i) {
        HoiVien* hv = rawResult[i];
        if (hv != nullptr && hv->getIsActive()) {
            finalResult.push_back(hv);
        }
    }

    return finalResult;
}

void SearchEngine::indexHLV(HLV* hlv) {
    if (hlv != nullptr) {
        trieHLV.insert(hlv->getHoTen(), hlv);
    }
}

void SearchEngine::removeIndexHLV(HLV* hlv) {
    if (hlv != nullptr) {
        trieHLV.remove(hlv->getHoTen(), hlv);
    }
}

MyVector<HLV*> SearchEngine::searchHLV(const string& namePrefix) {
    MyVector<HLV*> rawResult = trieHLV.searchPrefix(namePrefix);
    MyVector<HLV*> finalResult;

    for (size_t i = 0; i < rawResult.size(); ++i) {
        HLV* hlv = rawResult[i];
        if (hlv != nullptr && hlv->getIsActive()) {
            finalResult.push_back(hlv);
        }
    }

    return finalResult;
}

void SearchEngine::indexNhanVien(NhanVien* nv) {
    if (nv != nullptr) {
        trieNhanVien.insert(nv->getHoTen(), nv);
    }
}

void SearchEngine::removeIndexNhanVien(NhanVien* nv) {
    if (nv != nullptr) {
        trieNhanVien.remove(nv->getHoTen(), nv);
    }
}

MyVector<NhanVien*> SearchEngine::searchNhanVien(const string& namePrefix) {
    MyVector<NhanVien*> rawResult = trieNhanVien.searchPrefix(namePrefix);
    MyVector<NhanVien*> finalResult;

    for (size_t i = 0; i < rawResult.size(); ++i) {
        NhanVien* nv = rawResult[i];
        if (nv != nullptr && nv->getIsActive()) {
            finalResult.push_back(nv);
        }
    }

    return finalResult;
}
