#pragma once
#include <string>
#include <fstream>
#include "MyVector.h"
#include "HoiVien.h"
#include "NhanVien.h"
#include "HLV.h"
#include "GoiTap.h"
#include "HangHoa.h"
#include "HoaDon.h"
#include "ChiTietHD.h"
#include "ThamGia.h"
#include "IDGenerator.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;

/**
 * Lớp QL - Trung tâm quản lý toàn bộ hệ thống phòng gym
 * - Điểm truy cập duy nhất (Single Point of Entry)
 * - Quản lý tập trung tất cả dữ liệu
 * - Đảm bảo tính toàn vẹn dữ liệu
 * - Xử lý file I/O (.txt)
 */
class QL {
private:
    // === STORAGE - Lưu trữ tất cả dữ liệu ===
    MyVector<HoiVien> dsHoiVien;
    MyVector<NhanVien> dsNhanVien;
    MyVector<HLV> dsHLV;
    MyVector<GoiTap> dsGoiTap;
    MyVector<HangHoa> dsHangHoa;
    MyVector<HoaDon> dsHoaDon;
    MyVector<ThamGia> dsThamGia;

    // IDGenerator để sinh mã tự động
    IDGenerator* idGen;

    // === HELPER FUNCTIONS ===
    // Hàm tìm kiếm nội bộ (trả về index, -1 nếu không tìm thấy)
    int timIndexHoiVien(const string& maHV) const;
    int timIndexNhanVien(const string& maNV) const;
    int timIndexHLV(const string& maHLV) const;
    int timIndexGoiTap(const string& maGoi) const;
    int timIndexHangHoa(const string& maHH) const;
    int timIndexHoaDon(const string& maHD) const;

    // Hàm tách chuỗi theo delimiter
    MyVector<string> split(const string& s, char delimiter) const;

    // Hàm tính ngày hết hạn (ngayDK + số tháng của gói)
    string tinhNgayHetHan(const string& ngayDK, int soThang) const;

public:
    QL();
    ~QL();

    // Thiết lập IDGenerator
    void setIDGenerator(IDGenerator* gen);

    // === CRUD - HỘI VIÊN ===
    bool themHoiVien(const HoiVien& hv);
    bool suaHoiVien(const string& maHV, const HoiVien& hvMoi);
    bool xoaHoiVien(const string& maHV);
    HoiVien* timHoiVien(const string& maHV);
    const MyVector<HoiVien>& layDanhSachHoiVien() const;

    // === CRUD - NHÂN VIÊN ===
    bool themNhanVien(const NhanVien& nv);
    bool suaNhanVien(const string& maNV, const NhanVien& nvMoi);
    bool xoaNhanVien(const string& maNV);
    NhanVien* timNhanVien(const string& maNV);
    const MyVector<NhanVien>& layDanhSachNhanVien() const;

    // === CRUD - HLV ===
    bool themHLV(const HLV& hlv);
    bool suaHLV(const string& maHLV, const HLV& hlvMoi);
    bool xoaHLV(const string& maHLV);
    HLV* timHLV(const string& maHLV);
    const MyVector<HLV>& layDanhSachHLV() const;

    // === CRUD - GÓI TẬP ===
    bool themGoiTap(const GoiTap& gt);
    bool suaGoiTap(const string& maGoi, const GoiTap& gtMoi);
    bool xoaGoiTap(const string& maGoi);
    GoiTap* timGoiTap(const string& maGoi);
    const MyVector<GoiTap>& layDanhSachGoiTap() const;

    // === CRUD - HÀNG HÓA ===
    bool themHangHoa(const HangHoa& hh);
    bool suaHangHoa(const string& maHH, const HangHoa& hhMoi);
    bool xoaHangHoa(const string& maHH);
    HangHoa* timHangHoa(const string& maHH);
    const MyVector<HangHoa>& layDanhSachHangHoa() const;

    // === NGHIỆP VỤ - ĐĂNG KÝ GÓI TẬP ===
    // Hội viên đăng ký một gói tập
    bool dangKyGoiTap(const string& maHV, const string& maGoi, const string& ngayDK);
    // Hủy đăng ký
    bool huyDangKy(const string& maHV, const string& maGoi, const string& ngayDK);
    // Lấy danh sách gói tập của một hội viên
    MyVector<ThamGia> layGoiTapCuaHoiVien(const string& maHV) const;
    // Lấy danh sách hội viên của một gói tập
    MyVector<ThamGia> layHoiVienCuaGoiTap(const string& maGoi) const;

    // === NGHIỆP VỤ - HÓA ĐƠN ===
    // Tạo hóa đơn mới (trả về mã hóa đơn, rỗng nếu thất bại)
    string taoHoaDon(const string& maHV, const string& maNV, 
                     const string& ngayLap, const string& phuongThucTT);
    
    // Thêm sản phẩm vào hóa đơn (tự động lấy giá niêm yết)
    bool themSanPhamVaoHoaDon(const string& maHD, const string& maSP, int soLuong);
    
    // Xóa sản phẩm khỏi hóa đơn
    bool xoaSanPhamKhoiHoaDon(const string& maHD, const string& maSP);
    
    // Hoàn tất hóa đơn (cập nhật tồn kho, trả về tổng tiền)
    bool hoanTatHoaDon(const string& maHD, double& tongTienOut);
    
    // Tìm hóa đơn
    HoaDon* timHoaDon(const string& maHD);
    const MyVector<HoaDon>& layDanhSachHoaDon() const;

    // === FILE I/O - LƯU/TẢI DỮ LIỆU ===
    // Lưu toàn bộ dữ liệu vào thư mục chỉ định
    bool luuDuLieu(const string& duongDanThuMuc) const;
    
    // Tải toàn bộ dữ liệu từ thư mục chỉ định
    bool taiDuLieu(const string& duongDanThuMuc);

    // === TIỆN ÍCH ===
    // Đếm số lượng
    size_t demHoiVien() const { return dsHoiVien.size(); }
    size_t demNhanVien() const { return dsNhanVien.size(); }
    size_t demHLV() const { return dsHLV.size(); }
    size_t demGoiTap() const { return dsGoiTap.size(); }
    size_t demHangHoa() const { return dsHangHoa.size(); }
    size_t demHoaDon() const { return dsHoaDon.size(); }
};