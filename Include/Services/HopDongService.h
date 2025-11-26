#pragma once
#include <string>

#include "MyHashTable.h"

using namespace std;

class HopDong;

class HopDongService {
public:
    static void themHopDong(const string& maHV, const string& maGT, const string& maNV,
                           const string& ngayDK, const string& ngayHetHan, int soBuoiPT = 0);
    static void suaHopDong(const string& maHD, const string& maHV, const string& maGT,
                          const string& maNV, const string& ngayDK, const string& ngayHetHan);
    static void huyHopDong(const string& maHD);
    static void kichHoatHopDong(const string& maHD);
    static const MyHashTable<HopDong*>& getAllHopDong();
};
