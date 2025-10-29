#include <string>
using namespace std;

class MonTap {
private:
    string id;
    string tenMon;
    string lichTap;
    string maHLV;
public:
    MonTap();
    MonTap(const string&, const string&, const string& = "");
    MonTap(const MonTap& other);
    ~MonTap();

    const string& getID() const;
    const string& getTenMon() const;
    const string& getLichTap() const;
    const string& getMaHLV() const;

    void setTenMon(const string&);
    void setLichTap(const string&);
    void setMaHLV(const string&);

    static MonTap* create(const string&, const string&, const string& = "");
    string read() const;
};