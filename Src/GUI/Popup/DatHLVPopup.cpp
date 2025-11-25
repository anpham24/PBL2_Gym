// GUI/Popups/DatHLVPopup.cpp
#include "DatHLVPopup.h"
#include "LogTapPTService.h" // Goi Service
#include "Validator.h"
#include "DateTimeUils.h" // De lay ngay gio hien tai
#include "QuanLy.h"
#include <iostream>
#include <string> // De dung std::string

DatHLVPopup::DatHLVPopup(App& app)
    : BasePopup(app, "Dat Lich Tap Voi HLV"),
      font(app.getGlobalFont()),      // ✅ Khởi tạo reference
      currentHoiVien(nullptr),
      currentHopDong(nullptr),
      selectedHLV(nullptr),
      hoveredListIndex(-1),
    //   tooltip(font, "", 14),          // ✅ Dùng font sau khi đã khởi tạo
      errorMessage(font, "", 14),
      searchInput(),
      confirmButton(),
      cancelButton()
{
    // Layout setup
    sf::Vector2f panelSize(450, 500);
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    
    title.setPosition(sf::Vector2f(
        popupPanel.getPosition().x + 20, 
        popupPanel.getPosition().y + 15
    ));
    
    closeButton.setPosition(
        popupPanel.getPosition().x + panelSize.x - 40, 
        popupPanel.getPosition().y + 15
    );

    float pX = popupPanel.getPosition().x;
    float pY = popupPanel.getPosition().y;

    searchInput.setup("Tim HLV theo ten...", font, false);
    searchInput.setSize(panelSize.x - 60, 35);
    searchInput.setPosition(pX + 30, pY + 70);

    listContainer.setSize(sf::Vector2f(panelSize.x - 60, 280));
    listContainer.setPosition(sf::Vector2f(pX + 30, pY + 120));
    listContainer.setFillColor(Config::CardLight);
    listContainer.setOutlineColor(Config::CardLight);
    listContainer.setOutlineThickness(1);

    // tooltip.setFillColor(Config::Warning);
    
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX + 30, pY + 410));

    float btnY = pY + panelSize.y - 70;
    
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 70, btnY);
    confirmButton.setOnClick([this]() { 
        std::cout << "🔘 Confirm button clicked!" << std::endl;
        this->handleSubmit(); 
    });

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 260, btnY);
    cancelButton.setOnClick([this]() { 
        std::cout << "🔘 Cancel button clicked!" << std::endl;
        this->hide(); 
    });
}

void DatHLVPopup::show(HoiVien* hv, HopDong* hd, std::function<void()> onClose) {
    if (hv == nullptr || hd == nullptr) return;
    
    currentHoiVien = hv;
    currentHopDong = hd;
    onCloseCallback = onClose; // ✅ LƯU CALLBACK
    
    if (hd->getSoBuoiPTConLai() <= 0) {
        errorMessage.setString("Goi nay da het buoi PT.");
    } else {
        errorMessage.setString("");
    }
    
    loadAllHLV();
    applySearchFilter();
    BasePopup::show();
}

void DatHLVPopup::handleSubmit() {
    if (selectedHLV == nullptr || currentHopDong == nullptr) return;
    
    if (currentHopDong->getSoBuoiPTConLai() <= 0) {
        errorMessage.setString("Goi nay da het buoi PT");
        return;
    }

    std::string now = DateTimeUtils::getCurrentDateTime();
    LogTapPTService::themLogTapPT(currentHoiVien->getID(), selectedHLV->getID(), now);
    
    // ✅ TRỪ PT
    currentHopDong->truBuoiPT(1);
    
    // std::cout << "✅ PT session used. Remaining: " << currentHopDong->getSoBuoiPTConLai() << std::endl;
    
    // ✅ Đánh dấu dirty để lưu
    app.getQuanLy().setDirty(true);
    
    hide(); // ✅ Sẽ gọi callback
}

void DatHLVPopup::hide() {
    std::cout << "🚪 DatHLVPopup::hide() called" << std::endl;
    
    BasePopup::hide();
    currentHoiVien = nullptr;
    currentHopDong = nullptr;
    selectedHLV = nullptr;
    searchInput.setString("");
    allActiveHLV.clear();
    displayedHLV.clear();
    errorMessage.setString("");
    
    // ✅ GỌI CALLBACK
    if (onCloseCallback) {
        std::cout << "   📞 Calling onCloseCallback..." << std::endl;
        onCloseCallback();
    }
}
void DatHLVPopup::loadAllHLV() {
    allActiveHLV.clear();
    
    const MyVector<HLV*>& allHLV = app.getQuanLy().getDsHLV(); 
    
    for (size_t i = 0; i < allHLV.size(); ++i) {
        HLV* hlv = allHLV[i];
        if (hlv != nullptr && hlv->getIsActive()) { // Chi lay HLV dang hoat dong
            allActiveHLV.push_back(hlv);
        }
    }
}

/**
 * @brief (KHONG DOI) Loc theo ten
 */
void DatHLVPopup::applySearchFilter() {
    displayedHLV.clear();
    selectedHLV = nullptr;
    
    std::string searchTerm = searchInput.getString();//.toAnsiString();
    // (nen convert searchTerm ve chu thuong)

    if (searchTerm.empty()) {
        for(size_t i = 0; i < allActiveHLV.size(); ++i) {
            displayedHLV.push_back(allActiveHLV[i]);
        }
    } else {
        for (size_t i = 0; i < allActiveHLV.size(); ++i) {
            HLV* hlv = allActiveHLV[i];
            std::string hlvName = hlv->getHoTen();
            // (Chuyen hlvName ve chu thuong)
            
            if (hlvName.find(searchTerm) != std::string::npos) {
                displayedHLV.push_back(hlv);
            }
        }
    }
}

void DatHLVPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    
    BasePopup::handleEvent(event, mousePos);
    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    
    // ✅ XỬ LÝ CLICK ĐỂ FOCUS/UNFOCUS InputBox
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            
            // ✅ KIỂM TRA: Click vào search box?
            if (searchInput.isMouseOver(mousePos)) {
                searchInput.setFocus(true); // ← SET FOCUS
                std::cout << "🔍 Search box focused" << std::endl;
            } else {
                searchInput.setFocus(false); // ← BỎ FOCUS khi click ra ngoài
            }
            
            // ✅ KIỂM TRA: Click vào list?
            if (listContainer.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                if (hoveredListIndex >= 0 && hoveredListIndex < displayedHLV.size()) {
                    selectedHLV = displayedHLV[hoveredListIndex];
                    std::cout << "✅ Selected HLV: " << selectedHLV->getHoTen() << std::endl;
                }
            }
        }
    }
    
    // ✅ CHỈ XỬ LÝ VĂN BẢN KHI ĐÃ FOCUS
    if (searchInput.getFocus()) {
        if (event.getIf<sf::Event::TextEntered>()) {
            searchInput.handleEvent(event);
            applySearchFilter();
        }
        
        if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            searchInput.handleEvent(event);
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                applySearchFilter();
            }
        }
    }
}

void DatHLVPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    
    BasePopup::update(mousePos);
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
    searchInput.update(sf::Time::Zero);
    
    // ✅ CHỈ CẬP NHẬT hoveredListIndex, KHÔNG TẠO TOOLTIP
    hoveredListIndex = -1;

    sf::FloatRect listBounds = listContainer.getGlobalBounds();
    if (listBounds.contains(sf::Vector2f(mousePos))) {
        float listY = listBounds.position.y + 10;
        float rowHeight = 30;

        for (size_t i = 0; i < displayedHLV.size(); ++i) {
            sf::FloatRect rowBounds(
                {listBounds.position.x, listY + i * rowHeight},
                {listBounds.size.x, rowHeight}
            );
            
            if (rowBounds.contains(sf::Vector2f(mousePos))) {
                hoveredListIndex = i;
                break; // ✅ Tìm thấy rồi thì thoát
            }
        }
    }
}

void DatHLVPopup::drawContent(sf::RenderTarget& target) {
    searchInput.draw(target);
    target.draw(listContainer);
    
    // Vẽ danh sách HLV
    sf::FloatRect bounds = listContainer.getGlobalBounds();
    float listX = bounds.position.x + 10;
    float listY = bounds.position.y + 10;
    float rowHeight = 30;

    for (size_t i = 0; i < displayedHLV.size(); ++i) {
        if (listY + i * rowHeight > bounds.position.y + bounds.size.y - rowHeight) {
            break;
        }
        
        HLV* hlv = displayedHLV[i];
        
        std::string hlvInfo = hlv->getID() + ": " + hlv->getHoTen();
        sf::Text hlvName(font, sf::String::fromUtf8(hlvInfo.begin(), hlvInfo.end()), 16);
        hlvName.setPosition(sf::Vector2f(listX, listY + i * rowHeight));

        // Highlight
        if (hlv == selectedHLV) {
            hlvName.setFillColor(Config::Warning);
            hlvName.setStyle(sf::Text::Bold);
        } else if (i == hoveredListIndex) {
            hlvName.setFillColor(Config::AccentCyan);
        } else {
            hlvName.setFillColor(Config::TextNormal);
        }
        
        target.draw(hlvName);
    }
    
    target.draw(errorMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);
    
    // ✅ BỎ: target.draw(tooltip);
}

// void DatHLVPopup::handleSubmit() {
//     if (selectedHLV == nullptr) {
//         errorMessage.setString("Vui long chon mot HLV");
//         return;
//     }
//     if (currentHoiVien->getSoBuoiPT() <= 0) {
//         errorMessage.setString("Hoi vien da het buoi tap PT");
//         return;
//     }

//     std::string now = DateTimeUtils::getCurrentDateTime();
//     LogTapPTService::themLogTapPT(currentHoiVien->getID(), selectedHLV->getID(), now);
    
//     // (Quan trong: Ban can dam bao LogTapPTService::themLogTapPT
//     // hoac LogTapPT::create() co goi ham tru so buoi PT)
    
//     hide();
// }