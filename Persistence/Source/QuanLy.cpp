#include "../Include/QuanLy.h"
#include "../../Models/Include/HoiVien.h"
#include "../../Models/Include/HLV.h"
#include "../../Models/Include/NhanVien.h"
#include "../../Models/Include/GoiTap.h"
#include "../../Models/Include/LopHoc.h"
#include "../../Models/Include/MonTap.h"
#include "../../Models/Include/HopDong.h"
#include "../../Models/Include/HangHoa.h"
#include "../../Models/Include/HoaDon.h"
#include "../../Models/Include/ChiTietHoaDon_GT.h"
#include "../../Models/Include/ChiTietHoaDon_HH.h"
#include <fstream>
#include <sstream>

// ============================================================================
// DESTRUCTOR
// ============================================================================

QuanLy::~QuanLy() {
    dsHLV.clear(); 
    dsNhanVien.clear();
    dsGoiTap.clear();
    dsLopHoc.clear();
    dsMonTap.clear();
    dsHangHoa.clear();
}

// ============================================================================
// ĐỌC/GHI FILE
// ============================================================================

void QuanLy::setDirty(bool status) {
    this->isDirty = status;
}

bool QuanLy::getIsDirty() const {
    return this->isDirty;
}

void QuanLy::saveAllData(const string& folderPath) const {
    // Lưu dữ liệu vào các file .txt
    // Mỗi entity sẽ có một file riêng
    
    // Ví dụ: lưu GoiTap
    std::ofstream fileGT(folderPath + "/goitap.txt");
    if (fileGT.is_open()) {
        for (size_t i = 0; i < dsGoiTap.size(); ++i) {
            fileGT << dsGoiTap[i]->read() << "\n";
        }
        fileGT.close();
    }
    
    // Lưu HangHoa
    std::ofstream fileHH(folderPath + "/hanghoa.txt");
    if (fileHH.is_open()) {
        for (size_t i = 0; i < dsHangHoa.size(); ++i) {
            fileHH << dsHangHoa[i]->read() << "\n";
        }
        fileHH.close();
    }
    
    // Lưu MonTap
    std::ofstream fileMT(folderPath + "/montap.txt");
    if (fileMT.is_open()) {
        for (size_t i = 0; i < dsMonTap.size(); ++i) {
            fileMT << dsMonTap[i]->read() << "\n";
        }
        fileMT.close();
    }
    
    // Tương tự cho các entity khác...
}

void QuanLy::loadAllData(const string& folderPath) {
    // Đọc dữ liệu từ các file .txt
    // Cần parse từng dòng và tạo lại đối tượng
    
    // Ví dụ: đọc GoiTap
    std::ifstream fileGT(folderPath + "/goitap.txt");
    if (fileGT.is_open()) {
        string line;
        while (std::getline(fileGT, line)) {
            // Parse line và tạo GoiTap object
            // Giả sử format: id,tenGoi,thoiGian,gia
            // GoiTap* gt = new GoiTap(...);
            // addGoiTap(gt);
        }
        fileGT.close();
    }
    
    // Tương tự cho các entity khác...
}

// ============================================================================
// QUẢN LÝ HỘI VIÊN
// ============================================================================

bool QuanLy::addHoiVien(HoiVien* hv) {
    if (hv == nullptr)
        return false;
    if (dsHoiVien.search(hv->getID()) != nullptr) 
        return false;
    dsHoiVien.insert(hv->getID(), hv);
    return true;
}

bool QuanLy::removeHoiVien(const string& maHV) {
    HoiVien** hvPtr = dsHoiVien.search(maHV);
    
    // Nếu không tìm thấy Hội viên, hoặc con trỏ không hợp lệ
    if (hvPtr == nullptr || *hvPtr == nullptr) {
        return false; 
    }
    
    HoiVien* hvCanXoa = *hvPtr;

    hvCanXoa->setHLV(nullptr);

    MyVector<HopDong*>& dsHopDongLienQuan = hvCanXoa->getDsHopDong();
    if (!dsHopDongLienQuan.empty()) {
        hvCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa Hội viên nếu còn Hợp đồng liên quan
    }
    
    MyVector<HoaDon*>& dsHoaDonLienQuan = hvCanXoa->getDsHoaDon();
    if (!dsHoaDonLienQuan.empty()) {
        hvCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa Hội viên nếu còn Hóa đơn liên quan
    }

    bool removed = dsHoiVien.del(maHV);
    if (removed) {
        delete hvCanXoa; 
    }
    
    // Tầng Controller sẽ gọi setDirty(true)
    return removed;
}


HoiVien* QuanLy::getHoiVien(const string& maHV) {
    HoiVien** result = dsHoiVien.search(maHV);
    return result ? *result : nullptr;
}

const HoiVien* QuanLy::getHoiVien(const string& maHV) const {
    HoiVien* const* result = dsHoiVien.search(maHV);
    return result ? *result : nullptr;
}

// ============================================================================
// QUẢN LÝ HUẤN LUYỆN VIÊN (HLV)
// ============================================================================

bool QuanLy::addHLV(HLV* hlv) {
    if (hlv == nullptr) return false;
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        if (dsHLV[i]->getID() == hlv->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsHLV.push_back(hlv);
    return true;
}

bool QuanLy::removeHLV(const string& maHLV) {
    int hlvIndex = -1;
    HLV* hlvCanXoa = nullptr;

    for (size_t i = 0; i < dsHLV.size(); ++i) { 
        HLV* currentHLV = dsHLV[i];
        
        if (currentHLV != nullptr && currentHLV->getID() == maHLV) { 
            hlvCanXoa = currentHLV;
            hlvIndex = i;
            break;
        }
    }

    if (hlvCanXoa == nullptr) {
        return false; 
    }

    MyVector<HoiVien*>& dsHoiVienLienQuan = hlvCanXoa->getDsHoiVien();
    for (size_t i = 0; i < dsHoiVienLienQuan.size(); ++i) {
        HoiVien* hv = dsHoiVienLienQuan[i];
        
        if (hv != nullptr) {
            hv->setHLV(nullptr); 
        }
    }

    MyVector<LopHoc*>& dsLopHocLienQuan = hlvCanXoa->getDsLopHoc();
    for (size_t i = 0; i < dsLopHocLienQuan.size(); ++i) {
        LopHoc* lh = dsLopHocLienQuan[i];
        
        if (lh != nullptr) {
            lh->setHLV(nullptr); 
        }
    }
    
    bool removed = dsHLV.erase(hlvIndex); 
    return removed;
}

HLV* QuanLy::getHLV(const string& maHLV) {
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        if (dsHLV[i]->getID() == maHLV) {
            return dsHLV[i];
        }
    }
    return nullptr;
}

const HLV* QuanLy::getHLV(const string& maHLV) const {
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        if (dsHLV[i]->getID() == maHLV) {
            return dsHLV[i];
        }
    }
    return nullptr;
}

// ============================================================================
// QUẢN LÝ NHÂN VIÊN
// ============================================================================

bool QuanLy::addNhanVien(NhanVien* nv) {
    if (nv == nullptr) return false;
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        if (dsNhanVien[i]->getID() == nv->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsNhanVien.push_back(nv);
    return true;
}

bool QuanLy::removeNhanVien(const string& maNV) {
    int nvIndex = -1;
    NhanVien* nvCanXoa = nullptr;

    for (size_t i = 0; i < dsNhanVien.size(); ++i) { 
        NhanVien* currentNV = dsNhanVien[i];
        
        if (currentNV != nullptr && currentNV->getID() == maNV) { 
            nvCanXoa = currentNV;
            nvIndex = i;
            break;
        }
    }

    if (nvCanXoa == nullptr) {
        return false; 
    }

    MyVector<HopDong*>& dsHopDongLienQuan = nvCanXoa->getDsHopDong();
    if (!dsHopDongLienQuan.empty()) {
        nvCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa NhanVien nếu còn HopDong liên quan
    }

    MyVector<HoaDon*>& dsHoaDonLienQuan = nvCanXoa->getDsHoaDon();
    if (!dsHoaDonLienQuan.empty()) {
        nvCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa NhanVien nếu còn HoaDon liên quan
    }

    bool removed = dsNhanVien.erase(nvIndex);
    return removed;
}

NhanVien* QuanLy::getNhanVien(const string& maNV) {
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        if (dsNhanVien[i]->getID() == maNV) {
            return dsNhanVien[i];
        }
    }
    return nullptr;
}

const NhanVien* QuanLy::getNhanVien(const string& maNV) const {
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        if (dsNhanVien[i]->getID() == maNV) {
            return dsNhanVien[i];
        }
    }
    return nullptr;
}

// ============================================================================
// QUẢN LÝ GÓI TẬP
// ============================================================================

bool QuanLy::addGoiTap(GoiTap* gt) {
    if (gt == nullptr) return false;
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        if (dsGoiTap[i]->getID() == gt->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsGoiTap.push_back(gt);
    return true;
}

bool QuanLy::removeGoiTap(const string& maGoi) {
    int gtIndex = -1;
    GoiTap* gtCanXoa = nullptr;

    for (size_t i = 0; i < dsGoiTap.size(); ++i) { 
        GoiTap* currentGT = dsGoiTap[i];

        if (currentGT != nullptr && currentGT->getID() == maGoi) {
            gtCanXoa = currentGT;
            gtIndex = i;
            break;
        }
    }

    if (gtCanXoa == nullptr) {
        return false; 
    }

    MyVector<HopDong*>& dsHopDongLienQuan = gtCanXoa->getDsHopDong();
    if (!dsHopDongLienQuan.empty()) {
        gtCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa GoiTap nếu còn HopDong liên quan
    }

    MyVector<ChiTietHoaDon_GT*>& dsChiTietHoaDon_GTLienQuan = gtCanXoa->getDsChiTietHoaDon_GT();
    if (!dsChiTietHoaDon_GTLienQuan.empty()) {
        gtCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa GoiTap nếu còn ChiTietHoaDon_GT liên quan
    }

    bool removed = dsGoiTap.erase(gtIndex);
    return removed;
}

GoiTap* QuanLy::getGoiTap(const string& maGoi) {
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        if (dsGoiTap[i]->getID() == maGoi) {
            return dsGoiTap[i];
        }
    }
    return nullptr;
}

const GoiTap* QuanLy::getGoiTap(const string& maGoi) const {
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        if (dsGoiTap[i]->getID() == maGoi) {
            return dsGoiTap[i];
        }
    }
    return nullptr;
}

// ============================================================================
// QUẢN LÝ LỚP HỌC
// ============================================================================

bool QuanLy::addLopHoc(LopHoc* lh) {
    if (lh == nullptr) return false;
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        if (dsLopHoc[i]->getID() == lh->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsLopHoc.push_back(lh);
    return true;
}

bool QuanLy::removeLopHoc(const string& maLop) {
    int lhIndex = -1;
    LopHoc* lhCanXoa = nullptr;

    for (size_t i = 0; i < dsLopHoc.size(); ++i) { 
        LopHoc* currentLH = dsLopHoc[i];
        
        if (currentLH != nullptr && currentLH->getID() == maLop) { 
            lhCanXoa = currentLH;
            lhIndex = i;
            break;
        }
    }

    if (lhCanXoa == nullptr) {
        return false; 
    }

    lhCanXoa->setMonTap(nullptr);
    lhCanXoa->setHLV(nullptr);

    bool removed = dsLopHoc.erase(lhIndex);
    return removed;
}

LopHoc* QuanLy::getLopHoc(const string& maLop) {
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        if (dsLopHoc[i]->getID() == maLop) {
            return dsLopHoc[i];
        }
    }
    return nullptr;
}

const LopHoc* QuanLy::getLopHoc(const string& maLop) const {
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        if (dsLopHoc[i]->getID() == maLop) {
            return dsLopHoc[i];
        }
    }
    return nullptr;
}

// ============================================================================
// QUẢN LÝ MÔN TẬP
// ============================================================================

bool QuanLy::addMonTap(MonTap* mt) {
    if (mt == nullptr) return false;
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap[i]->getID() == mt->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsMonTap.push_back(mt);
    return true;
}

bool QuanLy::removeMonTap(const string& maMon) {
    int mtIndex = -1;
    MonTap* mtCanXoa = nullptr;

    for (size_t i = 0; i < dsMonTap.size(); ++i) { 
        MonTap* currentMonTap = dsMonTap[i];
        
        if (currentMonTap != nullptr && currentMonTap->getID() == maMon) { 
            mtCanXoa = currentMonTap;
            mtIndex = i;
            break;
        }
    }

    if (mtCanXoa == nullptr) {
        return false; 
    }

    MyVector<LopHoc*>& dsLopHocLienQuan = mtCanXoa->getDsLopHoc();
    for (size_t i = 0; i < dsLopHocLienQuan.size(); ++i) {
        LopHoc* lh = dsLopHocLienQuan[i];

        if (lh != nullptr) {
            lh->setHLV(nullptr);
            lh->setMonTap(nullptr);
            dsLopHoc.erase(i);
        }
    }

    MyVector<GoiTap*>& dsGoiTapLienQuan = mtCanXoa->getDsGoiTap();
    for (size_t i = 0; i < dsGoiTapLienQuan.size(); ++i) {
        GoiTap* gt = dsGoiTapLienQuan[i];

        if (gt != nullptr) {
            gt->removeMonTap(mtCanXoa);
        }
    }

    bool removed = dsMonTap.erase(mtIndex);
    return removed;
}

MonTap* QuanLy::getMonTap(const string& maMon) {
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap[i]->getID() == maMon) {
            return dsMonTap[i];
        }
    }
    return nullptr;
}

const MonTap* QuanLy::getMonTap(const string& maMon) const {
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap[i]->getID() == maMon) {
            return dsMonTap[i];
        }
    }
    return nullptr;
}

// ============================================================================
// QUẢN LÝ HỢP ĐỒNG
// ============================================================================

bool QuanLy::addHopDong(HopDong* hd) {
    if (hd == nullptr)
        return false;
    if (dsHopDong.search(hd->getID()) != nullptr)
        return false;
    dsHopDong.insert(hd->getID(), hd);
    return true;
}

// Không có hàm xóa HopDong vì HopDong là bất biến

bool QuanLy::cancelHopDong(const string& maHD) {
    HopDong** hdPtr = dsHopDong.search(maHD);
    if (hdPtr == nullptr || *hdPtr == nullptr) {
        return false; 
    }

    HopDong* hdCanHuy = *hdPtr;
    hdCanHuy->setIsActive(false); 
    return true;
}

HopDong* QuanLy::getHopDong(const string& maHD) {
    HopDong** result = dsHopDong.search(maHD);
    return result ? *result : nullptr;
}

const HopDong* QuanLy::getHopDong(const string& maHD) const {
    HopDong* const* result = dsHopDong.search(maHD);
    return result ? *result : nullptr;
}

// ============================================================================
// QUẢN LÝ HÀNG HÓA
// ============================================================================

bool QuanLy::addHangHoa(HangHoa* hh) {
    if (hh == nullptr) return false;
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        if (dsHangHoa[i]->getID() == hh->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsHangHoa.push_back(hh);
    return true;
}

bool QuanLy::removeHangHoa(const string& maHH) {
    int hhIndex = -1;
    HangHoa* hhCanXoa = nullptr;

    for (size_t i = 0; i < dsHangHoa.size(); ++i) { 
        HangHoa* currentHH = dsHangHoa[i];

        if (currentHH != nullptr && currentHH->getID() == maHH) {
            hhCanXoa = currentHH;
            hhIndex = i;
            break;
        }
    }

    if (hhCanXoa == nullptr) {
        return false; 
    }

    MyVector<ChiTietHoaDon_HH*>& dsChiTietHoaDon_HHLienQuan = hhCanXoa->getDsChiTietHoaDon_HH();
    if (!dsChiTietHoaDon_HHLienQuan.empty()) {
        hhCanXoa->setIsActive(false); // Chỉ đánh dấu không hoạt động
        return true; // Không được xóa HangHoa nếu còn ChiTietHoaDon_HH liên quan
    }

    bool removed = dsHangHoa.erase(hhIndex);
    return removed;
}

const HangHoa* QuanLy::getHangHoa(const string& maHH) const {
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        if (dsHangHoa[i]->getID() == maHH) {
            return dsHangHoa[i];
        }
    }
    return nullptr;
}

HangHoa* QuanLy::getHangHoa(const string& maHH) {
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        if (dsHangHoa[i]->getID() == maHH) {
            return dsHangHoa[i];
        }
    }
    return nullptr;
}

// ============================================================================
// QUẢN LÝ HÓA ĐƠN
// ============================================================================

bool QuanLy::addHoaDon(HoaDon* hd) {
    if (hd == nullptr)
        return false;
    if (dsHoaDon.search(hd->getID()) != nullptr)
        return false;
    dsHoaDon.insert(hd->getID(), hd);
    return true;
}

// Vì HoaDon là bất biến, không có hàm xóa HoaDon

HoaDon* QuanLy::getHoaDon(const string& maHD) {
    HoaDon** result = dsHoaDon.search(maHD);
    return result ? *result : nullptr;
}

const HoaDon* QuanLy::getHoaDon(const string& maHD) const {
    HoaDon* const* result = dsHoaDon.search(maHD);
    return result ? *result : nullptr;
}
