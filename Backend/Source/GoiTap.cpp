#include "../Include/GoiTap.h"
#include "../Include/MonTap.h"
#include "../Include/IDGenerator.h"

GoiTap::GoiTap() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
}

GoiTap::GoiTap(const string& tenGoi, int thoiGian, double gia)
    : tenGoi(tenGoi), thoiGian(thoiGian), gia(gia) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
}

GoiTap::GoiTap(const GoiTap& other) 
    : tenGoi(other.tenGoi), thoiGian(other.thoiGian), gia(other.gia) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
}

GoiTap::~GoiTap() {}

const string& GoiTap::getID() const { return this->id; }
const string& GoiTap::getTenGoi() const { return this->tenGoi; }
int GoiTap::getThoiGian() const { return this->thoiGian; }
double GoiTap::getGia() const { return this->gia; }

void GoiTap::setTenGoi(const string& t) { this->tenGoi = t; }
void GoiTap::setThoiGian(int tg) { this->thoiGian = tg; }
void GoiTap::setGia(double g) { this->gia = g; }

void GoiTap::addMonTap(MonTap* mt) {
    this->dsMonTap.push_back(mt);
}

void GoiTap::removeMonTap(MonTap* mt) {
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap.at(i) == mt) {
            dsMonTap.erase(i);
            return;
        }
    }
}

const MyVector<MonTap*>& GoiTap::getDsMonTap() const {
    return this->dsMonTap;
}

GoiTap* GoiTap::create(const string& tenGoi, int thoiGian, double gia) {
    return new GoiTap(tenGoi, thoiGian, gia);
}


string GoiTap::read() const {
    string result = id + "," + tenGoi + "," + to_string(thoiGian) + "," + to_string(gia) + ",";
    // Thêm danh sách mã môn
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        result += dsMonTap[i]->getID();
        if (i != dsMonTap.size() - 1) {
            result += ";"; // Ngăn cách các mã môn bằng dấu chấm phẩy
        }
    }
    return result;
}