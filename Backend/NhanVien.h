#include "person.h"

class NhanVien : public Person {
    private:
        double luong;
    public:
        NhanVien();
        NhanVien(const string&, const string&, const string&, const string&, double);
        ~NhanVien();

        double getLuong() const;
        void setLuong(double);

        void create() override;
        void read() const override;
        void update() override;
        void remove() override;
};