// #include "../../Include/Manager/DataManager.h"
// #include <algorithm> // Để dùng std::transform
// #include <ctime>     // Để dùng getCurrentDate
// #include <sstream>   // Để dùng std::stringstream
// #include <iomanip>   // Để dùng std::setfill, std::setw

// // ----- CONSTRUCTOR & DESTRUCTOR -----

// DataManager::DataManager() {
//     // Khởi tạo hệ thống Backend
//     m_quanLy = make_unique<QuanLy>();
    
//     // Tải dữ liệu từ backend (nếu có)
//     loadAllData();
// }

// DataManager::~DataManager() {
//     // Tự động lưu dữ liệu khi DataManager bị hủy
//     saveAllData();
// }

// void DataManager::loadAllData(const string& folderPath) {
//     // m_quanLy->loadAllData(folderPath); // TODO: Triển khai backend
    
//     // Cập nhật cache (nếu cần)
//     m_productCache = getAllProducts();
// }

// void DataManager::saveAllData(const string& folderPath) {
//     // m_quanLy->saveAllData(folderPath); // TODO: Triển khai backend
// }

// // ========== QUẢN LÝ THÀNH VIÊN (HỘI VIÊN) ==========

// vector<Member> DataManager::getAllMembers() const {
//     vector<Member> members;
//     MyVector<HoiVien*> hoivienList = m_quanLy->getDsHoiVien().getAllValues();
    
//     for (size_t i = 0; i < hoivienList.size(); ++i) {
//         members.push_back(convertToMember(hoivienList[i]));
//     }
//     return members;
// }

// vector<Member> DataManager::searchMembers(const string& query) {
//     vector<Member> results;
//     vector<Member> allMembers = getAllMembers();
//     string lowerQuery = toLower(query);

//     for (const auto& m : allMembers) {
//         if (toLower(m.name).find(lowerQuery) != string::npos ||
//             toLower(m.phone).find(lowerQuery) != string::npos ||
//             toLower(m.id).find(lowerQuery) != string::npos) {
//             results.push_back(m);
//         }
//     }
//     return results;
// }

// bool DataManager::addMember(const string& name, const string& phone, const string& email,
//                             MembershipType type, int months) {
//     // TODO: Backend HoiVien::create cần hoTen, sdt, gioiTinh, tuoi, point, hlv
//     HoiVien* hv = HoiVien::create(name, phone, "N/A", 0, 0, nullptr);
    
//     bool success = m_quanLy->addHoiVien(hv);
//     if (!success) {
//         delete hv;
//     }
//     return success;
// }

// bool DataManager::deleteMember(const string& id) {
//     return m_quanLy->removeHoiVien(id);
// }

// int DataManager::getActiveMemberCount() const {
//     return m_quanLy->getDsHoiVien().size();
// }

// int DataManager::getExpiredMemberCount() const {
//     return 0; // TODO: Backend không hỗ trợ
// }

// string DataManager::getMembershipTypeName(MembershipType type) const {
//     switch (type) {
//         case MembershipType::BASIC: return "Basic";
//         case MembershipType::PREMIUM: return "Premium";
//         case MembershipType::VIP: return "VIP";
//         default: return "Unknown";
//     }
// }

// // ========== QUẢN LÝ SẢN PHẨM (HÀNG HÓA) ==========

// vector<Product> DataManager::getAllProducts() const {
//     vector<Product> products;
//     const MyVector<HangHoa*>& hanghoaList = m_quanLy->getDsHangHoa();
    
//     for (size_t i = 0; i < hanghoaList.size(); ++i) {
//         products.push_back(convertToProduct(hanghoaList[i]));
//     }
//     return products;
// }

// vector<Product> DataManager::searchProducts(const string& query) {
//     vector<Product> results;
//     vector<Product> allProducts = getAllProducts();
//     string lowerQuery = toLower(query);

//     for (const auto& p : allProducts) {
//         if (toLower(p.name).find(lowerQuery) != string::npos ||
//             toLower(p.id).find(lowerQuery) != string::npos ||
//             toLower(p.category).find(lowerQuery) != string::npos) {
//             results.push_back(p);
//         }
//     }
//     return results;
// }

// Product* DataManager::getProduct(const std::string& id) {
//     for (auto& p : m_productCache) {
//         if (p.id == id) return &p;
//     }
//     return nullptr;
// }

// bool DataManager::addProduct(const string& name, double price, int quantity, const string& category) {
//     HangHoa* hh = HangHoa::create(name, price);
    
//     bool success = m_quanLy->addHangHoa(hh);
//     if (!success) {
//         delete hh;
//     } else {
//         m_productCache.push_back(convertToProduct(hh));
//     }
//     return success;
// }

// bool DataManager::deleteProduct(const string& id) {
//     for (size_t i = 0; i < m_productCache.size(); ++i) {
//         if (m_productCache[i].id == id) {
//             m_productCache.erase(m_productCache.begin() + i);
//             break;
//         }
//     }
//     return m_quanLy->removeHangHoa(id);
// }

// bool DataManager::updateProduct(const string& id, const string& name, double price,
//                                 int quantity, const string& category) {
//     HangHoa* hh = m_quanLy->getHangHoa(id);
//     if (hh) {
//         hh->setTenHH(name);
//         hh->setGia(price);
        
//         for (auto& p : m_productCache) {
//             if (p.id == id) {
//                 p.name = name; p.price = price;
//                 p.quantity = quantity; p.category = category;
//                 break;
//             }
//         }
//         return true;
//     }
//     return false;
// }

// int DataManager::getTotalProducts() const {
//     return 0; // TODO: Backend không hỗ trợ
// }


// // ========== QUẢN LÝ NHÂN VIÊN (NHÂN VIÊN) ==========

// vector<Employee> DataManager::getAllEmployees() const {
//     vector<Employee> employees;
//     // Chú ý: Dùng m_quanLy (L hoa)
//     const MyVector<NhanVien*>& nhanvienList = m_quanLy->getDsNhanVien();
    
//     for (size_t i = 0; i < nhanvienList.size(); ++i) {
//         employees.push_back(convertToEmployee(nhanvienList[i]));
//     }
//     return employees;
// }

// vector<Employee> DataManager::searchEmployees(const string& query) {
//     vector<Employee> results;
//     vector<Employee> allEmployees = getAllEmployees(); // Hàm này phải có "DataManager::"
//     string lowerQuery = toLower(query); // Hàm này cũng vậy

//     for (const auto& e : allEmployees) {
//         if (toLower(e.name).find(lowerQuery) != string::npos ||
//             toLower(e.phone).find(lowerQuery) != string::npos ||
//             toLower(e.id).find(lowerQuery) != string::npos) {
//             results.push_back(e);
//         }
//     }
//     return results;
// }

// bool DataManager::addEmployee(const string& name, const string& phone,
//                                 const string& position, double salary) {
//     // TODO: Backend NhanVien::create cần hoTen, sdt, gioiTinh, tuoi, luong
//     NhanVien* nv = NhanVien::create(name, phone, "N/A", 0, salary);
    
//     bool success = m_quanLy->addNhanVien(nv);
//     if (!success) {
//         delete nv;
//     }
//     return success;
// }

// bool DataManager::deleteEmployee(const string& id) {
//     return m_quanLy->xoaNhanVien(id);
// }

// bool DataManager::updateEmployee(const string& id, const string& name, const string& phone,
//                                 const string& position, double salary) {
//     NhanVien* nv = m_quanLy->timNhanVien(id);
//     if (nv) {
//         nv->setHoTen(name);
//         nv->setSDT(phone);
//         nv->setLuong(salary);
//         return true;
//     }
//     return false;
// }


// // ========== QUẢN LÝ HUẤN LUYỆN VIÊN (HLV) ==========

// vector<Trainer> DataManager::getAllTrainers() const {
//     vector<Trainer> trainers;
//     const MyVector<HLV*>& hlvList = m_quanLy->getDsHLV();
    
//     for (size_t i = 0; i < hlvList.size(); ++i) {
//         trainers.push_back(convertToTrainer(hlvList[i]));
//     }
//     return trainers;
// }

// vector<Trainer> DataManager::searchTrainers(const string& query) {
//     vector<Trainer> results;
//     vector<Trainer> allTrainers = getAllTrainers();
//     string lowerQuery = toLower(query);

//     for (const auto& t : allTrainers) {
//         if (toLower(t.name).find(lowerQuery) != string::npos ||
//             toLower(t.phone).find(lowerQuery) != string::npos ||
//             toLower(t.id).find(lowerQuery) != string::npos) {
//             results.push_back(t);
//         }
//     }
//     return results;
// }

// bool DataManager::addTrainer(const string& name, const string& phone, const string& specialty,
//                              double hourlyRate, int experience) {
//     // TODO: Backend HLV::create cần hoTen, sdt, gioiTinh, tuoi, luong
//     HLV* hlv = HLV::create(name, phone, "N/A", 0, hourlyRate);
    
//     bool success = m_quanLy->addHLV(hlv);
//     if (!success) {
//         delete hlv;
//     }
//     return success;
// }

// bool DataManager::deleteTrainer(const string& id) {
//     return m_quanLy->xoaHLV(id);
// }

// bool DataManager::updateTrainer(const string& id, const string& name, const string& phone,
//                                 const string& specialty, double hourlyRate, int experience) {
//     HLV* hlv = m_quanLy->timHLV(id);
//     if (hlv) {
//         hlv->setHoTen(name);
//         hlv->setSDT(phone);
//         hlv->setLuong(hourlyRate);
//         return true;
//     }
//     return false;
// }

// // ========== THỐNG KÊ ==========

// double DataManager::getTotalRevenue() const {
//     double total = 0.0;
//     const MyVector<HoaDon*>& invoices = m_quanLy->getDsHoaDon();
//     for(size_t i = 0; i < invoices.size(); ++i) {
//         total += invoices[i]->getTotal();
//     }
//     return total;
// }


// // ----- CÁC HÀM CHUYỂN ĐỔI (TRÁI TIM CỦA ADAPTER) -----

// Member DataManager::convertToMember(const HoiVien* hv) const {
//     if (!hv) return Member();
//     Member m;
//     m.id = hv->getID();
//     m.name = hv->getHoTen();
//     m.phone = hv->getSDT();
//     m.email = "N/A";
//     m.type = MembershipType::BASIC;
//     m.startDate = "N/A";
//     m.endDate = "N/A";
//     m.isActive = true; 
//     return m;
// }

// Product DataManager::convertToProduct(const HangHoa* hh) const {
//     if (!hh) return Product();
//     Product p;
//     p.id = hh->getID();
//     p.name = hh->getTenHH();
//     p.price = hh->getGia();
//     p.quantity = 1; 
//     p.category = "N/A";
//     return p;
// }

// Employee DataManager::convertToEmployee(const NhanVien* nv) const {
//     if (!nv) return Employee();
//     Employee e;
//     e.id = nv->getID();
//     e.name = nv->getHoTen();
//     e.phone = nv->getSDT();
//     e.salary = nv->getLuong();
//     e.position = "Staff";
//     e.startDate = "N/A";
//     return e;
// }

// Trainer DataManager::convertToTrainer(const HLV* hlv) const {
//     if (!hlv) return Trainer();
//     Trainer t;
//     t.id = hlv->getID();
//     t.name = hlv->getHoTen();
//     t.phone = hlv->getSDT();
//     t.hourlyRate = hlv->getLuong();
//     t.specialty = "N/A";
//     t.experience = 0;
//     return t;
// }

// // ----- HÀM HỖ TRỢ -----

// string DataManager::toLower(const string& str) const {
//     string result = str;
//     std::transform(result.begin(), result.end(), result.begin(), ::tolower);
//     return result;
// }

// string DataManager::getCurrentDate() const {
//     time_t now = time(0);
//     tm* ltm = localtime(&now);
//     std::stringstream ss;
//     ss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "/"
//        << std::setw(2) << (1 + ltm->tm_mon) << "/"
//        << (1900 + ltm->tm_year);
//     return ss.str();
// }

// string DataManager::calculateEndDate(int months) const {
//     time_t now = time(0);
//     tm* ltm = localtime(&now);
//     ltm->tm_mon += months;
//     mktime(ltm);
    
//     std::stringstream ss;
//     ss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "/"
//        << std::setw(2) << (1 + ltm->tm_mon) << "/"
//        << (1900 + ltm->tm_year);
//     return ss.str();
// }