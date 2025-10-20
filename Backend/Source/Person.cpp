#include "../Include/Person.h"

Person::Person() {

}

Person::Person(const string& hoTen, const string& sdt, const string& gioiTinh)
        : hoTen(hoTen), sdt(sdt), gioiTinh(gioiTinh) {

}

Person::~Person() {

}

string Person::getID() const { return this->id; }
string Person::getHoTen() const { return this->hoTen; }
string Person::getSDT() const { return this->sdt; }
string Person::getGioiTinh() const { return this->gioiTinh; }

void Person::setHoTen(const string& ten) { this->hoTen = ten; }
void Person::setSDT(const string& so) { this->sdt = so; }
void Person::setGioiTinh(const string& gt) { this->gioiTinh = gt; }