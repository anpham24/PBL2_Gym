// GUI/Components/Pagination.cpp
#include "Pagination.h"
#include <string>

Pagination::Pagination(sf::Font& font) 
    : font(font), 
      position(0, 0),
      currentPage(1), 
      totalPages(1), 
      itemsPerPage(10),
      hoveredPageIndex(-1),
      // SỬA Ở ĐÂY: Khởi tạo tất cả Text ngay tại đây
      firstPageText(createNavText(font, "<<", true)),
      prevPageText(createNavText(font, "<", true)),
      nextPageText(createNavText(font, ">", true)),
      lastPageText(createNavText(font, ">>", true)),
      pageStatusText(createNavText(font, "Trang 1 / 1", false))
{
    // XÓA HẾT 5 DÒNG GÁN "firstPageText = ..." Ở TRONG NÀY
}

Pagination::~Pagination() {}

sf::Text Pagination::createNavText(sf::Font& font, const std::string& str, bool isInteractive) {
    // SỬA Ở ĐÂY: Đổi thứ tự (font) lên trước (string)
    sf::Text text(font, sf::String::fromUtf8(str.begin(), str.end()), 16); 

    if (isInteractive) {
        text.setFillColor(Config::AccentCyan);
    } else {
        text.setFillColor(Config::TextMuted);
    }
    return text;
}

void Pagination::setup(int totalItems, int itemsPerPage) {
    this->itemsPerPage = (itemsPerPage > 0) ? itemsPerPage : 1;
    
    // Tinh toan tong so trang
    this->totalPages = (totalItems + this->itemsPerPage - 1) / this->itemsPerPage;
    if (this->totalPages == 0) {
        this->totalPages = 1; // Luon co it nhat 1 trang
    }
    
    // Dam bao trang hien tai hop le
    if (this->currentPage > this->totalPages) {
        this->currentPage = this->totalPages;
    }
    
    updateVisuals();
}

void Pagination::setOnPageChange(std::function<void(int)> func) {
    onPageChange = func;
}

void Pagination::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    updateVisuals(); // Cap nhat lai vi tri cac text
}

void Pagination::setCurrentPage(int page) {
    int oldPage = currentPage;

    // Gioi han gia tri
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    currentPage = page;
    
    // Neu trang thuc su thay doi
    if (oldPage != currentPage) {
        if (onPageChange) {
            onPageChange(currentPage); // Goi callback
        }
        updateVisuals(); // Ve lai cac so trang
    }
}

int Pagination::getCurrentPage() const {
    return currentPage;
}

int Pagination::getTotalPages() const {
    return totalPages;
}

void Pagination::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    // *** SFML 3.0 API CHANGE ***
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            
            int clickedIndex = getHoveredIndex(mousePos);
            if (clickedIndex == -1) return; // Khong click vao dau
            
            // 0 -> 3 la cac nut dieu khien
            if (clickedIndex == 0) { // First "<<"
                setCurrentPage(1);
            } else if (clickedIndex == 1) { // Prev "<"
                setCurrentPage(currentPage - 1);
            } else if (clickedIndex == (pageNumberTexts.size() + 2)) { // Next ">"
                setCurrentPage(currentPage + 1);
            } else if (clickedIndex == (pageNumberTexts.size() + 3)) { // Last ">>"
                setCurrentPage(totalPages);
            }
            // Con lai la cac so trang
            else if (clickedIndex >= 2 && (clickedIndex - 2) < pageNumberTexts.size()) {
                sf::Text& clickedText = pageNumberTexts[clickedIndex - 2];
                sf::String pageStr = clickedText.getString(); // Day la sf::String
                
                if (pageStr != "...") {
                    try {
                        // SFML 3.0: Chuyen sf::String sang std::string de stoi
                        setCurrentPage(std::stoi(pageStr.toAnsiString()));
                    } catch (...) {
                        // Khong lam gi neu khong phai la so
                    }
                }
            }
        }
    }
}

void Pagination::update(sf::Vector2i mousePos) {
    hoveredPageIndex = getHoveredIndex(mousePos);
    
    // Cap nhat mau sac khi hover
    sf::Text* allTexts[] = {&firstPageText, &prevPageText, &nextPageText, &lastPageText};
    
    // 1. Reset mau cac nut dieu khien
    for (int i = 0; i < 4; ++i) {
        allTexts[i]->setFillColor(Config::AccentCyan);
    }
    // 2. Reset mau cac so trang
    for (auto& txt : pageNumberTexts) {
        txt.setFillColor(Config::AccentCyan);
    }
    
    // 3. To mau muc dang hover
    if (hoveredPageIndex != -1) {
        if (hoveredPageIndex == 0) firstPageText.setFillColor(Config::Warning);
        else if (hoveredPageIndex == 1) prevPageText.setFillColor(Config::Warning);
        else if (hoveredPageIndex == (pageNumberTexts.size() + 2)) nextPageText.setFillColor(Config::Warning);
        else if (hoveredPageIndex == (pageNumberTexts.size() + 3)) lastPageText.setFillColor(Config::Warning);
        else if (hoveredPageIndex >= 2 && (hoveredPageIndex - 2) < pageNumberTexts.size()) {
            if (pageNumberTexts[hoveredPageIndex - 2].getString() != "...") {
                pageNumberTexts[hoveredPageIndex - 2].setFillColor(Config::Warning);
            }
        }
    }
    
    // 4. Vo hieu hoa (lam mo) cac nut dau/cuoi
    if (currentPage == 1) {
        firstPageText.setFillColor(Config::TextMuted);
        prevPageText.setFillColor(Config::TextMuted);
    }
    if (currentPage == totalPages) {
        nextPageText.setFillColor(Config::TextMuted);
        lastPageText.setFillColor(Config::TextMuted);
    }

    // 5. To mau dac biet cho trang hien tai
    for (auto& txt : pageNumberTexts) {
        // SFML 3.0: So sanh sf::String voi std::string
        if (txt.getString() == std::to_string(currentPage)) {
            txt.setFillColor(Config::Warning);
            txt.setStyle(sf::Text::Bold | sf::Text::Underlined);
        } else {
            txt.setStyle(sf::Text::Regular);
        }
    }
}

void Pagination::draw(sf::RenderTarget& target) {
    target.draw(firstPageText);
    target.draw(prevPageText);
    
    for (const auto& txt : pageNumberTexts) {
        target.draw(txt);
    }
    
    target.draw(nextPageText);
    target.draw(lastPageText);
    target.draw(pageStatusText);
}

// --- Ham Helper ---

int Pagination::getHoveredIndex(sf::Vector2i mousePos) {
    sf::Vector2f fMousePos(mousePos);

    if (firstPageText.getGlobalBounds().contains(fMousePos)) return 0;
    if (prevPageText.getGlobalBounds().contains(fMousePos)) return 1;
    
    for (size_t i = 0; i < pageNumberTexts.size(); ++i) {
        if (pageNumberTexts[i].getGlobalBounds().contains(fMousePos)) {
            return i + 2; // 0 va 1 da duoc dung
        }
    }
    
    size_t baseIndex = pageNumberTexts.size() + 2;
    if (nextPageText.getGlobalBounds().contains(fMousePos)) return baseIndex;
    if (lastPageText.getGlobalBounds().contains(fMousePos)) return baseIndex + 1;

    return -1; // Khong hover
}

void Pagination::updateVisuals() {
    float currentX = position.x;

    // --- Dat vi tri cho 2 nut dau ---
    firstPageText.setPosition(sf::Vector2f(currentX, position.y));
    currentX += firstPageText.getGlobalBounds().size.x + 20;

    prevPageText.setPosition(sf::Vector2f(currentX, position.y));
    currentX += prevPageText.getGlobalBounds().size.x + 30; // Khoang cach lon

    // --- Tinh toan cac so trang dong ---
    pageNumberTexts.clear();
    
    // Logic hien thi 1 ... 5 6 7 ... 100
    int start = std::max(1, currentPage - 2);
    int end = std::min(totalPages, currentPage + 2);

    if (start > 1) {
        pageNumberTexts.push_back(createNavText(font, "1", true));
        if (start > 2) {
            pageNumberTexts.push_back(createNavText(font, "...", false));
        }
    }

    for (int i = start; i <= end; ++i) {
        pageNumberTexts.push_back(createNavText(font, std::to_string(i), true));
    }

    if (end < totalPages) {
        if (end < totalPages - 1) {
            pageNumberTexts.push_back(createNavText(font, "...", false));
        }
        pageNumberTexts.push_back(createNavText(font, std::to_string(totalPages), true));
    }
    
    // --- Dat vi tri cho cac so trang ---
    for (auto& txt : pageNumberTexts) {
        txt.setPosition(sf::Vector2f(currentX, position.y));
        currentX += txt.getGlobalBounds().size.x + 20;
    }

    // --- Dat vi tri cho 2 nut cuoi ---
    currentX += 10; // Khoang cach lon
    nextPageText.setPosition(sf::Vector2f(currentX, position.y));
    currentX += nextPageText.getGlobalBounds().size.x + 20;

    lastPageText.setPosition(sf::Vector2f(currentX, position.y));
    currentX += lastPageText.getGlobalBounds().size.x + 50; // Khoang cach lon

    // --- Dat vi tri cho Status ---
    std::string status = "Trang " + std::to_string(currentPage) + " / " + std::to_string(totalPages);
    pageStatusText.setString(sf::String::fromUtf8(status.begin(), status.end()));
    pageStatusText.setPosition(sf::Vector2f(currentX, position.y));
}