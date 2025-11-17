// GUI/Popups/GoiTapMonTapPopup.cpp
#include "GoiTapMonTapPopup.h"
#include "Managers/QuanLy.h"
#include "Services/GoiTapService.h" // De goi themMonTapVaoGoiTap
#include <iostream>

GoiTapMonTapPopup::GoiTapMonTapPopup(App& app)
    : BasePopup(app, "Quan Ly Mon Tap"),
      tabManager(app.getGlobalFont()),
      currentGoiTap(nullptr),
      isReadOnly(false),
      hoveredAssignedIndex(-1),
      hoveredAvailableIndex(-1)
{
    // Chinh lai kich thuoc panel
    sf::Vector2f panelSize(500, 500);
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    // Dat lai vi tri title/nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + panelSize.x - 40, popupPanel.getPosition().y + 15);
    
    // Setup Tabs
    tabManager.setPosition(popupPanel.getPosition().x + 30, popupPanel.getPosition().y + 60);
    
    // Setup Search (Tab 2)
    searchInput.setup("Tim mon tap de them...", font, false);
    searchInput.setSize(panelSize.x - 60, 35);
    searchInput.setPosition(popupPanel.getPosition().x + 30, popupPanel.getPosition().y + 110);
}

void GoiTapMonTapPopup::show(GoiTap* gt, bool isReadOnly) {
    if (gt == nullptr) return;
    currentGoiTap = gt;
    this->isReadOnly = isReadOnly;
    
    title.setString("Mon Tap cho Goi: " + sf::String::fromUtf8(gt->getTenGoi().begin(), gt->getTenGoi().end()));
    
    // (MOI) Xoa cac tab cu va tao lai dua tren quyen
    tabManager.~TabManager(); // Huy tab cu
    new (&tabManager) TabManager(font); // Tao lai
    tabManager.setPosition(popupPanel.getPosition().x + 30, popupPanel.getPosition().y + 60);

    tabManager.addTab("Mon Da Gan");
    if (!isReadOnly) {
        tabManager.addTab("Them Mon Tap"); // Staff khong thay tab nay
    }
    
    loadData();
    BasePopup::show();
}

void GoiTapMonTapPopup::hide() {
    BasePopup::hide();
    currentGoiTap = nullptr;
    searchInput.setString("");
}

void GoiTapMonTapPopup::loadData() {
    if (currentGoiTap == nullptr) return;

    // Tab 1: Load mon da gan (Luon luon load)
    assignedMonTap = currentGoiTap->getDsMonTap();
    
    // Tab 2: Load mon co a them (Chi load neu la Admin)
    if (!isReadOnly) {
        availableMonTap.clear();
        MyVector<MonTap*>& allMonTap = app.getQuanLy().getDsMonTap();
        std::string searchTerm = searchInput.getString(); //.toAnsiString(); SFML 3.0
        // (nen convert searchTerm sang chu thuong)

        for (size_t i = 0; i < allMonTap.size(); ++i) {
            MonTap* mt = allMonTap[i];
            bool isAssigned = false;
            for (size_t j = 0; j < assignedMonTap.size(); ++j) {
                if (mt == assignedMonTap[j]) {
                    isAssigned = true;
                    break;
                }
            }
            if (isAssigned) continue;
            
            std::string mtName = mt->getTenMon();
            // (nen convert mtName sang chu thuong)
            if (searchTerm.empty() || mtName.find(searchTerm) != std::string::npos) {
                availableMonTap.push_back(mt);
            }
        }
    }
}

// --- Xu ly them/xoa ---
void GoiTapMonTapPopup::handleRemoveMonTap(MonTap* mt) {
    if (currentGoiTap == nullptr || mt == nullptr || isReadOnly) return;
    
    // (Ban can them ham `xoaMonTapKhoiGoiTap` vao Service)
    // GoiTapService::xoaMonTapKhoiGoiTap(currentGoiTap->getID(), mt->getID());
    
    // Tam thoi goi logic truc tiep:
    currentGoiTap->removeMonTap(mt);
    mt->removeGoiTap(currentGoiTap); // MonTap.h/.cpp can ham nay
    app.getQuanLy().setDirty(true);
    
    std::cout << "Da xoa Mon " << mt->getTenMon() << " khoi goi " << currentGoiTap->getTenGoi() << std::endl;
    loadData();
}

void GoiTapMonTapPopup::handleAddMonTap(MonTap* mt) {
    if (currentGoiTap == nullptr || mt == nullptr || isReadOnly) return;

    // Goi Service
    GoiTapService::themMonTapVaoGoiTap(currentGoiTap->getID(), mt->getID());
    
    std::cout << "Da them Mon " << mt->getTenMon() << " vao goi " << currentGoiTap->getTenGoi() << std::endl;
    loadData();
}

// --- Event, Update, Draw (DA CAP NHAT QUYEN) ---
void GoiTapMonTapPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos);
    tabManager.handleEvent(event, mousePos);

    // Neu la Staff, chi cho phep xem, khong cho phep tuong tac them
    if (isReadOnly) return; 

    // --- Logic chi danh cho Admin ---
    if (tabManager.getActiveTab() == 0) {
        // --- Tab 1: HLV Da Gan (Xu ly click nut Xoa) ---
        if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (hoveredAssignedIndex != -1) {
                float pX = popupPanel.getPosition().x + 30;
                float pY = popupPanel.getPosition().y + 110;
                float rowY = pY + hoveredAssignedIndex * 35;
                // SFML 3.0: Tinh toan bounds cua nut "Xoa"
                sf::FloatRect xButtonBounds({pX + 400, rowY}, {50, 30});
                
                if (xButtonBounds.contains(sf::Vector2f(mousePos))) {
                     handleRemoveMonTap(assignedMonTap[hoveredAssignedIndex]);
                }
            }
        }
    } else {
        // --- Tab 2: Them HLV ---
        if (event.getIf<sf::Event::TextEntered>()) {
            searchInput.handleEvent(event);
            loadData(); // Load lai khi search
        } else if (event.getIf<sf::Event::KeyPressed>()) {
             searchInput.handleEvent(event);
        } else if (event.getIf<sf::Event::MouseButtonPressed>()) {
             if (hoveredAvailableIndex != -1) {
                 handleAddMonTap(availableMonTap[hoveredAvailableIndex]);
             }
        }
    }
}

void GoiTapMonTapPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos);
    
    hoveredAssignedIndex = -1;
    hoveredAvailableIndex = -1;

    float pX = popupPanel.getPosition().x + 30;
    float pY_tab1 = popupPanel.getPosition().y + 110; // Duoi tabs
    float rowHeight = 35;

    if (tabManager.getActiveTab() == 0) {
        // --- Tab 1: Update Hover ---
        for (size_t i = 0; i < assignedMonTap.size(); ++i) {
            sf::FloatRect rowBounds({pX, pY_tab1 + i * rowHeight}, {popupPanel.getSize().x - 60, rowHeight});
            if (rowBounds.contains(sf::Vector2f(mousePos))) {
                hoveredAssignedIndex = i;
                break;
            }
        }
    } else if (!isReadOnly) {
        // --- Tab 2: Update Hover (Chi Admin) ---
        searchInput.update(sf::Time::Zero);
        float pY_tab2 = popupPanel.getPosition().y + 160; // Duoi thanh search
        for (size_t i = 0; i < availableMonTap.size(); ++i) {
            sf::FloatRect rowBounds({pX, pY_tab2 + i * rowHeight}, {popupPanel.getSize().x - 60, rowHeight});
            if (rowBounds.contains(sf::Vector2f(mousePos))) {
                hoveredAvailableIndex = i;
                break;
            }
        }
    }
}

void GoiTapMonTapPopup::drawContent(sf::RenderTarget& target) {
    tabManager.draw(target);
    
    if (tabManager.getActiveTab() == 0) {
        drawTabAssigned(target);
    } else if (!isReadOnly) { // Chi ve Tab 2 neu la Admin
        drawTabAdd(target);
    }
}

void GoiTapMonTapPopup::drawTabAssigned(sf::RenderTarget& target) {
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 110;
    float rowHeight = 35;
    
    sf::Text text(font, "", 16);
    
    for (size_t i = 0; i < assignedMonTap.size(); ++i) {
        MonTap* mt = assignedMonTap[i];
        float currentY = pY + i * rowHeight;
        
        // Ve highlight
        if (i == hoveredAssignedIndex) {
            sf::RectangleShape hoverBg(sf::Vector2f(popupPanel.getSize().x - 60, rowHeight));
            hoverBg.setPosition(sf::Vector2f(pX, currentY));
            hoverBg.setFillColor(Config::CardLight);
            target.draw(hoverBg);
        }
        
        // Ve thong tin Mon Tap
        text.setString(mt->getID());
        text.setPosition(sf::Vector2f(pX + 5, currentY + 7));
        text.setFillColor(Config::TextNormal);
        target.draw(text);
        
        text.setString(mt->getTenMon());
        text.setPosition(sf::Vector2f(pX + 100, currentY + 7));
        target.draw(text);

        // (MOI) Chi ve nut "Xoa" neu la Admin
        if (!isReadOnly) {
            text.setString("Xoa");
            text.setPosition(sf::Vector2f(pX + 400, currentY + 7));
            // To mau do neu dang hover dung nut Xoa
            sf::FloatRect xButtonBounds({pX + 400, currentY}, {50, 30});
            if (i == hoveredAssignedIndex && xButtonBounds.contains(sf::Vector2f(sf::Mouse::getPosition(app.getWindow())))) {
                text.setFillColor(sf::Color::Red);
            } else {
                text.setFillColor(Config::Danger);
            }
            target.draw(text);
        }
    }
}

void GoiTapMonTapPopup::drawTabAdd(sf::RenderTarget& target) {
    searchInput.draw(target);
    
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 160; // Duoi thanh search
    float rowHeight = 35;
    
    sf::Text text(font, "", 16);

    for (size_t i = 0; i < availableMonTap.size(); ++i) {
        MonTap* mt = availableMonTap[i];
        float currentY = pY + i * rowHeight;
        
        // Ve highlight
        if (i == hoveredAvailableIndex) {
            sf::RectangleShape hoverBg(sf::Vector2f(popupPanel.getSize().x - 60, rowHeight));
            hoverBg.setPosition(sf::Vector2f(pX, currentY));
            hoverBg.setFillColor(Config::CardLight);
            target.draw(hoverBg);
        }
        
        // Ve thong tin Mon Tap (de click vao them)
        text.setString(mt->getTenMon());
        text.setPosition(sf::Vector2f(pX + 5, currentY + 7));
        text.setFillColor(Config::TextNormal);
        target.draw(text);
    }
}