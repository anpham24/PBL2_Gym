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
    string dataFile = "users.txt";

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
    RectangleShape toggleButton;
    Text toggleText;
    bool showPassword;

public:
    ModernInputBox(const Font& font, const string& labelStr, const string& placeholderStr, 
                   float x, float y, float width = 350, float height = 50, bool password = false)
        : inputText(""), isActive(false), isPassword(password), cursorPosition(0), 
          accumulatedTime(0), cursorVisible(true), showPlaceholder(true), showPassword(false),
          text(font, "", 18), label(font, labelStr, 16), placeholder(font, placeholderStr, 18),
          toggleButton({30, height - 20}), toggleText(font, "O", 14)
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

        // Setup toggle button for password
        toggleButton.setPosition({x + width - 40, y + 10});
        toggleButton.setFillColor(Color(200, 200, 200));
        toggleButton.setOutlineColor(Color(150, 150, 150));
        toggleButton.setOutlineThickness(1);

        toggleText.setFillColor(Color(100, 100, 100));
        FloatRect toggleBounds = toggleText.getLocalBounds();
        toggleText.setOrigin({toggleBounds.size.x / 2.f, toggleBounds.size.y / 2.f});
        toggleText.setPosition({x + width - 25, y + height / 2.f});
    }

    void handleEvent(const Event& event) {
        if (event.is<Event::MouseButtonPressed>()) {
            auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
            if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
                Vector2f mousePos(static_cast<float>(mouseEvent->position.x), 
                                static_cast<float>(mouseEvent->position.y));
                bool wasActive = isActive;
                
                // Check if clicking on toggle button (for password fields)
                if (isPassword && toggleButton.getGlobalBounds().contains(mousePos)) {
                    showPassword = !showPassword;
                    updateDisplayText();
                    return;
                }
                
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
        string displayText;
        if (isPassword && !showPassword) {
            displayText = string(inputText.length(), '*');
        } else {
            displayText = inputText;
        }
        
        if (isActive && cursorVisible) {
            displayText.insert(cursorPosition, "|");
        }
        text.setString(displayText);
    }

    void draw(RenderWindow& window) {
        window.draw(box);
        window.draw(label);
        
        // Draw toggle button for password fields
        if (isPassword) {
            window.draw(toggleButton);
            toggleText.setString(showPassword ? "-" : "O");
            window.draw(toggleText);
        }
        
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
    
    // SỬA LỖI: Thêm hàm public setSize
    void setSize(Vector2f newSize) {
        box.setSize(newSize);
        
        float x = box.getPosition().x;
        float y = box.getPosition().y;
        float width = newSize.x;
        float height = newSize.y;

        // Recalculate positions based on new size
        text.setPosition({x + 15, y + 15});
        placeholder.setPosition({x + 15, y + 15});

        if (isPassword) {
            toggleButton.setPosition({x + width - 40, y + 10});
            toggleText.setPosition({x + width - 25, y + height / 2.f});
        }
    }
    
    // Thêm hàm setPosition để sắp xếp lại bố cục
    void setPosition(float x, float y) {
        box.setPosition({x, y});
        label.setPosition({x, y - 25});
        text.setPosition({x + 15, y + 15});
        placeholder.setPosition({x + 15, y + 15});
        
        if (isPassword) {
            float width = box.getSize().x;
            float height = box.getSize().y;
            toggleButton.setPosition({x + width - 40, y + 10});
            toggleText.setPosition({x + width - 25, y + height / 2.f});
        }
    }
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

    // SỬA LỖI: Thêm hàm public setSize
    void setSize(Vector2f newSize) {
        box.setSize(newSize);

        float x = box.getPosition().x;
        float y = box.getPosition().y;
        float width = newSize.x;
        float height = newSize.y;

        currentText.setPosition({x + 15, y + 15});

        // Re-calculate options
        for (size_t i = 0; i < optionBoxes.size(); i++) {
            optionBoxes[i].setSize({width, height}); // Also resize the option boxes!
            optionBoxes[i].setPosition({x, y + height * (i + 1)});
            optionTexts[i].setPosition({x + 15, y + height * (i + 1) + 15});
        }
    }

    // Thêm hàm setPosition để sắp xếp lại bố cục
    void setPosition(float x, float y) {
        box.setPosition({x, y});
        label.setPosition({x, y - 25});
        currentText.setPosition({x + 15, y + 15});

        float width = box.getSize().x;
        float height = box.getSize().y;
        for (size_t i = 0; i < optionBoxes.size(); i++) {
            optionBoxes[i].setPosition({x, y + height * (i + 1)});
            optionTexts[i].setPosition({x + 15, y + height * (i + 1) + 15});
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
    int currentPage;
    int itemsPerPage;
    int totalPages;
    
    // UI State
    bool isLoggedIn;
    string currentUser;
    // 0=Login, 2=Members, 4=Stats, 5=Settings
    // (Screen 1 'Dashboard' và 3 'AddMember' đã bị loại bỏ/gộp)
    int currentScreen; 
    bool showAddMemberPopup; // Thêm cờ cho popup

    // UI Components
    vector<unique_ptr<ModernInputBox>> loginInputs;
    vector<unique_ptr<ModernButton>> menuButtons;
    
    // Các UI cho form "Add Member" (nay là một phần của class)
    vector<unique_ptr<ModernInputBox>> memberInputs;
    unique_ptr<DropdownMenu> membershipDropdown;
    unique_ptr<DropdownMenu> durationDropdown;
    
    // Thêm UI cho popup
    RectangleShape popupBackground;
    unique_ptr<ModernButton> popupSaveButton;
    unique_ptr<ModernButton> popupCancelButton;

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
          currentPage(0), itemsPerPage(8), totalPages(0), // itemsPerPage trở lại 8
          showAddMemberPopup(false), // Khởi tạo cờ
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
    
	void updatePagination() {
	    totalPages = (searchResults.size() + itemsPerPage - 1) / itemsPerPage;
	    if (totalPages == 0) totalPages = 1; // Luôn có ít nhất 1 trang
	    if (currentPage >= totalPages && totalPages > 0) {
	        currentPage = totalPages - 1;
	    }
	    if (currentPage < 0) currentPage = 0;
	}

	vector<Member> getCurrentPageMembers() {
        if (searchResults.empty()) {
            return {};
        }

	    int startIdx = currentPage * itemsPerPage;
	    // Đảm bảo startIdx hợp lệ
	    if (startIdx < 0) startIdx = 0;
        if (startIdx >= searchResults.size()) {
            // Nếu startIdx vượt quá, có thể reset về trang cuối
            currentPage = totalPages - 1;
            startIdx = currentPage * itemsPerPage;
            if (startIdx < 0) startIdx = 0;
        }

	    int endIdx = min(startIdx + itemsPerPage, static_cast<int>(searchResults.size()));
	    
	    vector<Member> pageMembers;
	    for (int i = startIdx; i < endIdx; i++) {
	        pageMembers.push_back(searchResults[i]);
	    }
	    return pageMembers;
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
        setupDashboard(); // Tên hàm vẫn giữ nguyên, nhưng nội dung đã thay đổi
        setupAddMemberScreen(); // Khởi tạo các input cho form thành viên
    }

    void setupLoginScreen() {
        loginInputs.clear();
        loginInputs.push_back(make_unique<ModernInputBox>(
            font, "Ten dang nhap", "Nhap ten dang nhap...", 525, 350));
        loginInputs.push_back(make_unique<ModernInputBox>(
            font, "Mat khau", "Nhap mat khau...", 525, 450, 350, 50, true)); // true = isPassword
        
        if (!loginInputs.empty()) loginInputs[0]->setActive(true);
    }

    void setupDashboard() {
        menuButtons.clear();
        
        // Loại bỏ "Dashboard" và "Them Thanh Vien"
        vector<string> menus = {
            "Quan Ly Thanh Vien",
            "Thong Ke",
            "Cai Dat",
            "Dang Xuat"
        };
        
        for (size_t i = 0; i < menus.size(); i++) {
            menuButtons.push_back(make_unique<ModernButton>(
                font, menus[i], 20, 100 + i * 70, 210, 50));
        }
    }

    // Hàm này giờ chỉ khởi tạo UI, không chuyển màn hình
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
            
        // Không set active ở đây nữa, sẽ set khi render

        // Khởi tạo các thành phần của popup
        popupBackground.setSize({600, 400}); // Popup nhỏ hơn
        popupBackground.setFillColor(Color(255, 255, 255));
        popupBackground.setOutlineColor(Color(70, 130, 180));
        popupBackground.setOutlineThickness(3);
        popupBackground.setPosition({(1400 - 600) / 2, (900 - 400) / 2}); // Canh giữa

        popupSaveButton = make_unique<ModernButton>(font, "LUU THONG TIN", 450, 600, 190, 50);
        popupCancelButton = make_unique<ModernButton>(font, "HUY", 660, 600, 190, 50);
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
                // Tìm input đang active
                for (size_t i = 0; i < loginInputs.size(); i++) {
                    if (loginInputs[i]->getIsActive()) {
                        loginInputs[i]->setActive(false);
                        
                        if (i + 1 < loginInputs.size()) {
                            // Chuyển đến input tiếp theo
                            loginInputs[i + 1]->setActive(true);
                        } else {
                            // Nếu là input cuối cùng, thực hiện đăng nhập
                            attemptLogin();
                        }
                        break;
                    }
                }
            }
            else if (keyEvent->code == Keyboard::Key::Tab || 
                     keyEvent->code == Keyboard::Key::Down) {
                // Chuyển đến input tiếp theo
                for (size_t i = 0; i < loginInputs.size(); i++) {
                    if (loginInputs[i]->getIsActive()) {
                        loginInputs[i]->setActive(false);
                        size_t nextIndex = (i + 1) % loginInputs.size();
                        loginInputs[nextIndex]->setActive(true);
                        break;
                    }
                }
            }
            else if (keyEvent->code == Keyboard::Key::Up) {
                // Chuyển đến input trước đó
                for (size_t i = 0; i < loginInputs.size(); i++) {
                    if (loginInputs[i]->getIsActive()) {
                        loginInputs[i]->setActive(false);
                        size_t prevIndex = (i == 0) ? loginInputs.size() - 1 : i - 1;
                        loginInputs[prevIndex]->setActive(true);
                        break;
                    }
                }
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
        // Ưu tiên xử lý popup
        if (showAddMemberPopup) {
            handleAddMemberPopupEvents(event, mousePos);
            return; // Ngăn không cho click "xuyên" qua popup
        }

        // Handle menu buttons
        for (size_t i = 0; i < menuButtons.size(); i++) {
            menuButtons[i]->handleEvent(event, mousePos);
            if (menuButtons[i]->isClicked(event, mousePos)) {
                handleMenuClick(i);
            }
        }
        
        // Handle screen-specific events
        switch (currentScreen) {
            case 2: // Members list (nay đã gộp)
                handleMembersScreenEvents(event, mousePos);
                break;
            // case 3: (ĐÃ BỊ GỘP)
            case 4: // Statistics
                // (Không có event đặc biệt)
                break;
            case 5: // Settings
                handleSettingsEvents(event, mousePos);
                break;
        }
    }

	void handleMembersScreenEvents(const Event& event, Vector2f mousePos) {
	    // --- 1. Xử lý Nút Thêm Thành Viên Mới ---
        static ModernButton addMemberBtn(font, "THEM THANH VIEN MOI", 300, 270, 250, 50);
        addMemberBtn.handleEvent(event, mousePos);
        if (addMemberBtn.isClicked(event, mousePos)) {
            showAddMemberPopup = true;
            // Đặt active cho input đầu tiên của popup
            if (!memberInputs.empty()) memberInputs[0]->setActive(true);
        }

        // --- 2. Xử lý Danh Sách Thành Viên (logic cũ) ---
	    static unique_ptr<ModernInputBox> searchBox;
	    if (!searchBox) {
	        searchBox = make_unique<ModernInputBox>(
	            font, "", "Tim kiem ten, SDT, ma...", 750, 335, 450, 50); // Y mới
	    }
	    
	    searchBox->handleEvent(event);
	    
	    // Xử lý phím (Enter)
	    if (event.is<Event::KeyPressed>()) {
	        auto* keyEvent = event.getIf<Event::KeyPressed>();
	        
            bool searchInputActive = searchBox->getIsActive();

	        if (keyEvent->code == Keyboard::Key::Enter) {
                if (searchInputActive) {
                    // --- Thực hiện tìm kiếm ---
                    string query = searchBox->getText();
                    if (query.empty()) {
                        searchResults = dataManager.getAllMembers();
                    } else {
                        searchResults = dataManager.searchMembers(query);
                    }
                    currentPage = 0; // Reset về trang đầu khi tìm kiếm
                    updatePagination();
                } 
	        }
	    }
	    
	    // --- 3. Xử lý Nút Phân Trang (vị trí mới) ---
	    static ModernButton prevBtn(font, "< TRANG TRUOC", 300, 850, 150, 40);
	    static ModernButton nextBtn(font, "TRANG SAU >", 1200, 850, 150, 40);
	    
	    prevBtn.handleEvent(event, mousePos);
	    nextBtn.handleEvent(event, mousePos);
	    
	    if (prevBtn.isClicked(event, mousePos) && currentPage > 0) {
	        currentPage--;
	    }
	    
	    if (nextBtn.isClicked(event, mousePos) && currentPage < totalPages - 1) {
	        currentPage++;
	    }
	    
	    // Scroll handling
	    if (event.is<Event::MouseWheelScrolled>()) {
	        auto* scrollEvent = event.getIf<Event::MouseWheelScrolled>();
	        scrollOffset -= static_cast<int>(scrollEvent->delta * 30);
	        if (scrollOffset < 0) scrollOffset = 0;
	    }
	}

    // Thêm hàm xử lý event cho Popup
    void handleAddMemberPopupEvents(const Event& event, Vector2f mousePos) {
        // Xử lý các input trong popup
        for (auto& input : memberInputs) {
            input->handleEvent(event);
        }
        if (membershipDropdown) {
            membershipDropdown->handleEvent(event, mousePos);
        }
        if (durationDropdown) {
            durationDropdown->handleEvent(event, mousePos);
        }
        
        // Xử lý nút Lưu/Hủy của popup
        popupSaveButton->handleEvent(event, mousePos);
        popupCancelButton->handleEvent(event, mousePos);
        
        if (popupSaveButton->isClicked(event, mousePos)) {
            addNewMember(); // Hàm này đã bao gồm setMessage và cập nhật list
            // Chỉ đóng popup nếu thêm thành công (kiểm tra name/phone)
            if (!memberInputs[0]->getText().empty() && !memberInputs[1]->getText().empty()) {
                showAddMemberPopup = false;
                clearAddMemberForm();
            }
        }
        
        if (popupCancelButton->isClicked(event, mousePos)) {
            showAddMemberPopup = false;
            clearAddMemberForm();
        }

        // Xử lý phím (Enter, Tab, Up, Down) cho form
	    if (event.is<Event::KeyPressed>()) {
	        auto* keyEvent = event.getIf<Event::KeyPressed>();
	        
            bool formInputActive = false;
            for (auto& input : memberInputs) {
                if (input->getIsActive()) {
                    formInputActive = true;
                    break;
                }
            }

	        if (keyEvent->code == Keyboard::Key::Enter) {
                if (formInputActive) {
                    // --- Chuyển input trong form hoặc Lưu ---
                    bool foundActive = false;
                    for (size_t i = 0; i < memberInputs.size(); i++) {
                        if (memberInputs[i]->getIsActive()) {
                            memberInputs[i]->setActive(false);
                            
                            if (i + 1 < memberInputs.size()) {
                                memberInputs[i + 1]->setActive(true);
                            } else {
                                // Input cuối cùng, gọi addNewMember
                                addNewMember();
                                if (!memberInputs[0]->getText().empty() && !memberInputs[1]->getText().empty()) {
                                    showAddMemberPopup = false;
                                    clearAddMemberForm();
                                }
                            }
                            foundActive = true;
                            break;
                        }
                    }
                }
	        }
            else if ((keyEvent->code == Keyboard::Key::Tab || keyEvent->code == Keyboard::Key::Down) && formInputActive) {
                // Chuyển input trong form
                for (size_t i = 0; i < memberInputs.size(); i++) {
                    if (memberInputs[i]->getIsActive()) {
                        memberInputs[i]->setActive(false);
                        size_t nextIndex = (i + 1) % memberInputs.size();
                        memberInputs[nextIndex]->setActive(true);
                        break;
                    }
                }
            }
            else if (keyEvent->code == Keyboard::Key::Up && formInputActive) {
                // Chuyển input trong form
                for (size_t i = 0; i < memberInputs.size(); i++) {
                    if (memberInputs[i]->getIsActive()) {
                        memberInputs[i]->setActive(false);
                        size_t prevIndex = (i == 0) ? memberInputs.size() - 1 : i - 1;
                        memberInputs[prevIndex]->setActive(true);
                        break;
                    }
                }
            }
	    }
    }

    void handleSettingsEvents(const Event& event, Vector2f mousePos) {
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
            input->handleEvent(event);
        }
            
        static ModernButton changeBtn(font, "DOI MAT KHAU", 300, 520, 200, 50);
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
            case 0: // Quan Ly Thanh Vien
                currentScreen = 2;
	            searchResults = dataManager.getAllMembers();
	            currentPage = 0; // Reset về trang đầu
	            updatePagination(); // Cập nhật phân trang
	            break;
            case 1: // Thong Ke
                currentScreen = 4;
                break;
            case 2: // Cai Dat
                currentScreen = 5;
                break;
            case 3: // Dang Xuat
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
            // Nếu popup mở, chỉ update input của popup
            if (showAddMemberPopup) {
                for (auto& input : memberInputs) {
                    input->update(dt);
                }
            } else {
                // Update các input khác nếu cần (ví dụ: search box)
                // (Hiện tại search box không cần update, nhưng nếu có 
                // input cài đặt, chúng sẽ cần update ở đây)
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
            case 2: renderMembersScreen(); break;
            case 4: renderStatisticsScreen(); break;
            case 5: renderSettingsScreen(); break;
        }
        
        // Message (vị trí cố định)
        messageText.setPosition({300, 880}); // Di chuyển xuống dưới cùng
        window.draw(messageText);

        // Vẽ popup nếu cờ được bật
        if (showAddMemberPopup) {
            renderAddMemberPopup();
        }
    }

    void renderMembersScreen() {
        Text title(font, "QUAN LY THANH VIEN", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 100});
        window.draw(title);

        // === 1. THỐNG KÊ (Lấy từ Dashboard) ===
        int activeMembers = dataManager.getActiveMemberCount();
        int expiredMembers = dataManager.getExpiredMemberCount();
        
        // Thu nhỏ thẻ
        renderStatCard("THANH VIEN HOAT DONG", to_string(activeMembers), 
                      300, 150, Color(80, 180, 100), 200, 100); // Rộng 200, Cao 100
        renderStatCard("THANH VIEN HET HAN", to_string(expiredMembers), 
                      520, 150, Color(220, 100, 80), 200, 100); // Rộng 200, Cao 100

        // === 2. NÚT THÊM THÀNH VIÊN ===
        static ModernButton addMemberBtn(font, "THEM THANH VIEN MOI", 300, 270, 250, 50);
        addMemberBtn.draw(window);

        // === 3. DANH SÁCH THÀNH VIÊN ===
        Text listTitle(font, "DANH SACH THANH VIEN", 24);
        listTitle.setFillColor(Color(70, 130, 180));
        listTitle.setStyle(Text::Bold);
        listTitle.setPosition({300, 340}); // Y mới
        window.draw(listTitle);

        // Search box (vị trí mới)
        static unique_ptr<ModernInputBox> searchBox = make_unique<ModernInputBox>(
            font, "", "Tim kiem ten, SDT, ma...", 750, 335, 450, 50); // Y mới
        searchBox->draw(window);
        
        // Table header (vị trí mới)
        RectangleShape tableHeader({1050, 40});
        tableHeader.setPosition({300, 390}); // Y mới
        tableHeader.setFillColor(Color(70, 130, 180));
        window.draw(tableHeader);
        
        vector<string> headers = {"Ma", "Ho Ten", "SDT", "Loai The", "Ngay Het Han", "Trang Thai"};
        vector<float> positions = {320, 420, 620, 750, 880, 1040};
        
        for (size_t i = 0; i < headers.size(); i++) {
            Text header(font, headers[i], 16);
            header.setFillColor(Color::White);
            header.setStyle(Text::Bold);
            header.setPosition({positions[i], 400}); // Y mới
            window.draw(header);
        }
        
        // Lấy thành viên của trang hiện tại
        vector<Member> currentPageMembers = getCurrentPageMembers();
        
        float yPos = 440; // Y bắt đầu mới
        
        for (int i = 0; i < currentPageMembers.size(); i++) {
            const auto& m = currentPageMembers[i];
            
            RectangleShape row({1050, 45}); // Hàng mỏng hơn
            row.setPosition({300, yPos});
            row.setFillColor(i % 2 == 0 ? Color(250, 250, 250) : Color::White);
            row.setOutlineColor(Color(230, 230, 230));
            row.setOutlineThickness(1);
            window.draw(row);
            
            Text idText(font, m.id, 14);
            idText.setFillColor(Color(50, 50, 50));
            idText.setPosition({320, yPos + 15});
            window.draw(idText);
            
            Text nameText(font, m.name, 14);
            nameText.setFillColor(Color(50, 50, 50));
            nameText.setPosition({420, yPos + 15});
            window.draw(nameText);
            
            Text phoneText(font, m.phone, 14);
            phoneText.setFillColor(Color(50, 50, 50));
            phoneText.setPosition({620, yPos + 15});
            window.draw(phoneText);
            
            Text typeText(font, dataManager.getMembershipTypeName(m.type), 14);
            typeText.setFillColor(Color(70, 130, 180));
            typeText.setPosition({750, yPos + 15});
            window.draw(typeText);
            
            Text dateText(font, m.endDate, 14);
            dateText.setFillColor(Color(50, 50, 50));
            dateText.setPosition({880, yPos + 15});
            window.draw(dateText);
            
            Text statusText(font, m.isActive ? "Hoat dong" : "Het han", 14);
            statusText.setFillColor(m.isActive ? Color(80, 180, 100) : Color(220, 100, 80));
            statusText.setPosition({1040, yPos + 15});
            window.draw(statusText);
            
            yPos += 50;
        }
        
        // Phân trang (vị trí mới)
        static ModernButton prevBtn(font, "< TRANG TRUOC", 300, 850, 150, 40);
        static ModernButton nextBtn(font, "TRANG SAU >", 1200, 850, 150, 40);
        
        prevBtn.draw(window);
        nextBtn.draw(window);
        
        // Hiển thị thông tin trang
        Text pageInfo(font, "Trang " + to_string(currentPage + 1) + " / " + 
                      to_string(max(1, totalPages)), 16);
        pageInfo.setFillColor(Color(100, 100, 100));
        pageInfo.setPosition({650, 860});
        window.draw(pageInfo);
    }

    // Thêm hàm vẽ Popup
    void renderAddMemberPopup() {
        // 1. Vẽ lớp phủ mờ
        RectangleShape overlay({1400, 900});
        overlay.setFillColor(Color(0, 0, 0, 150));
        window.draw(overlay);

        // 2. Vẽ nền popup
        window.draw(popupBackground);

        // 3. Vẽ tiêu đề popup
        Text popupTitle(font, "THEM THANH VIEN MOI", 28);
        popupTitle.setFillColor(Color(70, 130, 180));
        popupTitle.setStyle(Text::Bold);
        popupTitle.setPosition({popupBackground.getPosition().x + 20, popupBackground.getPosition().y + 20});
        window.draw(popupTitle);

        // 4. Định vị lại và vẽ các control
        float popupY = popupBackground.getPosition().y + 100;

        // Căn chỉnh 2 cột
        float col1X = popupBackground.getPosition().x + 30;
        float col2X = popupBackground.getPosition().x + 320;
        float inputWidth = 250; // Input ngắn hơn

        // Cập nhật kích thước (SỬA LỖI: gọi hàm public)
        for(auto& input : memberInputs) input->setSize({inputWidth, 50});
        membershipDropdown->setSize({inputWidth, 50});
        durationDropdown->setSize({inputWidth, 50});

        // Hàng 1
        if (!memberInputs.empty()) memberInputs[0]->setPosition(col1X, popupY); // Name
        if (memberInputs.size() > 1) memberInputs[1]->setPosition(col2X, popupY); // Phone
        
        // Hàng 2
        if (memberInputs.size() > 2) memberInputs[2]->setPosition(col1X, popupY + 90); // Email
        if (membershipDropdown) membershipDropdown->setPosition(col2X, popupY + 90); // Type
        
        // Hàng 3
        if (durationDropdown) durationDropdown->setPosition(col1X, popupY + 180); // Duration

        // Nút
        popupSaveButton->setPosition(col1X + 50, popupY + 270);
        popupCancelButton->setPosition(col2X, popupY + 270);

        // Vẽ
        for (auto& input : memberInputs) { input->draw(window); }
        if (membershipDropdown) { membershipDropdown->draw(window); }
        if (durationDropdown) { durationDropdown->draw(window); }
        popupSaveButton->draw(window);
        popupCancelButton->draw(window);
    }

    void renderStatisticsScreen() {
        Text title(font, "THONG KE & BAO CAO", 32);
        title.setFillColor(Color(70, 130, 180));
        title.setStyle(Text::Bold);
        title.setPosition({300, 120});
        window.draw(title);
        
        Text comingSoon(font, "Tinh nang Thong Ke Doanh Thu dang duoc xay dung.", 20);
        comingSoon.setFillColor(Color(100, 100, 100));
        comingSoon.setPosition({300, 200});
        window.draw(comingSoon);
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
                font, "Mat khau cu", "Nhap mat khau cu...", 300, 250, 400, 50, true)); // true = isPassword
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Mat khau moi", "Nhap mat khau moi...", 300, 340, 400, 50, true)); // true = isPassword
            settingsInputs.push_back(make_unique<ModernInputBox>(
                font, "Xac nhan mat khau", "Nhap lai mat khau moi...", 300, 430, 400, 50, true)); // true = isPassword
        }
        
        for (auto& input : settingsInputs) {
            input->draw(window);
        }
        
        static ModernButton changeBtn(font, "DOI MAT KHAU", 300, 520, 200, 50);
        changeBtn.draw(window);
    }

    // Thêm tham số width/height
    void renderStatCard(const string& label, const string& value, 
                       float x, float y, Color color, float width = 250, float height = 150) {
        RectangleShape card({width, height});
        card.setPosition({x, y});
        card.setFillColor(color);
        window.draw(card);
        
        // Điều chỉnh text cho vừa
        Text labelText(font, label, 14); // Cỡ chữ nhỏ hơn
        labelText.setFillColor(Color(255, 255, 255, 180));
        labelText.setPosition({x + 15, y + 15}); // Gần hơn
        window.draw(labelText);
        
        Text valueText(font, value, 32); // Cỡ chữ nhỏ hơn
        valueText.setFillColor(Color::White);
        valueText.setStyle(Text::Bold);
        valueText.setPosition({x + 15, y + 45}); // Gần hơn
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
            currentScreen = 2; // Chuyển đến màn hình 2 (Members)
            // setMessage("Dang nhap thanh cong!", Color(80, 180, 80));

            // Tải danh sách thành viên ngay khi đăng nhập
            searchResults = dataManager.getAllMembers();
            currentPage = 0;
            updatePagination();
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
            setMessage("Vui long nhap Ho Ten va So Dien Thoai!", Color(220, 80, 80));
            return;
        }
        
        MembershipType type = static_cast<MembershipType>(membershipDropdown->getSelectedIndex());
        int months = membershipDropdown ? 
            (durationDropdown->getSelectedIndex() == 0 ? 1 : 
             durationDropdown->getSelectedIndex() == 1 ? 3 :
             durationDropdown->getSelectedIndex() == 2 ? 6 : 12) : 1;
        
        string id = dataManager.addMember(name, phone, email, type, months);
        setMessage("Them thanh vien thanh cong! Ma: " + id, Color(80, 180, 80));
        
        // Không chuyển màn hình, chỉ cập nhật lại danh sách
        searchResults = dataManager.getAllMembers();
        currentPage = 0;
        updatePagination();

        // Thêm hàm dọn dẹp form
        clearAddMemberForm();
    }

    void logout() {
        isLoggedIn = false;
        currentUser = "";
        currentScreen = 0;
        for (auto& input : loginInputs) {
            input->clear();
        }
        // setMessage("Da dang xuat!", Color(100, 100, 100));
    }

    // Thêm hàm dọn dẹp
    void clearAddMemberForm() {
        for(auto& input : memberInputs) input->clear();
        membershipDropdown->setSelectedIndex(0);
        durationDropdown->setSelectedIndex(0);
        if (!memberInputs.empty()) memberInputs[0]->setActive(false);
    }

    void setMessage(const string& msg, Color color = Color(70, 130, 180)) {
        messageText.setString(msg);
        messageText.setCharacterSize(16);
        messageText.setFillColor(color);
        // Vị trí của message sẽ được đặt lại trong hàm renderMain
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

