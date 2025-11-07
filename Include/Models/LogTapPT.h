#pragma once
#include <string>
using namespace std;

class HoiVien;
class HLV;

class LogTapPT {
private:
    string id;
    string ngayGio;     // Ngày giờ tập (format: "DD/MM/YYYY HH:MM")
    HoiVien* hv;        // Hội viên tập
    HLV* hlv;          // Huấn luyện viên phụ trách
public:
    LogTapPT();
    LogTapPT(const string&, const string&, HoiVien* = nullptr, HLV* = nullptr);
    LogTapPT(const LogTapPT&);
    ~LogTapPT();

    const string& getID() const;
    const string& getNgayGio() const;
    const HoiVien& getHoiVien() const;
    const HLV& getHLV() const;

    void setNgayGio(const string&);
    void setHoiVien(HoiVien*);
    void setHLV(HLV*);

    static LogTapPT* create(const string&, const string&, HoiVien* = nullptr, HLV* = nullptr);
    static LogTapPT* create(const string&, HoiVien* = nullptr, HLV* = nullptr);
    string read();
};