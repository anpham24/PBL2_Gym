// GUI/Screens/MonTapScreen.cpp
#include "MonTapScreen.h"
#include "QuanLy.h"
#include "MonTapService.h" // Goi Service

// --- Constructor ---
MonTapScreen::MonTapScreen(App& app) 
    : BaseScreen(app), 
      monTapTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),                 // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f; // Vi tri bat dau (sau Sidebar)
    
    // --- Nut Them ---
    themMonTapButton.setup("Them Mon Tap Moi", app.getGlobalFont());
    themMonTapButton.setSize(200, 40);
    themMonTapButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themMonTapButton.setOnClick([this]() {
        // Goi popup them moi, voi callback de load lai data khi thanh cong
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData();
        });
    });

    // --- Bang Du Lieu ---
    monTapTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    monTapTable.addColumn("ID", 120, [](const MonTap* mt) { return mt->getID(); });
    monTapTable.addColumn("Ten Mon", 300, [](const MonTap* mt) { return mt->getTenMon(); });
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        monTapTable.addEditAction([this](MonTap* mt) {
            // Goi popup sua, voi callback
            this->formPopup.show(mt, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        monTapTable.addDeleteAction([this](MonTap* mt) {
            auto onConfirm = [this, mt]() {
                MonTapService::xoaMonTap(mt->getID()); // GOI SERVICE
                this->loadAndDisplayData(); // Load lai sau khi xoa
            };
            
            // Kiem tra rang buoc (neu Mon Tap co Lop Hoc hoac Goi Tap)
            std::string warning = "";
            if (!mt->getDsLopHoc().empty() || !mt->getDsGoiTap().empty()) {
                warning = "Mon tap nay dang duoc su dung trong Goi Tap hoac Lop Hoc!";
            }
            this->deletePopup.show(mt->getTenMon(), onConfirm, warning);
        });
    }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600); // Dat duoi bang
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    // --- Load data lan dau ---
    loadAndDisplayData();
}

MonTapScreen::~MonTapScreen() {}

// --- Ham Logic ---
void MonTapScreen::loadAndDisplayData() {
    allMonTap.clear();
    
    // Lay data tu QuanLy
    const MyVector<MonTap*>& dsMonTapGoc = app.getQuanLy().getDsMonTap();
    for (size_t i = 0; i < dsMonTapGoc.size(); ++i) {
        allMonTap.push_back(dsMonTapGoc[i]);
    }
    
    // (Sau nay se them logic Tim kiem/Loc o day)
    
    // Thiet lap pagination
    pagination.setup(allMonTap.size(), 10); // 10 muc moi trang
    
    // Hien thi trang hien tai
    onPageChange(pagination.getCurrentPage());
}

void MonTapScreen::onPageChange(int newPage) {
    pagedMonTap.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allMonTap.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedMonTap.push_back(allMonTap.at(i));
    }
    
    monTapTable.setData(pagedMonTap);
}

// --- Vong lap chinh (Event, Update, Draw) ---
void MonTapScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // --- Xu ly Popups truoc (de chan click) ---
    if (formPopup.getIsVisible()) {
        formPopup.handleEvent(event, mousePos);
        return;
    }
    if (deletePopup.getIsVisible()) {
        deletePopup.handleEvent(event, mousePos);
        return;
    }
    
    // --- Neu khong co popup, xu ly man hinh chinh ---
    if (!isStaffReadOnly) { // Staff khong the an nut "Them"
        themMonTapButton.handleEvent(event, mousePos);
    }
    monTapTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void MonTapScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup
    formPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    // Neu co popup dang mo, khong update man hinh chinh
    if (formPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    if (!isStaffReadOnly) {
        themMonTapButton.update(mousePos);
    }
    monTapTable.update(mousePos);
    pagination.update(mousePos);
}

void MonTapScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    if (!isStaffReadOnly) { // Staff khong thay nut "Them"
        themMonTapButton.draw(target);
    }
    monTapTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG de no noi len tren)
    formPopup.draw(target);
    deletePopup.draw(target);
}