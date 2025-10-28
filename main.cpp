#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace sf;

// ============================================================================
// ENUMS & STRUCTS
// ============================================================================
enum class UserRole { ADMIN, STAFF };
enum class MembershipType { BASIC, PREMIUM, VIP };

struct User {
    string username;
    string password;
    UserRole role;
};

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

struct Product {
    string id;
    string name;
    double price;
    int quantity;
    string category;
};

struct Employee {
    string id;
    string name;
    string phone;
    string position;
    double salary;
    string startDate;
};

struct Trainer {
    string id;
    string name;
    string phone;
    string specialty;
    double hourlyRate;
    int experience; // years
};

struct InvoiceItem {
    string productId;
    string productName;
    int quantity;
    double price;
};

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

// ============================================================================
// DATA MANAGER
// ============================================================================
class DataManager {
private:
    vector<Member> members;
    vector<Payment> payments;
    vector<Product> products;
    vector<Employee> employees;
    vector<Trainer> trainers;
    vector<Invoice> invoices;
    int nextMemberId, nextPaymentId, nextProductId, nextEmployeeId, nextTrainerId, nextInvoiceId;

public:
    DataManager() : nextMemberId(1), nextPaymentId(1), nextProductId(1), 
                    nextEmployeeId(1), nextTrainerId(1), nextInvoiceId(1) {
        loadMembers();
        loadPayments();
        loadProducts();
        loadEmployees();
        loadTrainers();
        loadInvoices();
    }

    // ========== MEMBER MANAGEMENT ==========
    string addMember(const string& name, const string& phone, const string& email, 
                     MembershipType type, int months) {
        Member m;
        m.id = "M" + to_string(nextMemberId++);
        m.name = name;
        m.phone = phone;
        m.email = email;
        m.type = type;
        m.startDate = getCurrentDate();
        m.endDate = calculateEndDate(months);
        m.isActive = true;
        
        members.push_back(m);
        saveMembers();
        return m.id;
    }

    bool deleteMember(const string& id) {
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].id == id) {
                members.erase(members.begin() + i);
                saveMembers();
                return true;
            }
        }
        return false;
    }

    vector<Member> searchMembers(const string& query) {
        vector<Member> results;
        string lowerQuery = toLower(query);
        
        for (const auto& m : members) {
            if (toLower(m.name).find(lowerQuery) != string::npos ||
                toLower(m.phone).find(lowerQuery) != string::npos ||
                toLower(m.id).find(lowerQuery) != string::npos) {
                results.push_back(m);
            }
        }
        return results;
    }

    const vector<Member>& getAllMembers() const { return members; }

    // ========== PRODUCT MANAGEMENT ==========
    string addProduct(const string& name, double price, int quantity, const string& category) {
        Product p;
        p.id = "P" + to_string(nextProductId++);
        p.name = name;
        p.price = price;
        p.quantity = quantity;
        p.category = category;
        
        products.push_back(p);
        saveProducts();
        return p.id;
    }

    bool updateProduct(const string& id, const string& name, double price, 
                      int quantity, const string& category) {
        for (auto& p : products) {
            if (p.id == id) {
                p.name = name;
                p.price = price;
                p.quantity = quantity;
                p.category = category;
                saveProducts();
                return true;
            }
        }
        return false;
    }

    bool deleteProduct(const string& id) {
        for (size_t i = 0; i < products.size(); i++) {
            if (products[i].id == id) {
                products.erase(products.begin() + i);
                saveProducts();
                return true;
            }
        }
        return false;
    }

    vector<Product> searchProducts(const string& query) {
        vector<Product> results;
        string lowerQuery = toLower(query);
        
        for (const auto& p : products) {
            if (toLower(p.name).find(lowerQuery) != string::npos ||
                toLower(p.id).find(lowerQuery) != string::npos ||
                toLower(p.category).find(lowerQuery) != string::npos) {
                results.push_back(p);
            }
        }
        return results;
    }

    const vector<Product>& getAllProducts() const { return products; }

    Product* getProduct(const string& id) {
        for (auto& p : products) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }

    // ========== EMPLOYEE MANAGEMENT ==========
    string addEmployee(const string& name, const string& phone, 
                      const string& position, double salary) {
        Employee e;
        e.id = "E" + to_string(nextEmployeeId++);
        e.name = name;
        e.phone = phone;
        e.position = position;
        e.salary = salary;
        e.startDate = getCurrentDate();
        
        employees.push_back(e);
        saveEmployees();
        return e.id;
    }

    bool updateEmployee(const string& id, const string& name, const string& phone,
                       const string& position, double salary) {
        for (auto& e : employees) {
            if (e.id == id) {
                e.name = name;
                e.phone = phone;
                e.position = position;
                e.salary = salary;
                saveEmployees();
                return true;
            }
        }
        return false;
    }

    bool deleteEmployee(const string& id) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i].id == id) {
                employees.erase(employees.begin() + i);
                saveEmployees();
                return true;
            }
        }
        return false;
    }

    vector<Employee> searchEmployees(const string& query) {
        vector<Employee> results;
        string lowerQuery = toLower(query);
        
        for (const auto& e : employees) {
            if (toLower(e.name).find(lowerQuery) != string::npos ||
                toLower(e.phone).find(lowerQuery) != string::npos ||
                toLower(e.id).find(lowerQuery) != string::npos) {
                results.push_back(e);
            }
        }
        return results;
    }

    const vector<Employee>& getAllEmployees() const { return employees; }

    // ========== TRAINER MANAGEMENT ==========
    string addTrainer(const string& name, const string& phone, const string& specialty,
                     double hourlyRate, int experience) {
        Trainer t;
        t.id = "T" + to_string(nextTrainerId++);
        t.name = name;
        t.phone = phone;
        t.specialty = specialty;
        t.hourlyRate = hourlyRate;
        t.experience = experience;
        
        trainers.push_back(t);
        saveTrainers();
        return t.id;
    }

    bool updateTrainer(const string& id, const string& name, const string& phone,
                      const string& specialty, double hourlyRate, int experience) {
        for (auto& t : trainers) {
            if (t.id == id) {
                t.name = name;
                t.phone = phone;
                t.specialty = specialty;
                t.hourlyRate = hourlyRate;
                t.experience = experience;
                saveTrainers();
                return true;
            }
        }
        return false;
    }

    bool deleteTrainer(const string& id) {
        for (size_t i = 0; i < trainers.size(); i++) {
            if (trainers[i].id == id) {
                trainers.erase(trainers.begin() + i);
                saveTrainers();
                return true;
            }
        }
        return false;
    }

    vector<Trainer> searchTrainers(const string& query) {
        vector<Trainer> results;
        string lowerQuery = toLower(query);
        
        for (const auto& t : trainers) {
            if (toLower(t.name).find(lowerQuery) != string::npos ||
                toLower(t.phone).find(lowerQuery) != string::npos ||
                toLower(t.id).find(lowerQuery) != string::npos ||
                toLower(t.specialty).find(lowerQuery) != string::npos) {
                results.push_back(t);
            }
        }
        return results;
    }

    const vector<Trainer>& getAllTrainers() const { return trainers; }

    // ========== INVOICE MANAGEMENT ==========
    string createInvoice(const string& memberId, const string& memberName, const string& employeeId) {
        Invoice inv;
        inv.id = "INV" + to_string(nextInvoiceId++);
        inv.memberId = memberId;
        inv.memberName = memberName;
        inv.employeeId = employeeId;
        inv.date = getCurrentDate();
        inv.totalAmount = 0;
        inv.paymentMethod = "Cash";
        
        invoices.push_back(inv);
        saveInvoices();
        return inv.id;
    }

    bool addItemToInvoice(const string& invoiceId, const string& productId, int quantity) {
        for (auto& inv : invoices) {
            if (inv.id == invoiceId) {
                Product* prod = getProduct(productId);
                if (!prod || prod->quantity < quantity) return false;
                
                InvoiceItem item;
                item.productId = productId;
                item.productName = prod->name;
                item.quantity = quantity;
                item.price = prod->price;
                
                inv.items.push_back(item);
                inv.totalAmount += item.price * item.quantity;
                
                // Update product quantity
                prod->quantity -= quantity;
                saveProducts();
                saveInvoices();
                return true;
            }
        }
        return false;
    }

    bool finalizeInvoice(const string& invoiceId, const string& paymentMethod) {
        for (auto& inv : invoices) {
            if (inv.id == invoiceId) {
                inv.paymentMethod = paymentMethod;
                saveInvoices();
                return true;
            }
        }
        return false;
    }

    vector<Invoice> searchInvoices(const string& query) {
        vector<Invoice> results;
        string lowerQuery = toLower(query);
        
        for (const auto& inv : invoices) {
            if (toLower(inv.id).find(lowerQuery) != string::npos ||
                toLower(inv.memberName).find(lowerQuery) != string::npos ||
                toLower(inv.memberId).find(lowerQuery) != string::npos) {
                results.push_back(inv);
            }
        }
        return results;
    }

    const vector<Invoice>& getAllInvoices() const { return invoices; }

    // ========== STATISTICS ==========
    int getActiveMemberCount() const {
        return count_if(members.begin(), members.end(), 
                       [](const Member& m) { return m.isActive; });
    }

    int getExpiredMemberCount() const {
        return count_if(members.begin(), members.end(), 
                       [](const Member& m) { return !m.isActive; });
    }

    double getTotalRevenue() const {
        double total = 0;
        for (const auto& inv : invoices) {
            total += inv.totalAmount;
        }
        return total;
    }

    int getTotalProducts() const {
        int total = 0;
        for (const auto& p : products) {
            total += p.quantity;
        }
        return total;
    }

    string getMembershipTypeName(MembershipType type) const {
        switch (type) {
            case MembershipType::BASIC: return "Basic";
            case MembershipType::PREMIUM: return "Premium";
            case MembershipType::VIP: return "VIP";
            default: return "Unknown";
        }
    }

    const vector<Payment>& getAllPayments() const { return payments; }

private:
    string getCurrentDate() const {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream ss;
        ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
           << setw(2) << (1 + ltm->tm_mon) << "/"
           << (1900 + ltm->tm_year);
        return ss.str();
    }

    string calculateEndDate(int months) const {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        ltm->tm_mon += months;
        mktime(ltm);
        
        stringstream ss;
        ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
           << setw(2) << (1 + ltm->tm_mon) << "/"
           << (1900 + ltm->tm_year);
        return ss.str();
    }

    string toLower(const string& str) const {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // ========== FILE I/O ==========
    void saveMembers() {
        ofstream file("members.dat");
        for (const auto& m : members) {
            file << m.id << ","  << m.name << "," << m.phone << "," 
                 << m.email << "," << static_cast<int>(m.type) << ","
                 << m.startDate << "," << m.endDate << "," << m.isActive << "\n";
        }
        file.close();
    }

    void loadMembers() {
        ifstream file("members.dat");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Member m;
            string typeStr, activeStr;
            
            getline(ss, m.id, ',');
            getline(ss, m.name, ',');
            getline(ss, m.phone, ',');
            getline(ss, m.email, ',');
            getline(ss, typeStr, ',');
            getline(ss, m.startDate, ',');
            getline(ss, m.endDate, ',');
            getline(ss, activeStr);
            
            m.type = static_cast<MembershipType>(stoi(typeStr));
            m.isActive = (activeStr == "1");
            members.push_back(m);
            
            int id = stoi(m.id.substr(1));
            if (id >= nextMemberId) nextMemberId = id + 1;
        }
        file.close();
    }

    void saveProducts() {
        ofstream file("products.dat");
        for (const auto& p : products) {
            file << p.id << "," << p.name << "," << p.price << ","
                 << p.quantity << "," << p.category << "\n";
        }
        file.close();
    }

    void loadProducts() {
        ifstream file("products.dat");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Product p;
            string priceStr, qtyStr;
            
            getline(ss, p.id, ',');
            getline(ss, p.name, ',');
            getline(ss, priceStr, ',');
            getline(ss, qtyStr, ',');
            getline(ss, p.category);
            
            p.price = stod(priceStr);
            p.quantity = stoi(qtyStr);
            products.push_back(p);
            
            int id = stoi(p.id.substr(1));
            if (id >= nextProductId) nextProductId = id + 1;
        }
        file.close();
    }

    void saveEmployees() {
        ofstream file("employees.dat");
        for (const auto& e : employees) {
            file << e.id << "," << e.name << "," << e.phone << ","
                 << e.position << "," << e.salary << "," << e.startDate << "\n";
        }
        file.close();
    }

    void loadEmployees() {
        ifstream file("employees.dat");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Employee e;
            string salaryStr;
            
            getline(ss, e.id, ',');
            getline(ss, e.name, ',');
            getline(ss, e.phone, ',');
            getline(ss, e.position, ',');
            getline(ss, salaryStr, ',');
            getline(ss, e.startDate);
            
            e.salary = stod(salaryStr);
            employees.push_back(e);
            
            int id = stoi(e.id.substr(1));
            if (id >= nextEmployeeId) nextEmployeeId = id + 1;
        }
        file.close();
    }

    void saveTrainers() {
        ofstream file("trainers.dat");
        for (const auto& t : trainers) {
            file << t.id << "," << t.name << "," << t.phone << ","
                 << t.specialty << "," << t.hourlyRate << "," << t.experience << "\n";
        }
        file.close();
    }

    void loadTrainers() {
        ifstream file("trainers.dat");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Trainer t;
            string rateStr, expStr;
            
            getline(ss, t.id, ',');
            getline(ss, t.name, ',');
            getline(ss, t.phone, ',');
            getline(ss, t.specialty, ',');
            getline(ss, rateStr, ',');
            getline(ss, expStr);
            
            t.hourlyRate = stod(rateStr);
            t.experience = stoi(expStr);
            trainers.push_back(t);
            
            int id = stoi(t.id.substr(1));
            if (id >= nextTrainerId) nextTrainerId = id + 1;
        }
        file.close();
    }

    void saveInvoices() {
        ofstream file("invoices.dat");
        for (const auto& inv : invoices) {
            file << inv.id << "," << inv.memberId << "," << inv.memberName << ","
                 << inv.employeeId << "," << inv.date << "," << inv.totalAmount << ","
                 << inv.paymentMethod << "," << inv.items.size() << "\n";
            
            for (const auto& item : inv.items) {
                file << "ITEM," << item.productId << "," << item.productName << ","
                     << item.quantity << "," << item.price << "\n";
            }
        }
        file.close();
    }

    void loadInvoices() {
        ifstream file("invoices.dat");
        if (!file.is_open()) return;

        string line;
        Invoice currentInv;
        bool readingInvoice = false;
        
        while (getline(file, line)) {
            if (line.substr(0, 4) == "ITEM") {
                stringstream ss(line);
                string prefix;
                InvoiceItem item;
                string qtyStr, priceStr;
                
                getline(ss, prefix, ',');
                getline(ss, item.productId, ',');
                getline(ss, item.productName, ',');
                getline(ss, qtyStr, ',');
                getline(ss, priceStr);
                
                item.quantity = stoi(qtyStr);
                item.price = stod(priceStr);
                currentInv.items.push_back(item);
            } else {
                if (readingInvoice) {
                    invoices.push_back(currentInv);
                    currentInv = Invoice();
                }
                
                stringstream ss(line);
                string totalStr, itemCountStr;
                
                getline(ss, currentInv.id, ',');
                getline(ss, currentInv.memberId, ',');
                getline(ss, currentInv.memberName, ',');
                getline(ss, currentInv.employeeId, ',');
                getline(ss, currentInv.date, ',');
                getline(ss, totalStr, ',');
                getline(ss, currentInv.paymentMethod, ',');
                getline(ss, itemCountStr);
                
                currentInv.totalAmount = stod(totalStr);
                readingInvoice = true;
                
                int id = stoi(currentInv.id.substr(3));
                if (id >= nextInvoiceId) nextInvoiceId = id + 1;
            }
        }
        
        if (readingInvoice) {
            invoices.push_back(currentInv);
        }
        
        file.close();
    }

    void savePayments() {
        ofstream file("payments.dat");
        for (const auto& p : payments) {
            file << p.id << "," << p.memberId << "," << p.date << ","
                 << p.amount << "," << p.method << "\n";
        }
        file.close();
    }

    void loadPayments() {
        ifstream file("payments.dat");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Payment p;
            string amountStr;
            
            getline(ss, p.id, ',');
            getline(ss, p.memberId, ',');
            getline(ss, p.date, ',');
            getline(ss, amountStr, ',');
            getline(ss, p.method);
            
            p.amount = stod(amountStr);
            payments.push_back(p);
            
            int id = stoi(p.id.substr(1));
            if (id >= nextPaymentId) nextPaymentId = id + 1;
        }
        file.close();
    }
};

// ============================================================================
// LOGIN SYSTEM
// ============================================================================
class LoginSystem {
private:
    map<string, User> users;
    string dataFile = "users.dat";

public:
    LoginSystem() {
        loadUsersFromFile();
        if (users.empty()) {
            users["admin"] = {"admin", "admin123", UserRole::ADMIN};
            users["staff"] = {"staff", "staff123", UserRole::STAFF};
            saveUsersToFile();
        }
    }

    bool login(const string& username, const string& password) {
        if (users.find(username) != users.end() && 
            users[username].password == password) {
            return true;
        }
        return false;
    }

    User getUser(const string& username) {
        return users[username];
    }

    bool addStaffAccount(const string& adminUser, const string& newUsername, 
                        const string& newPassword) {
        if (users[adminUser].role != UserRole::ADMIN) return false;
        if (users.find(newUsername) != users.end()) return false;

        users[newUsername] = {newUsername, newPassword, UserRole::STAFF};
        saveUsersToFile();
        return true;
    }

    bool changePassword(const string& username, const string& oldPassword, 
                       const string& newPassword) {
        if (users.find(username) != users.end() && 
            users[username].password == oldPassword) {
            users[username].password = newPassword;
            saveUsersToFile();
            return true;
        }
        return false;
    }

    bool isAdmin(const string& username) {
        return users[username].role == UserRole::ADMIN;
    }

    vector<string> getAllStaffUsernames() {
        vector<string> staffUsers;
        for (const auto& [username, user] : users) {
            if (user.role == UserRole::STAFF) {
                staffUsers.push_back(username);
            }
        }
        return staffUsers;
    }

private:
    void saveUsersToFile() {
        ofstream file(dataFile);
        for (const auto& [username, user] : users) {
            file << username << "," << user.password << "," 
                 << (user.role == UserRole::ADMIN ? "admin" : "staff") << "\n";
        }
        file.close();
    }

    void loadUsersFromFile() {
        ifstream file(dataFile);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, password, roleStr;
            
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, roleStr);

            UserRole role = (roleStr == "admin") ? UserRole::ADMIN : UserRole::STAFF;
            users[username] = {username, password, role};
        }
        file.close();
    }
};

// ============================================================================
// UI COMPONENTS
// ============================================================================
class ModernInputBox {
private:
    RectangleShape box;
    Text text, label, placeholder;
    string inputText;
    bool isActive, isPassword;
    int cursorPosition;
    float accumulatedTime;
    bool cursorVisible, showPlaceholder;

public:
    ModernInputBox(const Font& font, const string& labelStr, const string& placeholderStr, 
                   float x, float y, float width = 350, float height = 50, bool password = false) 
        : inputText(""), isActive(false), isPassword(password), cursorPosition(0), 
          accumulatedTime(0), cursorVisible(true), showPlaceholder(true),
          text(font, "", 18), label(font, labelStr, 16), placeholder(font, placeholderStr, 18)
    {
        box.setSize(Vector2f(width, height));
        box.setPosition({x, y});
        box.setFillColor(Color(240, 240, 240));
        box.setOutlineColor(Color(200, 200, 200));
        box.setOutlineThickness(2);

        label.setFillColor(Color(100, 100, 100));
        label.setPosition({x, y - 25});

        text.setFillColor(Color(50, 50, 50));
        text.setPosition({x + 15, y + 15});

        placeholder.setFillColor(Color(150, 150, 150));
        placeholder.setPosition({x + 15, y + 15});
    }

    void handleEvent(const Event& event) {
        if (event.is<Event::MouseButtonPressed>()) {
            auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
            if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
                Vector2f mousePos(static_cast<float>(mouseEvent->position.x), 
                                static_cast<float>(mouseEvent->position.y));
                bool wasActive = isActive;
                isActive = box.getGlobalBounds().contains(mousePos);
                
                if (isActive) {
                    box.setOutlineColor(Color(70, 130, 180));
                    box.setFillColor(Color(250, 250, 250));
                    showPlaceholder = false;
                    if (!wasActive) cursorPosition = inputText.length();
                } else {
                    box.setOutlineColor(Color(200, 200, 200));
                    box.setFillColor(Color(240, 240, 240));
                    showPlaceholder = inputText.empty();
                }
            }
        }
        else if (event.is<Event::TextEntered>() && isActive) {
            auto* textEvent = event.getIf<Event::TextEntered>();
            if (textEvent->unicode >= 32 && textEvent->unicode < 127) {
                inputText.insert(cursorPosition, 1, static_cast<char>(textEvent->unicode));
                cursorPosition++;
                showPlaceholder = false;
            }
        }
        else if (event.is<Event::KeyPressed>() && isActive) {
            auto* keyEvent = event.getIf<Event::KeyPressed>();
            switch (keyEvent->code) {
                case Keyboard::Key::Backspace:
                    if (cursorPosition > 0) {
                        inputText.erase(cursorPosition - 1, 1);
                        cursorPosition--;
                    }
                    showPlaceholder = inputText.empty();
                    break;
                case Keyboard::Key::Delete:
                    if (cursorPosition < inputText.length()) {
                        inputText.erase(cursorPosition, 1);
                    }
                    break;
                case Keyboard::Key::Left:
                    if (cursorPosition > 0) cursorPosition--;
                    break;
                case Keyboard::Key::Right:
                    if (cursorPosition < inputText.length()) cursorPosition++;
                    break;
                case Keyboard::Key::Home:
                    cursorPosition = 0;
                    break;
                case Keyboard::Key::End:
                    cursorPosition = inputText.length();
                    break;
                default: break;
            }
        }
        updateDisplayText();
    }

    void update(float deltaTime) {
        if (isActive) {
            accumulatedTime += deltaTime;
            if (accumulatedTime >= 0.5f) {
                cursorVisible = !cursorVisible;
                accumulatedTime = 0;
                updateDisplayText();
            }
        }
    }

    void updateDisplayText() {
        string displayText = isPassword ? string(inputText.length(), '*') : inputText;
        if (isActive && cursorVisible) {
            displayText.insert(cursorPosition, "|");
        }
        text.setString(displayText);
    }

    void draw(RenderWindow& window) {
        window.draw(box);
        window.draw(label);
        if (showPlaceholder && inputText.empty()) {
            window.draw(placeholder);
        } else {
            window.draw(text);
        }
    }

    string getText() const { return inputText; }
    void clear() { 
        inputText.clear(); 
        cursorPosition = 0;
        showPlaceholder = true;
        updateDisplayText();
    }
    void setText(const string& t) {
        inputText = t;
        cursorPosition = inputText.length();
        showPlaceholder = false;
        updateDisplayText();
    }
    void setActive(bool active) { 
        isActive = active;
        if (active) {
            box.setOutlineColor(Color(70, 130, 180));
            box.setFillColor(Color(250, 250, 250));
            showPlaceholder = false;
            cursorVisible = true;
        } else {
            box.setOutlineColor(Color(200, 200, 200));
            box.setFillColor(Color(240, 240, 240));
            showPlaceholder = inputText.empty();
        }
        updateDisplayText();
    }
    bool getIsActive() const { return isActive; }
};

class ModernButton {
private:
    RectangleShape box;
    Text text;
    Color normalColor, hoverColor, clickColor;
    bool isHovered, isClicking;

public:
    ModernButton(const Font& font, const string& buttonText, float x, float y, 
                 float width = 200, float height = 50)
        : normalColor(Color(70, 130, 180)), hoverColor(Color(100, 160, 210)), 
          clickColor(Color(50, 110, 160)), isHovered(false), isClicking(false),
          text(font, buttonText, 18)
    {
        box.setSize({width, height});
        box.setPosition({x, y});
        box.setFillColor(normalColor);
        
        text.setFillColor(Color::White);
        FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
        text.setPosition({x + width / 2.f, y + height / 2.f - 3});
    }

    void handleEvent(const Event& event, Vector2f mousePos) {
        bool wasHovered = isHovered;
        isHovered = box.getGlobalBounds().contains(mousePos);
        
        if (isHovered != wasHovered) {
            box.setFillColor(isHovered ? hoverColor : normalColor);
        }

        if (event.is<Event::MouseButtonPressed>() && isHovered) {
            auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
            if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
                box.setFillColor(clickColor);
                isClicking = true;
            }
        }
        
        if (event.is<Event::MouseButtonReleased>()) {
            isClicking = false;
            box.setFillColor(isHovered ? hoverColor : normalColor);
        }
    }

    bool isClicked(const Event& event, Vector2f mousePos) {
        if (event.is<Event::MouseButtonReleased>()) {
            auto* mouseEvent = event.getIf<Event::MouseButtonReleased>();
            if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
                return box.getGlobalBounds().contains(mousePos);
            }
        }
        return false;
    }

    void draw(RenderWindow& window) {
        window.draw(box);
        window.draw(text);
    }

    void setPosition(float x, float y) {
        box.setPosition({x, y});
        Vector2f size = box.getSize();
        FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
        text.setPosition({x + size.x / 2.f, y + size.y / 2.f - 3});
    }
};

class DropdownMenu {
private:
    RectangleShape box;
    Text currentText, label;
    vector<string> options;
    vector<RectangleShape> optionBoxes;
    vector<Text> optionTexts;
    int selectedIndex;
    bool isExpanded;
    Font& font;

public:
    DropdownMenu(Font& f, const string& labelStr, const vector<string>& opts,
                 float x, float y, float width = 350, float height = 50)
        : font(f), options(opts), selectedIndex(0), isExpanded(false),
          currentText(f, opts[0], 18), label(f, labelStr, 16)
    {
        box.setSize({width, height});
        box.setPosition({x, y});
        box.setFillColor(Color(240, 240, 240));
        box.setOutlineColor(Color(200, 200, 200));
        box.setOutlineThickness(2);

        label.setFillColor(Color(100, 100, 100));
        label.setPosition({x, y - 25});

        currentText.setFillColor(Color(50, 50, 50));
        currentText.setPosition({x + 15, y + 15});

        // Create option boxes
        for (size_t i = 0; i < options.size(); i++) {
            RectangleShape optBox({width, height});
            optBox.setPosition({x, y + height * (i + 1)});
            optBox.setFillColor(Color(250, 250, 250));
            optBox.setOutlineColor(Color(200, 200, 200));
            optBox.setOutlineThickness(1);
            optionBoxes.push_back(optBox);

            Text optText(font, options[i], 18);
            optText.setFillColor(Color(50, 50, 50));
            optText.setPosition({x + 15, y + height * (i + 1) + 15});
            optionTexts.push_back(optText);
        }
    }

    void handleEvent(const Event& event, Vector2f mousePos) {
        if (event.is<Event::MouseButtonPressed>()) {
            auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
            if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
                // Check main box click
                if (box.getGlobalBounds().contains(mousePos)) {
                    isExpanded = !isExpanded;
                    return;
                }

                // Check option clicks when expanded
                if (isExpanded) {
                    for (size_t i = 0; i < optionBoxes.size(); i++) {
                        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
                            selectedIndex = i;
                            currentText.setString(options[i]);
                            isExpanded = false;
                            return;
                        }
                    }
                }

                // Click outside - collapse
                isExpanded = false;
            }
        }
    }

    void draw(RenderWindow& window) {
        window.draw(label);
        window.draw(box);
        window.draw(currentText);

        // Draw arrow indicator
        Text arrow(font, isExpanded ? "▲" : "▼", 16);
        arrow.setFillColor(Color(100, 100, 100));
        Vector2f pos = box.getPosition();
        Vector2f size = box.getSize();
        arrow.setPosition({pos.x + size.x - 30, pos.y + 15});
        window.draw(arrow);

        // Draw options if expanded
        if (isExpanded) {
            for (size_t i = 0; i < optionBoxes.size(); i++) {
                window.draw(optionBoxes[i]);
                window.draw(optionTexts[i]);
            }
        }
    }

    int getSelectedIndex() const { return selectedIndex; }
    string getSelectedValue() const { return options[selectedIndex]; }
    void setSelectedIndex(int idx) {
        if (idx >= 0 && idx < options.size()) {
            selectedIndex = idx;
            currentText.setString(options[idx]);
        }
    }
};

// ============================================================================
// MAIN APPLICATION
// ============================================================================
class GymManagementApp {
private:
    RenderWindow window;
    LoginSystem loginSystem;
    DataManager dataManager;
    Font font;
    Clock clock;
    
    // UI State
    bool isLoggedIn;
    string currentUser;
    int currentScreen; // 0=Login, 1=Dashboard, 2=Members, 3=AddMember, 4=Settings
    
    // UI Components
    vector<unique_ptr<ModernInputBox>> loginInputs;
    vector<unique_ptr<ModernButton>> menuButtons;
    vector<unique_ptr<ModernInputBox>> memberInputs;
    unique_ptr<DropdownMenu> membershipDropdown;
    unique_ptr<DropdownMenu> durationDropdown;
    
    // Texts
    Text titleText, messageText, statsText;
    
    // Background
    RectangleShape background, header, sidebar;
    
    // Member list
    vector<Member> searchResults;
    int scrollOffset;

public:
    GymManagementApp() 
        : window(VideoMode({1400, 900}), "GYM Management System - Professional", Style::Close),
          isLoggedIn(false), currentScreen(0), scrollOffset(0),
          titleText(font), messageText(font), statsText(font)
    {
        window.setFramerateLimit(60);
        
        // Load font
        vector<string> fontPaths = {
            "arial.ttf", "C:/Windows/Fonts/arial.ttf"
        };
        
        for (const auto& path : fontPaths) {
            if (font.openFromFile(path)) break;
        }
        
        setupUI();
    }

    void setupUI() {
        // Background
        background.setSize({1400, 900});
        background.setFillColor(Color(245, 247, 250));
        
        header.setSize({1400, 80});
        header.setFillColor(Color(70, 130, 180));
        
        sidebar.setSize({250, 820});
        sidebar.setPosition({0, 80});
        sidebar.setFillColor(Color(50, 60, 70));
        
        setupLoginScreen();
        setupDashboard();
    }

    void setupLoginScreen() {
        loginInputs.clear();
        loginInputs.push_back(make_unique<ModernInputBox>(
            font, "Ten dang nhap", "Nhap ten dang nhap...", 525, 350));
        loginInputs.push_back(make_unique<ModernInputBox>(
            font, "Mat khau", "Nhap mat khau...", 525, 450, 350, 50, true));
        
        if (!loginInputs.empty()) loginInputs[0]->setActive(true);
    }

    void setupDashboard() {
        menuButtons.clear();
        
        vector<string> menus = {
            "Dashboard",
            "Quan Ly Thanh Vien", 
            "Them Thanh Vien",
            "Thong Ke",
            "Cai Dat",
            "Dang Xuat"
        };
        
        for (size_t i = 0; i < menus.size(); i++) {
            menuButtons.push_back(make_unique<ModernButton>(
                font, menus[i], 20, 100 + i * 70, 210, 50));
        }
    }

    void setupAddMemberScreen() {
        memberInputs.clear();
        memberInputs.push_back(make_unique<ModernInputBox>(
            font, "Ho va ten", "Nhap ho ten...", 300, 150, 400, 50));
        memberInputs.push_back(make_unique<ModernInputBox>(
            font, "So dien thoai", "Nhap so dien thoai...", 300, 240, 400, 50));
        memberInputs.push_back(make_unique<ModernInputBox>(
            font, "Email", "Nhap email...", 300, 330, 400, 50));
        
        membershipDropdown = make_unique<DropdownMenu>(
            font, "Loai the", vector<string>{"Basic - 500K", "Premium - 1M", "VIP - 2M"},
            300, 420, 400, 50);
        
        durationDropdown = make_unique<DropdownMenu>(
            font, "Thoi han", vector<string>{"1 thang", "3 thang", "6 thang", "12 thang"},
            300, 510, 400, 50);
            
        if (!memberInputs.empty()) memberInputs[0]->setActive(true);
    }

    void run() {
        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            handleEvents();
            update(dt);
            render();
        }
    }

    void handleEvents() {
        Vector2f mousePos(Mouse::getPosition(window));

        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
            
            if (!isLoggedIn) {
                handleLoginEvents(*event, mousePos);
            } else {
                handleMainEvents(*event, mousePos);
            }
        }
    }

    void handleLoginEvents(const Event& event, Vector2f mousePos) {
        for (auto& input : loginInputs) {
            input->handleEvent(event);
        }
        
        if (event.is<Event::KeyPressed>()) {
            auto* keyEvent = event.getIf<Event::KeyPressed>();
            if (keyEvent->code == Keyboard::Key::Enter) {
                attemptLogin();
            }
        }
        
        // Login button
        static ModernButton loginBtn(font, "DANG NHAP", 575, 550, 250, 55);
        loginBtn.handleEvent(event, mousePos);
        if (loginBtn.isClicked(event, mousePos)) {
            attemptLogin();
        }
    }

    void handleMainEvents(const Event& event, Vector2f mousePos) {
        // Handle menu buttons
        for (size_t i = 0; i < menuButtons.size(); i++) {
            menuButtons[i]->handleEvent(event, mousePos);
            if (menuButtons[i]->isClicked(event, mousePos)) {
                handleMenuClick(i);
            }
        }
        
        // Handle screen-specific events
        switch (currentScreen) {
            case 2: // Members list
                handleMembersScreenEvents(event, mousePos);
                break;
            case 3: // Add member
                handleAddMemberEvents(event, mousePos);
                break;
            case 5: // Settings
                handleSettingsEvents(event, mousePos);
                break;
        }
    }

    void handleMembersScreenEvents(const Event& event, Vector2f mousePos) {
        static unique_ptr<ModernInputBox> searchBox;
        if (!searchBox) {
            searchBox = make_unique<ModernInputBox>(
                font, "Tim kiem", "Nhap ten, SDT hoac ma...", 300, 120, 500, 50);
        }
        
        searchBox->handleEvent(event);
        
        if (event.is<Event::KeyPressed>()) {
            auto* keyEvent = event.getIf<Event::KeyPressed>();
            if (keyEvent->code == Keyboard::Key::Enter) {
                string query = searchBox->getText();
                if (query.empty()) {
                    searchResults = dataManager.getAllMembers();
                } else {
                    searchResults = dataManager.searchMembers(query);
                }
            }
        }
        
        // Scroll handling
        if (event.is<Event::MouseWheelScrolled>()) {
            auto* scrollEvent = event.getIf<Event::MouseWheelScrolled>();
            scrollOffset -= static_cast<int>(scrollEvent->delta * 30);
            if (scrollOffset < 0) scrollOffset = 0;
        }
    }

    void handleAddMemberEvents(const Event& event, Vector2f mousePos) {
        for (auto& input : memberInputs) {
            input->handleEvent(event);
        }
        
        if (membershipDropdown) {
            membershipDropdown->handleEvent(event, mousePos);
        }
        
        if (durationDropdown) {
            durationDropdown->handleEvent(event, mousePos);
        }
        
        static ModernButton saveBtn(font, "LUU THONG TIN", 300, 600, 200, 50);
        static ModernButton cancelBtn(font, "HUY", 520, 600, 200, 50);
        
        saveBtn.handleEvent(event, mousePos);
        cancelBtn.handleEvent(event, mousePos);
        
        if (saveBtn.isClicked(event, mousePos)) {
            addNewMember();
        }
        
        if (cancelBtn.isClicked(event, mousePos)) {
            currentScreen = 1;
        }
    }

    void handleSettingsEvents(const Event& event, Vector2f mousePos) {
        static vector<unique_ptr<ModernInputBox>> settingsInputs;
        if (settingsInputs.empty()) {
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Mat khau cu", "Nhap mat khau cu...", 300, 200, 400, 50, true));
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Mat khau moi", "Nhap mat khau moi...", 300, 290, 400, 50, true));
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Xac nhan mat khau", "Nhap lai mat khau moi...", 300, 380, 400, 50, true));
        }
        
        for (auto& input : settingsInputs) {
            input->handleEvent(event);
        }
        
        static ModernButton changeBtn(font, "DOI MAT KHAU", 300, 470, 200, 50);
        changeBtn.handleEvent(event, mousePos);
        
        if (changeBtn.isClicked(event, mousePos)) {
            string oldPass = settingsInputs[0]->getText();
            string newPass = settingsInputs[1]->getText();
            string confirm = settingsInputs[2]->getText();
            
            if (newPass != confirm) {
                setMessage("Mat khau xac nhan khong khop!", Color(220, 80, 80));
            } else if (loginSystem.changePassword(currentUser, oldPass, newPass)) {
                setMessage("Doi mat khau thanh cong!", Color(80, 180, 80));
                for (auto& input : settingsInputs) input->clear();
            } else {
                setMessage("Mat khau cu khong dung!", Color(220, 80, 80));
            }
        }
    }

    void handleMenuClick(int index) {
        switch (index) {
            case 0: // Dashboard
                currentScreen = 1;
                break;
            case 1: // Members
                currentScreen = 2;
                searchResults = dataManager.getAllMembers();
                break;
            case 2: // Add member
                currentScreen = 3;
                setupAddMemberScreen();
                break;
            case 3: // Statistics
                currentScreen = 4;
                break;
            case 4: // Settings
                currentScreen = 5;
                break;
            case 5: // Logout
                logout();
                break;
        }
    }

    void update(float dt) {
        if (!isLoggedIn) {
            for (auto& input : loginInputs) {
                input->update(dt);
            }
        } else {
            for (auto& input : memberInputs) {
                input->update(dt);
            }
        }
    }

    void render() {
        window.clear(Color::White);
        window.draw(background);
        window.draw(header);

        if (!isLoggedIn) {
            renderLogin();
        } else {
            renderMain();
        }

        window.display();
    }

    void renderLogin() {
        // Title
        Text title(font, "GYM MANAGEMENT SYSTEM", 48);
        title.setFillColor(Color::White);
        title.setStyle(Text::Bold);
        FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
        title.setPosition({700, 40});
        window.draw(title);
        
        // Login panel
        RectangleShape loginPanel({450, 400});
        loginPanel.setPosition({475, 250});
        loginPanel.setFillColor(Color(255, 255, 255));
        loginPanel.setOutlineColor(Color(200, 200, 200));
        loginPanel.setOutlineThickness(2);
        window.draw(loginPanel);
        
        Text loginTitle(font, "DANG NHAP HE THONG", 28);
        loginTitle.setFillColor(Color(70, 130, 180));
        loginTitle.setStyle(Text::Bold);
        loginTitle.setPosition({550, 280});
        window.draw(loginTitle);
        
        // Inputs
        for (auto& input : loginInputs) {
            input->draw(window);
        }
        
        // Login button
        static ModernButton loginBtn(font, "DANG NHAP", 575, 550, 250, 55);
        loginBtn.draw(window);
        
        // Message
        Text msg(font, "Tai khoan mac dinh: admin/admin123 hoac staff/staff123", 14);
        msg.setFillColor(Color(150, 150, 150));
        msg.setPosition({475, 670});
        window.draw(msg);
        
        // Display message text
        window.draw(messageText);
    }

    void renderMain() {
        // Sidebar
        window.draw(sidebar);
        
        // Menu buttons
        for (auto& btn : menuButtons) {
            btn->draw(window);
        }
        
        // Header title
        Text headerTitle(font, "GYM MANAGEMENT SYSTEM", 32);
        headerTitle.setFillColor(Color::White);
        headerTitle.setStyle(Text::Bold);
        headerTitle.setPosition({280, 25});
        window.draw(headerTitle);
        
        // User info
        Text userInfo(font, "Dang nhap: " + currentUser + 
                     (loginSystem.isAdmin(currentUser) ? " (Admin)" : " (Staff)"), 18);
        userInfo.setFillColor(Color(220, 220, 220));
        userInfo.setPosition({1150, 35});
        window.draw(userInfo);
        
        // Content area
        switch (currentScreen) {
            case 1: renderDashboard(); break;
            case 2: renderMembersScreen(); break;
            case 3: renderAddMemberScreen(); break;
            case 4: renderStatisticsScreen(); break;
            case 5: renderSettingsScreen(); break;
        }
        
        // Message
        window.draw(messageText);
    }

    void renderDashboard() {
        Text title(font, "DASHBOARD - TONG QUAN", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 120});
        window.draw(title);
        
        // Stats cards
        int activeMembers = dataManager.getActiveMemberCount();
        int expiredMembers = dataManager.getExpiredMemberCount();
        double revenue = dataManager.getTotalRevenue();
        
        renderStatCard("THANH VIEN HOAT DONG", to_string(activeMembers), 
                      300, 200, Color(80, 180, 100));
        renderStatCard("THANH VIEN HET HAN", to_string(expiredMembers), 
                      580, 200, Color(220, 100, 80));
        renderStatCard("DOANH THU", formatMoney(revenue), 
                      860, 200, Color(70, 130, 180));
        
        // Recent members
        Text recentTitle(font, "THANH VIEN MOI NHAT", 24);
        recentTitle.setFillColor(Color(70, 130, 180));
        recentTitle.setStyle(Text::Bold);
        recentTitle.setPosition({300, 380});
        window.draw(recentTitle);
        
        const auto& members = dataManager.getAllMembers();
        int count = min(5, static_cast<int>(members.size()));
        
        for (int i = 0; i < count; i++) {
            int idx = members.size() - 1 - i;
            const auto& m = members[idx];
            
            RectangleShape card({800, 60});
            card.setPosition({300.0f, 430.0f + i * 70.0f});
            card.setFillColor(Color(250, 250, 250));
            card.setOutlineColor(Color(220, 220, 220));
            card.setOutlineThickness(1);
            window.draw(card);
            
            Text nameText(font, m.name, 18);
            nameText.setFillColor(Color(50, 50, 50));
            nameText.setPosition({320.0f, 445.0f + i * 70.0f});
            window.draw(nameText);
            
            Text idText(font, "ID: " + m.id, 16);
            idText.setFillColor(Color(100, 100, 100));
            idText.setPosition({600.0f, 447.0f + i * 70.0f});
            window.draw(idText);
            
            Text typeText(font, dataManager.getMembershipTypeName(m.type), 16);
            typeText.setFillColor(Color(70, 130, 180));
            typeText.setPosition({750.0f, 447.0f + i * 70.0f});
            window.draw(typeText);
            
            Text statusText(font, m.isActive ? "Hoat dong" : "Het han", 16);
            statusText.setFillColor(m.isActive ? Color(80, 180, 100) : Color(220, 100, 80));
            statusText.setPosition({950.0f, 447.0f + i * 70.0f});
            window.draw(statusText);
        }
    }

    void renderMembersScreen() {
        Text title(font, "QUAN LY THANH VIEN", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 100});
        window.draw(title);
        
        // Search box
        static unique_ptr<ModernInputBox> searchBox = make_unique<ModernInputBox>(
            font, "Tim kiem", "Nhap ten, SDT hoac ma...", 300, 160, 500, 50);
        searchBox->draw(window);
        
        // Search button
        static ModernButton searchBtn(font, "TIM KIEM", 820, 160, 150, 50);
        searchBtn.draw(window);
        
        // Table header
        RectangleShape tableHeader({1050, 40});
        tableHeader.setPosition({300, 240});
        tableHeader.setFillColor(Color(70, 130, 180));
        window.draw(tableHeader);
        
        vector<string> headers = {"Ma", "Ho Ten", "SDT", "Loai The", "Ngay Het Han", "Trang Thai"};
        vector<float> positions = {320, 420, 620, 750, 880, 1040};
        
        for (size_t i = 0; i < headers.size(); i++) {
            Text header(font, headers[i], 16);
            header.setFillColor(Color::White);
            header.setStyle(Text::Bold);
            header.setPosition({positions[i], 250});
            window.draw(header);
        }
        
        // Table rows
        float yPos = 290;
        int displayCount = min(10, static_cast<int>(searchResults.size()));
        
        for (int i = 0; i < displayCount; i++) {
            const auto& m = searchResults[i];
            
            RectangleShape row({1050, 50});
            row.setPosition({300, yPos});
            row.setFillColor(i % 2 == 0 ? Color(250, 250, 250) : Color::White);
            row.setOutlineColor(Color(230, 230, 230));
            row.setOutlineThickness(1);
            window.draw(row);
            
            Text idText(font, m.id, 14);
            idText.setFillColor(Color(50, 50, 50));
            idText.setPosition({320, yPos + 18});
            window.draw(idText);
            
            Text nameText(font, m.name, 14);
            nameText.setFillColor(Color(50, 50, 50));
            nameText.setPosition({420, yPos + 18});
            window.draw(nameText);
            
            Text phoneText(font, m.phone, 14);
            phoneText.setFillColor(Color(50, 50, 50));
            phoneText.setPosition({620, yPos + 18});
            window.draw(phoneText);
            
            Text typeText(font, dataManager.getMembershipTypeName(m.type), 14);
            typeText.setFillColor(Color(70, 130, 180));
            typeText.setPosition({750, yPos + 18});
            window.draw(typeText);
            
            Text dateText(font, m.endDate, 14);
            dateText.setFillColor(Color(50, 50, 50));
            dateText.setPosition({880, yPos + 18});
            window.draw(dateText);
            
            Text statusText(font, m.isActive ? "Hoat dong" : "Het han", 14);
            statusText.setFillColor(m.isActive ? Color(80, 180, 100) : Color(220, 100, 80));
            statusText.setPosition({1040, yPos + 18});
            window.draw(statusText);
            
            yPos += 55;
        }
        
        // Total count
        Text totalText(font, "Tong: " + to_string(searchResults.size()) + " thanh vien", 16);
        totalText.setFillColor(Color(100, 100, 100));
        totalText.setPosition({300, 850});
        window.draw(totalText);
    }

    void renderAddMemberScreen() {
        Text title(font, "THEM THANH VIEN MOI", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 100});
        window.draw(title);
        
        // Form panel
        RectangleShape formPanel({600, 550});
        formPanel.setPosition({280, 140});
        formPanel.setFillColor(Color(255, 255, 255));
        formPanel.setOutlineColor(Color(220, 220, 220));
        formPanel.setOutlineThickness(2);
        window.draw(formPanel);
        
        // Inputs
        for (auto& input : memberInputs) {
            input->draw(window);
        }
        
        if (membershipDropdown) {
            membershipDropdown->draw(window);
        }
        
        if (durationDropdown) {
            durationDropdown->draw(window);
        }
        
        // Buttons
        static ModernButton saveBtn(font, "LUU THONG TIN", 300, 600, 200, 50);
        static ModernButton cancelBtn(font, "HUY", 520, 600, 200, 50);
        
        saveBtn.draw(window);
        cancelBtn.draw(window);
    }

    void renderStatisticsScreen() {
        Text title(font, "THONG KE & BAO CAO", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 120});
        window.draw(title);
        
        // Revenue chart (simplified)
        RectangleShape chartBg({800, 400});
        chartBg.setPosition({300, 200});
        chartBg.setFillColor(Color(250, 250, 250));
        chartBg.setOutlineColor(Color(220, 220, 220));
        chartBg.setOutlineThickness(2);
        window.draw(chartBg);
        
        Text chartTitle(font, "BIEU DO DOANH THU", 24);
        chartTitle.setFillColor(Color(70, 130, 180));
        chartTitle.setPosition({500, 220});
        window.draw(chartTitle);
        
        // Simple bar chart
        const auto& payments = dataManager.getAllPayments();
        map<int, double> monthlyRevenue;
        
        for (const auto& p : payments) {
            // Extract month from date (simplified)
            monthlyRevenue[1] += p.amount; // All in month 1 for demo
        }
        
        float barHeight = monthlyRevenue[1] / 10000.0f; // Scale
        RectangleShape bar({60, barHeight});
        bar.setPosition({400, 550 - barHeight});
        bar.setFillColor(Color(70, 130, 180));
        window.draw(bar);
        
        Text barLabel(font, "Thang 1", 14);
        barLabel.setPosition({395, 560});
        window.draw(barLabel);
    }

    void renderSettingsScreen() {
        Text title(font, "CAI DAT HE THONG", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 120});
        window.draw(title);
        
        // Settings panel
        RectangleShape settingsPanel({600, 450});
        settingsPanel.setPosition({280, 180});
        settingsPanel.setFillColor(Color(255, 255, 255));
        settingsPanel.setOutlineColor(Color(220, 220, 220));
        settingsPanel.setOutlineThickness(2);
        window.draw(settingsPanel);
        
        Text sectionTitle(font, "DOI MAT KHAU", 24);
        sectionTitle.setFillColor(Color(70, 130, 180));
        sectionTitle.setPosition({300, 200});
        window.draw(sectionTitle);
        
        static vector<unique_ptr<ModernInputBox>> settingsInputs;
        if (settingsInputs.empty()) {
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Mat khau cu", "Nhap mat khau cu...", 300, 250, 400, 50, true));
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Mat khau moi", "Nhap mat khau moi...", 300, 340, 400, 50, true));
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Xac nhan mat khau", "Nhap lai mat khau moi...", 300, 430, 400, 50, true));
        }
        
        for (auto& input : settingsInputs) {
            input->draw(window);
        }
        
        static ModernButton changeBtn(font, "DOI MAT KHAU", 300, 520, 200, 50);
        changeBtn.draw(window);
    }

    void renderStatCard(const string& label, const string& value, 
                       float x, float y, Color color) {
        RectangleShape card({250, 150});
        card.setPosition({x, y});
        card.setFillColor(color);
        window.draw(card);
        
        Text labelText(font, label, 16);
        labelText.setFillColor(Color(255, 255, 255, 180));
        labelText.setPosition({x + 20, y + 20});
        window.draw(labelText);
        
        Text valueText(font, value, 42);
        valueText.setFillColor(Color::White);
        valueText.setStyle(Text::Bold);
        valueText.setPosition({x + 20, y + 70});
        window.draw(valueText);
    }

    void attemptLogin() {
        string username = loginInputs[0]->getText();
        string password = loginInputs[1]->getText();

        if (username.empty() || password.empty()) {
            setMessage("Vui long nhap day du thong tin!", Color(220, 80, 80));
            return;
        }

        if (loginSystem.login(username, password)) {
            isLoggedIn = true;
            currentUser = username;
            currentScreen = 1;
            setMessage("Dang nhap thanh cong!", Color(80, 180, 80));
        } else {
            setMessage("Sai ten dang nhap hoac mat khau!", Color(220, 80, 80));
        }
    }

    void addNewMember() {
        if (memberInputs.size() < 3) return;
        
        string name = memberInputs[0]->getText();
        string phone = memberInputs[1]->getText();
        string email = memberInputs[2]->getText();
        
        if (name.empty() || phone.empty()) {
            setMessage("Vui long nhap day du thong tin!", Color(220, 80, 80));
            return;
        }
        
        MembershipType type = static_cast<MembershipType>(membershipDropdown->getSelectedIndex());
        int months = membershipDropdown ? 
            (durationDropdown->getSelectedIndex() == 0 ? 1 : 
             durationDropdown->getSelectedIndex() == 1 ? 3 :
             durationDropdown->getSelectedIndex() == 2 ? 6 : 12) : 1;
        
        string id = dataManager.addMember(name, phone, email, type, months);
        setMessage("Them thanh vien thanh cong! Ma: " + id, Color(80, 180, 80));
        
        currentScreen = 1;
    }

    void logout() {
        isLoggedIn = false;
        currentUser = "";
        currentScreen = 0;
        for (auto& input : loginInputs) {
            input->clear();
        }
        setMessage("Da dang xuat!", Color(100, 100, 100));
    }

    void setMessage(const string& msg, Color color = Color(70, 130, 180)) {
        messageText.setString(msg);
        messageText.setCharacterSize(16);
        messageText.setFillColor(color);
        messageText.setPosition({300, 850});
    }

    string formatMoney(double amount) const {
        stringstream ss;
        ss << fixed << setprecision(0) << amount;
        string result = ss.str();
        
        int pos = result.length() - 3;
        while (pos > 0) {
            result.insert(pos, ",");
            pos -= 3;
        }
        return result + " VND";
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    try {
        GymManagementApp app;
        app.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    return 0;
}