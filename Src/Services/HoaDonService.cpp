#include "HoaDonService.h"
#include "HoaDon.h"
#include "ChiTietHoaDon_HH.h"
#include "ChiTietHoaDon_GT.h"
#include "HangHoa.h"
#include "GoiTap.h"
#include "Validator.h"
#include "QuanLy.h"

HoaDon *HoaDonService::taoHoaDon(const string &maNV, const string &maHV,
                                 const string &ngayLap, const string &phuongThucTT)
{

    // Validate ngày lập
    string errorMsg = Validator::validateNgay(ngayLap);
    if (!errorMsg.empty())
    {
        // std::cerr << "❌ Lỗi ngày lập: " << errorMsg << std::endl;
        return nullptr;
    }

    // Validate phương thức thanh toán
    if (phuongThucTT.empty())
    {
        // std::cerr << "❌ Phương thức thanh toán trống!" << std::endl;
        return nullptr;
    }

    QuanLy &ql = QuanLy::getInstance();

    // ✅ FIX: Cho phép ADMIN tạo hóa đơn
    NhanVien *nv = nullptr;
    if (maNV != "ADMIN")
    {
        nv = ql.getNhanVien(maNV);
        if (nv == nullptr)
        {
            return nullptr;
        }
    }
    else
    {
    }

    // Validate Hội viên
    HoiVien *hv = ql.getHoiVien(maHV);
    if (hv == nullptr)
    {
        // std::cerr << "❌ Mã hội viên không tồn tại: " << maHV << std::endl;
        return nullptr;
    }
    // std::cout << "   ✅ Found HoiVien: " << hv->getHoTen() << std::endl;

    HoaDon *newHoaDon = HoaDon::create(ngayLap, phuongThucTT, nv, hv);

    if (newHoaDon == nullptr)
    {
        // std::cerr << "❌ HoaDon::create() returned nullptr!" << std::endl;
        return nullptr;
    }

    // std::cout << "   ✅ HoaDon created: " << newHoaDon->getID() << std::endl;

    // Thêm vào QuanLy
    if (ql.addHoaDon(newHoaDon))
    {
        ql.setDirty(true);
        // std::cout << "   ✅ HoaDon added to QuanLy" << std::endl;
        // std::cout << "=========================================\n" << std::endl;
        return newHoaDon;
    }
    else
    {
        // std::cerr << "❌ Failed to add HoaDon to QuanLy!" << std::endl;
        delete newHoaDon;
        return nullptr;
    }
}

void HoaDonService::themHangHoaVaoHoaDon(const string &maHD, const string &maHH, int soLuong, double donGia)
{
    // std::cout << "📦 Adding HangHoa to HoaDon: " << maHH << " x" << soLuong << std::endl;

    QuanLy &ql = QuanLy::getInstance();
    HoaDon *hd = ql.getHoaDon(maHD);
    if (hd == nullptr)
    {
        // std::cerr << "❌ HoaDon not found: " << maHD << std::endl;
        return;
    }

    HangHoa *hh = ql.getHangHoa(maHH);
    if (hh == nullptr)
    {
        // std::cerr << "❌ HangHoa not found: " << maHH << std::endl;
        return;
    }

    string errorMsg = Validator::validateSoDuong(soLuong);
    if (!errorMsg.empty())
    {
        // std::cerr << "❌ Invalid quantity: " << errorMsg << std::endl;
        return;
    }

    errorMsg = Validator::validateSoDuong(donGia);
    if (!errorMsg.empty())
    {
        // std::cerr << "❌ Invalid price: " << errorMsg << std::endl;
        return;
    }

    if (hh->getSoLuongCon() < soLuong)
    {
        // std::cerr << "❌ Not enough stock! Available: " << hh->getSoLuongCon() << std::endl;
        return;
    }

    ChiTietHoaDon_HH *chiTiet = ChiTietHoaDon_HH::create(soLuong, donGia, nullptr, nullptr);
    chiTiet->setHoaDon(hd);
    chiTiet->setHangHoa(hh);
    hh->setSoLuongCon(hh->getSoLuongCon() - soLuong);

    ql.setDirty(true);
    // std::cout << "   ✅ HangHoa added. Stock: " << hh->getSoLuongCon() << std::endl;
}

void HoaDonService::themGoiTapVaoHoaDon(const string &maHD, const string &maGT, int soLuong, double donGia)
{
    // std::cout << "🎫 Adding GoiTap to HoaDon: " << maGT << " x" << soLuong << std::endl;

    QuanLy &ql = QuanLy::getInstance();
    HoaDon *hd = ql.getHoaDon(maHD);
    if (hd == nullptr)
    {
        // std::cerr << "❌ HoaDon not found: " << maHD << std::endl;
        return;
    }

    GoiTap *gt = ql.getGoiTap(maGT);
    if (gt == nullptr)
    {
        // std::cerr << "❌ GoiTap not found: " << maGT << std::endl;
        return;
    }

    ChiTietHoaDon_GT *chiTiet = ChiTietHoaDon_GT::create(soLuong, donGia, nullptr, nullptr);
    chiTiet->setHoaDon(hd);
    chiTiet->setGoiTap(gt);

    ql.setDirty(true);
    // std::cout << "   ✅ GoiTap added" << std::endl;
}

void HoaDonService::xoaHangHoaKhoiHoaDon(const string &maHD, const string &maHH)
{
    QuanLy &ql = QuanLy::getInstance();
    HoaDon *hd = ql.getHoaDon(maHD);
    if (hd == nullptr)
    {
        return;
    }

    MyVector<ChiTietHoaDon_HH *> &dsChiTiet = hd->getDsChiTietHoaDon_HH();
    for (size_t i = 0; i < dsChiTiet.size(); ++i)
    {
        if (dsChiTiet[i]->getHangHoa()->getID() == maHH)
        {
            HangHoa *hh = ql.getHangHoa(maHH);
            if (hh != nullptr)
            {
                hh->setSoLuongCon(hh->getSoLuongCon() + dsChiTiet[i]->getSoLuong());
            }

            ChiTietHoaDon_HH *itemToDelete = dsChiTiet[i];
            hd->removeChiTietHoaDon_HH(itemToDelete);
            delete itemToDelete;
            ql.setDirty(true);
            return;
        }
    }
}

void HoaDonService::xoaGoiTapKhoiHoaDon(const string &maHD, const string &maGT)
{
    QuanLy &ql = QuanLy::getInstance();
    HoaDon *hd = ql.getHoaDon(maHD);
    if (hd == nullptr)
    {
        return;
    }

    MyVector<ChiTietHoaDon_GT *> &dsChiTiet = hd->getDsChiTietHoaDon_GT();
    for (size_t i = 0; i < dsChiTiet.size(); ++i)
    {
        if (dsChiTiet[i]->getGoiTap()->getID() == maGT)
        {
            ChiTietHoaDon_GT *itemToDelete = dsChiTiet[i];
            hd->removeChiTietHoaDon_GT(itemToDelete);
            delete itemToDelete;
            ql.setDirty(true);
            return;
        }
    }
}

void HoaDonService::tinhHoaDon(double tongTienHang, HoiVien *hv, double &outGiamGia, double &outTongThanhToan)
{
    outGiamGia = 0.0;

    if (hv != nullptr)
    {
        // Lay ty le giam gia tu HoiVien (da duoc tinh dua tren Rank)
        double rate = hv->getDiscountRate();
        outGiamGia = tongTienHang * rate;
    }

    outTongThanhToan = tongTienHang - outGiamGia;
    if (outTongThanhToan < 0)
        outTongThanhToan = 0;
}

const MyHashTable<HoaDon *> &HoaDonService::getAllHoaDon()
{
    return QuanLy::getInstance().getDsHoaDon();
}