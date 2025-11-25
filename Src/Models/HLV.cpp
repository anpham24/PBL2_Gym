#include "HLV.h"
#include "LopHoc.h"
#include "LogTapPT.h"
#include "IDGenerator.h"
#include "Config.h"

HLV::HLV() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
    this->luongCoBan = 0.0;
    this->isActive = true;
}

HLV::HLV(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, 
         const string& ngaySinh, const string& chuyenMon, double luong, bool isActive)
    : Person(id, hoTen, sdt, gioiTinh, ngaySinh), 
      chuyenMon(chuyenMon), luongCoBan(luong), isActive(isActive) {}

HLV::HLV(const HLV& other) 
    : Person(other.id, other.hoTen, other.sdt, other.gioiTinh, other.ngaySinh), 
      chuyenMon(other.chuyenMon), luongCoBan(other.luongCoBan), isActive(other.isActive) {}

HLV::~HLV() {}

// Getters
string HLV::getChuyenMon() const { 
    return this->chuyenMon; 
}

double HLV::getLuongCoBan() const { 
    return this->luongCoBan; 
}

// Tính tổng lương = Cơ bản + PT + Lớp
double HLV::getLuong() const {
    double tongLuong = luongCoBan;
    
    // Cộng lương từ PT
    int soBuoiPT = getSoBuoiPT();
    tongLuong += soBuoiPT * Config::HLV_LUONG_PT_MOI_BUOI;
    
    // Cộng lương từ lớp học
    int tongBuoiLop = getTongSoBuoiLop();
    tongLuong += tongBuoiLop * Config::HLV_LUONG_LOP_MOI_BUOI;
    
    return tongLuong;
}

// Đếm số buổi PT
int HLV::getSoBuoiPT() const {
    return dsLogTapPT.size();
}

// Đếm tổng số buổi dạy lớp
int HLV::getTongSoBuoiLop() const {
    int tong = 0;
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        if (dsLopHoc[i] != nullptr) {
            // Giả định mỗi lớp có số buổi = thời lượng (phút) / 60
            // Hoặc có thể thêm field `soBuoi` vào LopHoc
            tong += 1; // Tạm thời đếm mỗi lớp = 1 buổi
        }
    }
    return tong;
}

bool HLV::getIsActive() const { 
    return this->isActive; 
}

// Setters
void HLV::setChuyenMon(const string& chuyenMon) { 
    this->chuyenMon = chuyenMon; 
}

void HLV::setLuongCoBan(double luong) { 
    this->luongCoBan = luong; 
}

void HLV::setIsActive(bool status) { 
    this->isActive = status; 
}

void HLV::addLogTapPT(LogTapPT* log) {
    dsLogTapPT.push_back(log);
}

void HLV::removeLogTapPT(LogTapPT* log) {
    for (size_t i = 0; i < dsLogTapPT.size(); i++) {
        if (dsLogTapPT[i] == log) {
            dsLogTapPT.erase(i);
            break;
        }
    }
}

const MyVector<LogTapPT*>& HLV::getDsLogTapPT() const {
    return dsLogTapPT;
}

MyVector<LogTapPT*>& HLV::getDsLogTapPT() {
    return dsLogTapPT;
}

void HLV::addLopHoc(LopHoc* lh) {
    dsLopHoc.push_back(lh);
}

void HLV::removeLopHoc(LopHoc* lh) {
    for (size_t i = 0; i < dsLopHoc.size(); i++) {
        if (dsLopHoc[i] == lh) {
            dsLopHoc.erase(i);
            break;
        }
    }
}

const MyVector<LopHoc*>& HLV::getDsLopHoc() const {
    return dsLopHoc;
}

MyVector<LopHoc*>& HLV::getDsLopHoc() {
    return dsLopHoc;
}

HLV* HLV::create(const string& id, const string& hoTen, const string& sdt, 
                 const string& gioiTinh, const string& ngaySinh, const string& chuyenMon, 
                 double luong, bool isActive) {
    return new HLV(id, hoTen, sdt, gioiTinh, ngaySinh, chuyenMon, luong, isActive);
}

HLV* HLV::create(const string& hoTen, const string& sdt, const string& gioiTinh, 
                 const string& ngaySinh, const string& chuyenMon, double luong, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
    return new HLV(id, hoTen, sdt, gioiTinh, ngaySinh, chuyenMon, luong, isActive);
}

string HLV::read() const {
    // ✅ Chỉ lưu lương cơ bản
    string result = id + ";" + hoTen + ";" + sdt + ";" + gioiTinh + ";" + 
                    ngaySinh + ";" + chuyenMon + ";" + 
                    to_string(luongCoBan) + ";" + 
                    (isActive ? "true" : "false");
    return result;
}