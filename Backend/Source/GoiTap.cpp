#include "../Include/GoiTap.h"
#include "../Include/IDGenerator.h"
#define GT_ID 4

GoiTap::GoiTap() {
    this->id = IDGenerator::generateID(GT_ID);
}

GoiTap::GoiTap(const string& tenGoi, int thoiGian, double gia, 
               const string& lichTap)
    : tenGoi(tenGoi), thoiGian(thoiGian), gia(gia), 
      lichTap(lichTap) {
    this->id = IDGenerator::generateID(GT_ID);
}

GoiTap::GoiTap(const GoiTap& other) 
    : id(other.id), tenGoi(other.tenGoi), thoiGian(other.thoiGian), gia(other.gia), 
      lichTap(other.lichTap), maMon(other.maMon) {}

GoiTap::~GoiTap() {}

const string& GoiTap::getID() const { return this->id; }
const string& GoiTap::getTenGoi() const { return this->tenGoi; }
int GoiTap::getThoiGian() const { return this->thoiGian; }
double GoiTap::getGia() const { return this->gia; }
const string& GoiTap::getLichTap() const { return this->lichTap; }
const MyVector<string>& GoiTap::getMaMon() const { return this->maMon; }

void GoiTap::setTenGoi(const string& t) { this->tenGoi = t; }
void GoiTap::setThoiGian(int tg) { this->thoiGian = tg; }
void GoiTap::setGia(double g) { this->gia = g; }
void GoiTap::setLichTap(const string& l) { this->lichTap = l; }
void GoiTap::addMaMon(const string& m) { this->maMon.push_back(m); }

GoiTap GoiTap::create(const string& tenGoi, int thoiGian, double gia, 
                      const string& lichTap)
{
    return GoiTap(tenGoi, thoiGian, gia, lichTap);
}

void GoiTap::update(const string& newTenGoi, int newThoiGian, double newGia, 
                    const string& newLichTap)
{
    this->tenGoi = newTenGoi;
    this->thoiGian = newThoiGian;
    this->gia = newGia;
    this->lichTap = newLichTap;
}

string GoiTap::read() const {
    string result = id + "," + tenGoi + "," + to_string(thoiGian) + "," + to_string(gia) + "," + lichTap;
    // Thêm danh sách mã môn
    for (size_t i = 0; i < maMon.size(); ++i) {
        result += "," + maMon.at(i);
    }
    return result;
}