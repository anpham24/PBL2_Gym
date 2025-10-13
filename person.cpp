#include "person.h"

Person::Person() {

}

Person::Person(const string& ID, const string& hoTen, const string& sdt, const string& gioiTinh)
        : ID(ID), hoTen(hoTen), sdt(sdt), gioiTinh(gioiTinh) {

}

string Person::getID() const {
    return ID;
}

string Person::getHoTen() const { 
    return hoTen; 
}

string Person::getSDT() const { 
    return sdt; 
}

string Person::getGioiTinh() const { 
    return gioiTinh; 
}

void Person::setID(const string& id) { 
    this->ID = id; 
}

void Person::setHoTen(const string& ten) { 
    this->hoTen = ten; 
}

void Person::setSDT(const string& so) { 
    sdt = so; 
}

void Person::setGioiTinh(const string& gt) { 
    gioiTinh = gt; 
}