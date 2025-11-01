#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>
#include <memory> 

// 1. BAO GỒM CÁC THÀNH PHẦN CHÍNH CỦA BACKEND
#include "../../Backend/Include/QuanLy.h"
#include "../../Backend/Include/HoiVien.h"
#include "../../Backend/Include/NhanVien.h"
#include "../../Backend/Include/HLV.h"
#include "../../Backend/Include/HangHoa.h"
#include "../../Backend/Include/GoiTap.h"
#include "../../Backend/Include/HoaDon.h"

using namespace std;


// 2. ĐỊNH NGHĨA CÁC STRUCT MÀ FRONTEND (main.cpp) CẦN
enum class UserRole { ADMIN, STAFF };
enum class MembershipType { BASIC, PREMIUM, VIP };

struct User {
    string username;
    string password;
    UserRole role;
};

// Ánh xạ tới: HoiVien
struct Member {
    string id;
    string name;
    string phone;
    string email;
    MembershipType type;
    string startDate;
    string endDate;
    bool isActive;
};

// Ánh xạ tới: HangHoa
struct Product {
    string id;
    string name;
    double price;
    int quantity;
    string category;
};

// Ánh xạ tới: NhanVien
struct Employee {
    string id;
    string name;
    string phone;
    string position;
    double salary;
    string startDate;
};

// Ánh xạ tới: HLV
struct Trainer {
    string id;
    string name;
    string phone;
    string specialty;
    double hourlyRate;
    int experience;
};

struct InvoiceItem {
    string productId;
    string productName;
    int quantity;
    double price;
};

// Ánh xạ tới: HoaDon
struct Invoice {
    string id;
    string memberId;
    string memberName;
    string employeeId;
    string date;
    vector<InvoiceItem> items;
    double totalAmount;
    string paymentMethod;
};

struct Payment {
    string id;
    string memberId;
    string date;
    double amount;
    string method;
};

// 3. LỚP DATAMANAGER (ADAPTER)
class DataManager {
public:
    DataManager();
    ~DataManager();

    // Tải và Lưu tất cả dữ liệu bằng cách gọi QuanLy
    void loadAllData(const string& folderPath = "data");
    void saveAllData(const string& folderPath = "data");

    // ========== QUẢN LÝ THÀNH VIÊN (HỘI VIÊN) ==========
    vector<Member> getAllMembers() const;
    vector<Member> searchMembers(const string& query);
    bool addMember(const string& name, const string& phone, const string& email,
                   MembershipType type, int months);
    bool deleteMember(const string& id);
    int getActiveMemberCount() const;
    int getExpiredMemberCount() const; 
    string getMembershipTypeName(MembershipType type) const;

    // ========== QUẢN LÝ SẢN PHẨM (HÀNG HÓA) ==========
    vector<Product> getAllProducts() const;
    vector<Product> searchProducts(const string& query);
    Product* getProduct(const std::string& id); 
    bool addProduct(const string& name, double price, int quantity, const string& category);
    bool deleteProduct(const string& id);
    bool updateProduct(const string& id, const string& name, double price, int quantity, const string& category);
    int getTotalProducts() const;

    // ========== PHẦN BỊ THIẾU CỦA BẠN (ĐÃ BỔ SUNG) ==========
    vector<Employee> getAllEmployees() const;
    vector<Employee> searchEmployees(const string& query);
    bool addEmployee(const string& name, const string& phone,
                      const string& position, double salary);
    bool deleteEmployee(const string& id);
    bool updateEmployee(const string& id, const string& name, const string& phone,
                       const string& position, double salary);
    // ========================================================

    // ========== QUẢN LÝ HUẤN LUYỆN VIÊN (HLV) ==========
    vector<Trainer> getAllTrainers() const;
    vector<Trainer> searchTrainers(const string& query);
    bool addTrainer(const string& name, const string& phone, const string& specialty, double hourlyRate, int experience);
    bool deleteTrainer(const string& id);
    bool updateTrainer(const string& id, const string& name, const string& phone, const string& specialty, double hourlyRate, int experience);

    // ========== QUẢN LÝ HÓA ĐƠN (HOADON) ==========
    double getTotalRevenue() const;

private:
    // Con trỏ duy nhất trỏ đến toàn bộ hệ thống backend
    unique_ptr<QuanLy> m_quanLy; // <-- Chú ý 'L' viết hoa

    // ----- CÁC HÀM CHUYỂN ĐỔI (TRÁI TIM CỦA ADAPTER) -----
    Member convertToMember(const HoiVien* hv) const;
    Product convertToProduct(const HangHoa* hh) const;
    Employee convertToEmployee(const NhanVien* nv) const;
    Trainer convertToTrainer(const HLV* hlv) const;

    // ----- HÀM HỖ TRỢ -----
    string toLower(const string& str) const;
    string getCurrentDate() const;
    string calculateEndDate(int months) const;
    
    mutable vector<Product> m_productCache;
};

#endif // DATAMANAGER_H