#include "../Include/GoiTap.h"
#include "../Include/IDGenerator.h"
#include <sstream>

GoiTap::GoiTap() {
    this->id = IDGenerator::generateID(50);
}

GoiTap::GoiTap(const string& tenGoi, int thoiGian, double gia, 
               const string& lichTap, const string& maMon, 
               const string& MaHLV)
    : tenGoi(tenGoi), thoiGian(thoiGian), gia(gia), 
      lichTap(lichTap), maMon(maMon), maHLV(maHLV) {
    this->id = IDGenerator::generateID(50);
}

GoiTap::~GoiTap() {
    
}

string GoiTap::getID() const { return this->id; }
string GoiTap::getTenGoi() const { return this->tenGoi; }
int GoiTap::getThoiGian() const { return this->thoiGian; }
double GoiTap::getGia() const { return this->gia; }
string GoiTap::getLichTap() const { return this->lichTap; }
string GoiTap::getMaMon() const { return this->maMon; }
string GoiTap::getMaHLV() const { return this->maHLV; }

void GoiTap::setTenGoi(const string& t) { this->tenGoi = t; }
void GoiTap::setThoiGian(int tg) { this->thoiGian = tg; }
void GoiTap::setGia(double g) { this->gia = g; }
void GoiTap::setLichTap(const string& l) { this->lichTap = l; }
void GoiTap::setMaMon(const string& m) { this->maMon = m; }
void GoiTap::setMaHLV(const string& h) { this->maHLV = h; }

GoiTap GoiTap::create(const string& tenGoi, int thoiGian, double gia, 
                      const string& lichTap, const string& maMon,
                      const string& maHLV)
{
    return GoiTap(tenGoi, thoiGian, gia, lichTap, maMon, maHLV);
}

void GoiTap::update(const string& newTenGoi, int newThoiGian, double newGia, 
                    const string& newLichTap, const string& newMaMon,
                    const string& newMaHLV)
{
    this->tenGoi = newTenGoi;
    this->thoiGian = newThoiGian;
    this->gia = newGia;
    this->lichTap = newLichTap;
    this->maMon = newMaMon;
    this->maHLV = newMaHLV;
}

string GoiTap::read() const {
    ostringstream oss;
    oss << id << " | " << tenGoi << " | "
        << thoiGian << " | " << gia << " | " 
        << lichTap << " | " << maMon << " | " << maHLV;
    return oss.str();
}