#include "../Include/QL.h"

// ============================================================================
// CONSTRUCTOR & DESTRUCTOR
// ============================================================================

QL::QL() : idGen(nullptr) {}

QL::~QL() {}

void QL::setIDGenerator(IDGenerator* gen) {
    this->idGen = gen;
}

// ============================================================================
// HELPER FUNCTIONS - Tìm kiếm nội bộ
// ============================================================================

int QL::timIndexHoiVien(const string& maHV) const {
    for (size_t i = 0; i < dsHoiVien.size(); ++i) {
        if (dsHoiVien[i].getID() == maHV) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int QL::timIndexNhanVien(const string& maNV) const {
    for (size_t i = 0; i < dsNhanVien.size(); ++i) {
        if (dsNhanVien[i].getID() == maNV) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int QL::timIndexHLV(const string& maHLV) const {
    for (size_t i = 0; i < dsHLV.size(); ++i) {
        if (dsHLV[i].getID() == maHLV) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int QL::timIndexGoiTap(const string& maGoi) const {
    for (size_t i = 0; i < dsGoiTap.size(); ++i) {
        if (dsGoiTap[i].getID() == maGoi) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int QL::timIndexHangHoa(const string& maHH) const {
    for (size_t i = 0; i < dsHangHoa.size(); ++i) {
        if (dsHangHoa[i].getID() == maHH) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int QL::timIndexHoaDon(const string& maHD) const {
    for (size_t i = 0; i < dsHoaDon.size(); ++i) {
        if (dsHoaDon[i].getID() == maHD) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Hàm tách chuỗi
MyVector<string> QL::split(const string& s, char delimiter) const {
    MyVector<string> tokens;
    string token;
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == delimiter) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += s[i];
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

// Hàm tính ngày hết hạn (đơn giản: thêm số tháng vào ngày đăng ký)
// Format: YYYY-MM-DD
string QL::tinhNgayHetHan(const string& ngayDK, int soThang) const {
    // TODO: Implement date arithmetic properly
    // Hiện tại trả về ngày đăng ký + note (bạn cần thư viện date hoặc logic phức tạp hơn)
    return ngayDK; // Placeholder - cần implement đúng
}

// ============================================================================
// CRUD - HỘI VIÊN
// ============================================================================

bool QL::themHoiVien(const HoiVien& hv) {
    // Kiểm tra trùng mã
    if (timIndexHoiVien(hv.getID()) != -1) {
        return false; // Đã tồn tại
    }
    dsHoiVien.push_back(hv);
    return true;
}

bool QL::suaHoiVien(const string& maHV, const HoiVien& hvMoi) {
    int idx = timIndexHoiVien(maHV);
    if (idx == -1) return false;
    dsHoiVien[idx] = hvMoi;
    return true;
}

bool QL::xoaHoiVien(const string& maHV) {
    int idx = timIndexHoiVien(maHV);
    if (idx == -1) return false;
    
    // Xóa các bản ghi ThamGia liên quan
    for (size_t i = 0; i < dsThamGia.size(); ) {
        if (dsThamGia[i].getMaHV() == maHV) {
            dsThamGia.erase(i);
        } else {
            ++i;
        }
    }
    
    // Xóa hội viên
    dsHoiVien.erase(idx);
    return true;
}

HoiVien* QL::timHoiVien(const string& maHV) {
    int idx = timIndexHoiVien(maHV);
    return (idx == -1) ? nullptr : &dsHoiVien[idx];
}

const MyVector<HoiVien>& QL::layDanhSachHoiVien() const {
    return dsHoiVien;
}

// ============================================================================
// CRUD - NHÂN VIÊN
// ============================================================================

bool QL::themNhanVien(const NhanVien& nv) {
    if (timIndexNhanVien(nv.getID()) != -1) return false;
    dsNhanVien.push_back(nv);
    return true;
}

bool QL::suaNhanVien(const string& maNV, const NhanVien& nvMoi) {
    int idx = timIndexNhanVien(maNV);
    if (idx == -1) return false;
    dsNhanVien[idx] = nvMoi;
    return true;
}

bool QL::xoaNhanVien(const string& maNV) {
    int idx = timIndexNhanVien(maNV);
    if (idx == -1) return false;
    dsNhanVien.erase(idx);
    return true;
}

NhanVien* QL::timNhanVien(const string& maNV) {
    int idx = timIndexNhanVien(maNV);
    return (idx == -1) ? nullptr : &dsNhanVien[idx];
}

const MyVector<NhanVien>& QL::layDanhSachNhanVien() const {
    return dsNhanVien;
}

// ============================================================================
// CRUD - HLV
// ============================================================================

bool QL::themHLV(const HLV& hlv) {
    if (timIndexHLV(hlv.getID()) != -1) return false;
    dsHLV.push_back(hlv);
    return true;
}

bool QL::suaHLV(const string& maHLV, const HLV& hlvMoi) {
    int idx = timIndexHLV(maHLV);
    if (idx == -1) return false;
    dsHLV[idx] = hlvMoi;
    return true;
}

bool QL::xoaHLV(const string& maHLV) {
    int idx = timIndexHLV(maHLV);
    if (idx == -1) return false;
    dsHLV.erase(idx);
    return true;
}

HLV* QL::timHLV(const string& maHLV) {
    int idx = timIndexHLV(maHLV);
    return (idx == -1) ? nullptr : &dsHLV[idx];
}

const MyVector<HLV>& QL::layDanhSachHLV() const {
    return dsHLV;
}

// ============================================================================
// CRUD - GÓI TẬP
// ============================================================================

bool QL::themGoiTap(const GoiTap& gt) {
    if (timIndexGoiTap(gt.getID()) != -1) return false;
    dsGoiTap.push_back(gt);
    return true;
}

bool QL::suaGoiTap(const string& maGoi, const GoiTap& gtMoi) {
    int idx = timIndexGoiTap(maGoi);
    if (idx == -1) return false;
    dsGoiTap[idx] = gtMoi;
    return true;
}

bool QL::xoaGoiTap(const string& maGoi) {
    int idx = timIndexGoiTap(maGoi);
    if (idx == -1) return false;
    
    // Xóa các bản ghi ThamGia liên quan
    for (size_t i = 0; i < dsThamGia.size(); ) {
        if (dsThamGia[i].getMaGoi() == maGoi) {
            dsThamGia.erase(i);
        } else {
            ++i;
        }
    }
    
    dsGoiTap.erase(idx);
    return true;
}

GoiTap* QL::timGoiTap(const string& maGoi) {
    int idx = timIndexGoiTap(maGoi);
    return (idx == -1) ? nullptr : &dsGoiTap[idx];
}

const MyVector<GoiTap>& QL::layDanhSachGoiTap() const {
    return dsGoiTap;
}

// ============================================================================
// CRUD - HÀNG HÓA
// ============================================================================

bool QL::themHangHoa(const HangHoa& hh) {
    if (timIndexHangHoa(hh.getID()) != -1) return false;
    dsHangHoa.push_back(hh);
    return true;
}

bool QL::suaHangHoa(const string& maHH, const HangHoa& hhMoi) {
    int idx = timIndexHangHoa(maHH);
    if (idx == -1) return false;
    dsHangHoa[idx] = hhMoi;
    return true;
}

bool QL::xoaHangHoa(const string& maHH) {
    int idx = timIndexHangHoa(maHH);
    if (idx == -1) return false;
    dsHangHoa.erase(idx);
    return true;
}

HangHoa* QL::timHangHoa(const string& maHH) {
    int idx = timIndexHangHoa(maHH);
    return (idx == -1) ? nullptr : &dsHangHoa[idx];
}

const MyVector<HangHoa>& QL::layDanhSachHangHoa() const {
    return dsHangHoa;
}

// ============================================================================
// NGHIỆP VỤ - ĐĂNG KÝ GÓI TẬP
// ============================================================================

bool QL::dangKyGoiTap(const string& maHV, const string& maGoi, const string& ngayDK) {
    // Kiểm tra hội viên và gói tập có tồn tại không
    if (timIndexHoiVien(maHV) == -1) return false;
    int idxGoi = timIndexGoiTap(maGoi);
    if (idxGoi == -1) return false;
    
    // Tính ngày hết hạn dựa trên thời gian của gói
    int soThang = dsGoiTap[idxGoi].getThoiGian();
    string ngayHetHan = tinhNgayHetHan(ngayDK, soThang);
    
    // Tạo bản ghi ThamGia
    ThamGia tg(maHV, maGoi, ngayDK, ngayHetHan);
    dsThamGia.push_back(tg);
    
    return true;
}

bool QL::huyDangKy(const string& maHV, const string& maGoi, const string& ngayDK) {
    for (size_t i = 0; i < dsThamGia.size(); ++i) {
        if (dsThamGia[i].getMaHV() == maHV && 
            dsThamGia[i].getMaGoi() == maGoi && 
            dsThamGia[i].getNgayDK() == ngayDK) {
            dsThamGia.erase(i);
            return true;
        }
    }
    return false;
}

MyVector<ThamGia> QL::layGoiTapCuaHoiVien(const string& maHV) const {
    MyVector<ThamGia> ketQua;
    for (size_t i = 0; i < dsThamGia.size(); ++i) {
        if (dsThamGia[i].getMaHV() == maHV) {
            ketQua.push_back(dsThamGia[i]);
        }
    }
    return ketQua;
}

MyVector<ThamGia> QL::layHoiVienCuaGoiTap(const string& maGoi) const {
    MyVector<ThamGia> ketQua;
    for (size_t i = 0; i < dsThamGia.size(); ++i) {
        if (dsThamGia[i].getMaGoi() == maGoi) {
            ketQua.push_back(dsThamGia[i]);
        }
    }
    return ketQua;
}

// ============================================================================
// NGHIỆP VỤ - HÓA ĐƠN
// ============================================================================

string QL::taoHoaDon(const string& maHV, const string& maNV, 
                      const string& ngayLap, const string& phuongThucTT) {
    // Kiểm tra hội viên và nhân viên có tồn tại không
    if (timIndexHoiVien(maHV) == -1) return "";
    if (timIndexNhanVien(maNV) == -1) return "";
    
    // Tạo hóa đơn mới
    HoaDon hd(maHV, maNV, ngayLap, phuongThucTT);
    string maHD = hd.getID();
    
    dsHoaDon.push_back(hd);
    return maHD;
}

bool QL::themSanPhamVaoHoaDon(const string& maHD, const string& maSP, int soLuong) {
    int idxHD = timIndexHoaDon(maHD);
    if (idxHD == -1) return false;
    
    if (soLuong <= 0) return false;
    
    // Tìm hàng hóa để lấy giá niêm yết
    int idxHH = timIndexHangHoa(maSP);
    if (idxHH == -1) return false;
    
    double giaNiemYet = dsHangHoa[idxHH].getGia();
    
    // Tạo chi tiết hóa đơn với 3 tham số: maSP, soLuong, donGia
    ChiTietHD ct(maSP, soLuong, giaNiemYet);
    dsHoaDon[idxHD].addItem(ct);
    
    return true;
}

bool QL::xoaSanPhamKhoiHoaDon(const string& maHD, const string& maSP) {
    int idxHD = timIndexHoaDon(maHD);
    if (idxHD == -1) return false;
    
    return dsHoaDon[idxHD].removeItemByMaSP(maSP);
}

bool QL::hoanTatHoaDon(const string& maHD, double& tongTienOut) {
    int idxHD = timIndexHoaDon(maHD);
    if (idxHD == -1) return false;
    
    HoaDon& hd = dsHoaDon[idxHD];
    
    // Tính tổng tiền
    tongTienOut = hd.getTotal();
    
    // LƯU Ý: Class HangHoa hiện tại KHÔNG có trường soLuong để quản lý tồn kho
    // Nếu muốn quản lý tồn kho, cần thêm:
    //   1. Thêm trường 'int soLuong' vào HangHoa.h
    //   2. Implement getSoLuong() và setSoLuong() trong HangHoa.cpp
    // Code cập nhật tồn kho (đang comment):
    /*
    const MyVector<ChiTietHD>& items = hd.getItems();
    for (size_t i = 0; i < items.size(); ++i) {
        int idxHH = timIndexHangHoa(items[i].getMaSP());
        if (idxHH != -1) {
            int tonKhoHienTai = dsHangHoa[idxHH].getSoLuong();
            int soLuongBan = items[i].getSoLuong();
            dsHangHoa[idxHH].setSoLuong(tonKhoHienTai - soLuongBan);
        }
    }
    */
    
    return true;
}

HoaDon* QL::timHoaDon(const string& maHD) {
    int idx = timIndexHoaDon(maHD);
    return (idx == -1) ? nullptr : &dsHoaDon[idx];
}

const MyVector<HoaDon>& QL::layDanhSachHoaDon() const {
    return dsHoaDon;
}

// ============================================================================
// FILE I/O - LƯU/TẢI DỮ LIỆU
// ============================================================================

bool QL::luuDuLieu(const string& duongDanThuMuc) const {
    string sep = "\\"; // Windows path separator
    
    // Lưu Hội viên
    {
        ofstream file(duongDanThuMuc + sep + "hoivien.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsHoiVien.size(); ++i) {
            file << dsHoiVien[i].read() << "\n";
        }
        file.close();
    }
    
    // Lưu Nhân viên
    {
        ofstream file(duongDanThuMuc + sep + "nhanvien.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsNhanVien.size(); ++i) {
            file << dsNhanVien[i].read() << "\n";
        }
        file.close();
    }
    
    // Lưu HLV
    {
        ofstream file(duongDanThuMuc + sep + "hlv.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsHLV.size(); ++i) {
            file << dsHLV[i].read() << "\n";
        }
        file.close();
    }
    
    // Lưu Gói tập
    {
        ofstream file(duongDanThuMuc + sep + "goitap.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsGoiTap.size(); ++i) {
            file << dsGoiTap[i].read() << "\n";
        }
        file.close();
    }
    
    // Lưu Hàng hóa
    {
        ofstream file(duongDanThuMuc + sep + "hanghoa.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsHangHoa.size(); ++i) {
            file << dsHangHoa[i].read() << "\n";
        }
        file.close();
    }
    
    // Lưu ThamGia
    {
        ofstream file(duongDanThuMuc + sep + "thamgia.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsThamGia.size(); ++i) {
            file << dsThamGia[i].read() << "\n";
        }
        file.close();
    }
    
    // Lưu Hóa đơn (chỉ lưu thông tin header, không lưu items)
    {
        ofstream file(duongDanThuMuc + sep + "hoadon.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsHoaDon.size(); ++i) {
            const HoaDon& hd = dsHoaDon[i];
            file << hd.getID() << ";" << hd.getMaHV() << ";" 
                 << hd.getMaNV() << ";" << hd.getNgayLap() << ";" 
                 << hd.getPhuongThuc() << "\n";
        }
        file.close();
    }
    
    // Lưu Chi tiết hóa đơn
    {
        ofstream file(duongDanThuMuc + sep + "chitiet_hd.txt");
        if (!file.is_open()) return false;
        for (size_t i = 0; i < dsHoaDon.size(); ++i) {
            const HoaDon& hd = dsHoaDon[i];
            const MyVector<ChiTietHD>& items = hd.getItems();
            for (size_t j = 0; j < items.size(); ++j) {
                file << hd.getID() << ";" << items[j].read() << "\n";
            }
        }
        file.close();
    }
    
    return true;
}

bool QL::taiDuLieu(const string& duongDanThuMuc) {
    string sep = "\\";
    string line;
    
    // Xóa dữ liệu cũ
    dsHoiVien.clear();
    dsNhanVien.clear();
    dsHLV.clear();
    dsGoiTap.clear();
    dsHangHoa.clear();
    dsThamGia.clear();
    dsHoaDon.clear();
    
    // Tải Hội viên
    {
        ifstream file(duongDanThuMuc + sep + "hoivien.txt");
        if (file.is_open()) {
            while (getline(file, line)) {
                MyVector<string> fields = split(line, ';');
                if (fields.size() >= 5) {
                    // TODO: Parse theo định dạng read() của HoiVien
                    // Tạm thời skip vì cần biết format chính xác
                }
            }
            file.close();
        }
    }
    
    // Tương tự cho các file khác...
    // NOTE: Cần biết format read() của từng class để parse đúng
    
    return true;
}
