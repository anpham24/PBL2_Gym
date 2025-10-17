#include "../Include/Person.h"
#include <ctime>
#include <sstream>
#include <iomanip>

map <int, int> Person::nextIDMap;
int Person::lastYear = 0;

string Person::generateID(int prefix) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentYear = (now->tm_year + 1900) % 100;

    if (currentYear != lastYear) {
        nextIDMap.clear();
        lastYear = currentYear;
    }

    int& nextID = nextIDMap[prefix];

    ostringstream oss;
    oss << setw(2) << setfill('0') << prefix
        << setw(2) << setfill('0') << currentYear
        << setw(4) << setfill('0') << ++nextID;
    return oss.str();
}

Person::Person() {

}

Person::Person(const string& hoTen, const string& sdt, const string& gioiTinh)
        : hoTen(hoTen), sdt(sdt), gioiTinh(gioiTinh) {

}

Person::~Person() {

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