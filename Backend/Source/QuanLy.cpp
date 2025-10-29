#include "../Include/QuanLy.h"
#include "../Include/HoiVien.h"
#include "../Include/HLV.h"
#include "../Include/NhanVien.h"
#include "../Include/GoiTap.h"
#include "../Include/LopHoc.h"
#include "../Include/MonTap.h"
#include "../Include/HopDong.h"
#include "../Include/HangHoa.h"
#include "../Include/HoaDon.h"
#include <fstream>

// ============================================================================
// DESTRUCTOR
// ============================================================================

QuanLy::~QuanLy() {
    // Xóa tất cả các con trỏ trong MyVector
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        delete dsHLV[i];
    }
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        delete dsNhanVien[i];
    }
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        delete dsGoiTap[i];
    }
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        delete dsLopHoc[i];
    }
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        delete dsMonTap[i];
    }
    for (size_t i = 0; i < dsHopDong.size(); ++i) {
        delete dsHopDong[i];
    }
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        delete dsHangHoa[i];
    }
    for (size_t i = 0; i < dsHoaDon.size(); ++i) {
        delete dsHoaDon[i];
    }
    // MyHashTable tự động xóa các HoiVien* trong destructor của nó
}

// ============================================================================
// QUẢN LÝ HỘI VIÊN
// ============================================================================

bool QuanLy::addHoiVien(HoiVien* hv) {
    if (!hv) return false;
    if (dsHoiVien.search(hv->getID())) {
        return false; // Đã tồn tại
    }
    dsHoiVien.insert(hv->getID(), hv);
    return true;
}

bool QuanLy::removeHoiVien(const string& maHV) {
    HoiVien** hvPtr = dsHoiVien.search(maHV);
    if (!hvPtr || !(*hvPtr)) return false;
    HoiVien* hv = *hvPtr;
    
    // Xóa các hợp đồng liên quan
    for (size_t i = 0; i < dsHopDong.size(); ) {
        if (&(dsHopDong[i]->getHoiVien()) == hv) {
            delete dsHopDong[i];
            dsHopDong.erase(i);
        } else {
            ++i;
        }
    }
    
    // Xóa các hóa đơn liên quan
    for (size_t i = 0; i < dsHoaDon.size(); ) {
        if (dsHoaDon[i]->getHoiVien() == hv) {
            delete dsHoaDon[i];
            dsHoaDon.erase(i);
        } else {
            ++i;
        }
    }
    
    // Xóa khỏi hash table và giải phóng bộ nhớ
    bool removed = dsHoiVien.remove(maHV);
    if (removed) {
        delete hv;
    }
    return removed;
}

HoiVien* QuanLy::getHoiVien(const string& maHV) const {
    return dsHoiVien.search(maHV);
}

// ============================================================================
// QUẢN LÝ HUẤN LUYỆN VIÊN (HLV)
// ============================================================================

bool QuanLy::addHLV(HLV* hlv) {
    if (!hlv) return false;
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        if (dsHLV[i]->getID() == hlv->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsHLV.push_back(hlv);
    return true;
}

bool QuanLy::xoaHLV(const string& maHLV) {
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        if (dsHLV[i]->getID() == maHLV) {
            HLV* hlv = dsHLV[i];
            
            // Xóa liên kết với các lớp học
            for (size_t j = 0; j < dsLopHoc.size(); ++j) {
                if (dsLopHoc[j]->getHLV() == hlv) {
                    dsLopHoc[j]->setHLV(nullptr);
                }
            }
            
            delete hlv;
            dsHLV.erase(i);
            return true;
        }
    }
    return false;
}

HLV* QuanLy::timHLV(const string& maHLV) const {
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
    if (!nv) return false;
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        if (dsNhanVien[i]->getID() == nv->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsNhanVien.push_back(nv);
    return true;
}

bool QuanLy::xoaNhanVien(const string& maNV) {
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        if (dsNhanVien[i]->getID() == maNV) {
            delete dsNhanVien[i];
            dsNhanVien.erase(i);
            return true;
        }
    }
    return false;
}

NhanVien* QuanLy::timNhanVien(const string& maNV) const {
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
    if (!gt) return false;
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        if (dsGoiTap[i]->getID() == gt->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsGoiTap.push_back(gt);
    return true;
}

bool QuanLy::xoaGoiTap(const string& maGoi) {
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        if (dsGoiTap[i]->getID() == maGoi) {
            GoiTap* gt = dsGoiTap[i];
            
            // Xóa các hợp đồng liên quan
            for (size_t j = 0; j < dsHopDong.size(); ) {
                if (&(dsHopDong[j]->getGoiTap()) == gt) {
                    delete dsHopDong[j];
                    dsHopDong.erase(j);
                } else {
                    ++j;
                }
            }
            
            delete gt;
            dsGoiTap.erase(i);
            return true;
        }
    }
    return false;
}

GoiTap* QuanLy::timGoiTap(const string& maGoi) const {
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
    if (!lh) return false;
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        if (dsLopHoc[i]->getID() == lh->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsLopHoc.push_back(lh);
    return true;
}

bool QuanLy::removeLopHoc(const string& maLop) {
    for (size_t i = 0; i < dsLopHoc.size(); ++i) {
        if (dsLopHoc[i]->getID() == maLop) {
            delete dsLopHoc[i];
            dsLopHoc.erase(i);
            return true;
        }
    }
    return false;
}

LopHoc* QuanLy::getLopHoc(const string& maLop) const {
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
    if (!mt) return false;
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap[i]->getID() == mt->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsMonTap.push_back(mt);
    return true;
}

bool QuanLy::removeMonTap(const string& maMon) {
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap[i]->getID() == maMon) {
            MonTap* mt = dsMonTap[i];
            
            // Xóa liên kết với các lớp học
            for (size_t j = 0; j < dsLopHoc.size(); ++j) {
                if (dsLopHoc[j]->getMonTap() == mt) {
                    dsLopHoc[j]->setMonTap(nullptr);
                }
            }
            
            delete mt;
            dsMonTap.erase(i);
            return true;
        }
    }
    return false;
}

MonTap* QuanLy::getMonTap(const string& maMon) const {
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
    if (!hd) return false;
    dsHopDong.push_back(hd);
    return true;
}

bool QuanLy::removeHopDong(const string& maHD) {
    for (size_t i = 0; i < dsHopDong.size(); ++i) {
        // Giả sử HopDong có getID(), nếu không thì cần thêm
        // Tạm thời dùng cách so sánh khác
        delete dsHopDong[i];
        dsHopDong.erase(i);
        return true;
    }
    return false;
}

HopDong* QuanLy::getHopDong(const string& maHD) const {
    // Cần thêm method getID() vào HopDong
    // Tạm thời return nullptr
    return nullptr;
}

// ============================================================================
// QUẢN LÝ HÀNG HÓA
// ============================================================================

bool QuanLy::addHangHoa(HangHoa* hh) {
    if (!hh) return false;
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        if (dsHangHoa[i]->getID() == hh->getID()) {
            return false; // Đã tồn tại
        }
    }
    dsHangHoa.push_back(hh);
    return true;
}

bool QuanLy::removeHangHoa(const string& maHH) {
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        if (dsHangHoa[i]->getID() == maHH) {
            delete dsHangHoa[i];
            dsHangHoa.erase(i);
            return true;
        }
    }
    return false;
}

HangHoa* QuanLy::getHangHoa(const string& maHH) const {
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
    if (!hd) return false;
    dsHoaDon.push_back(hd);
    return true;
}

bool QuanLy::removeHoaDon(const string& maHD) {
    for (size_t i = 0; i < dsHoaDon.size(); ++i) {
        if (dsHoaDon[i]->getID() == maHD) {
            delete dsHoaDon[i];
            dsHoaDon.erase(i);
            return true;
        }
    }
    return false;
}

HoaDon* QuanLy::getHoaDon(const string& maHD) const {
    for (size_t i = 0; i < dsHoaDon.size(); ++i) {
        if (dsHoaDon[i]->getID() == maHD) {
            return dsHoaDon[i];
        }
    }
    return nullptr;
}

// ============================================================================
// ĐỌC/GHI FILE
// ============================================================================

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
// HIỂN THỊ DANH SÁCH
// ============================================================================

string QuanLy::getDisplayList(const string& entityName) const {
    std::stringstream ss;
    
    if (entityName == "HoiVien") {
        // Cần implement getAllValues() trong MyHashTable
        ss << "Danh sach Hoi Vien:\n";
        // for (auto hv : dsHoiVien.getAllValues()) {
        //     ss << hv->read() << "\n";
        // }
    } else if (entityName == "GoiTap") {
        ss << "Danh sach Goi Tap:\n";
        for (size_t i = 0; i < dsGoiTap.size(); ++i) {
            ss << dsGoiTap[i]->read() << "\n";
        }
    } else if (entityName == "HangHoa") {
        ss << "Danh sach Hang Hoa:\n";
        for (size_t i = 0; i < dsHangHoa.size(); ++i) {
            ss << dsHangHoa[i]->read() << "\n";
        }
    }
    // Thêm các entity khác...
    
    return ss.str();
}

