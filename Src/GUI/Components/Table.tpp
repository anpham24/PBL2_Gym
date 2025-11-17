// // GUI/Components/Table.tpp
// // (Ban phai tao file nay va dat code duoi day vao)

// // --- CONSTRUCTOR & DESTRUCTOR ---
// template <typename T>
// Table<T>::Table(sf::Font& font, float rowHeight) 
//     : font(font), rowHeight(rowHeight), headerHeight(45.0f), 
//       tableWidth(0), hoveredRow(-1), position(0, 0) 
// {
//     headerBg.setFillColor(Config::CardDark);
//     rowBg.setFillColor(Config::CardLight);
//     rowBgAlt.setFillColor(Config::CardDark); // Mau xen ke
    
//     headerText.setFont(font);
//     headerText.setCharacterSize(16);
//     headerText.setFillColor(Config::TextMuted); // Mau chu header
//     headerText.setStyle(sf::Text::Bold);

//     cellText.setFont(font);
//     cellText.setCharacterSize(16);
//     cellText.setFillColor(Config::TextNormal);
// }

// template <typename T>
// Table<T>::~Table() {
//     // Khong lam gi, vi Table khong so huu cac con tro T*
// }

// // --- SETUP ---
// template <typename T>
// void Table<T>::addColumn(const std::string& header, float width, std::function<std::string(const T*)> extractor) {
//     dataColumns.push_back({header, width, extractor});
//     tableWidth += width;
// }

// template <typename T>
// void Table<T>::addAction(const std::string& actionName, std::function<void(T*)> onClick, sf::Color color, float width) {
//     actionColumns.push_back({actionName, width, onClick, color});
//     tableWidth += width;
// }

// // Cac ham tien ich
// template <typename T>
// void Table<T>::addViewAction(std::function<void(T*)> onClick) {
//     addAction("Xem", onClick, Config::AccentCyan, 60.0f);
// }

// template <typename T>
// void Table<T>::addEditAction(std::function<void(T*)> onClick) {
//     addAction("Sua", onClick, Config::Warning, 60.0f);
// }

// template <typename T>
// void Table<T>::addDeleteAction(std::function<void(T*)> onClick) {
//     addAction("Xoa", onClick, Config::Danger, 60.0f);
// }

// template <typename T>
// void Table<T>::setPosition(float x, float y) {
//     position = sf::Vector2f(x, y);
// }

// template <typename T>
// void Table<T>::setData(const MyVector<T*>& data) {
//     displayedItems.clear();
//     // Sao chep con tro tu MyVector sang std::vector noi bo
//     for (T* item : data) {
//         displayedItems.push_back(item);
//     }
// }

// // --- EVENT & UPDATE ---
// template <typename T>
// void Table<T>::handleEvent(sf::Event event, sf::Vector2i mousePos) {
//     if (event.type != sf::Event::MouseButtonPressed || event.mouseButton.button != sf::Mouse::Left) {
//         return;
//     }
    
//     if (hoveredRow < 0 || hoveredRow >= displayedItems.size()) {
//         return; // Click ben ngoai cac hang
//     }

//     // Tinh toan xem click vao cot hanh dong nao
//     float currentX = position.x;
//     T* clickedItem = displayedItems[hoveredRow];
    
//     // Bo qua cac cot du lieu
//     for (const auto& col : dataColumns) {
//         currentX += col.width;
//     }
    
//     // Kiem tra cac cot hanh dong
//     for (const auto& actionCol : actionColumns) {
//         if (mousePos.x >= currentX && mousePos.x < (currentX + actionCol.width)) {
//             // Click trung vao cot hanh dong nay!
//             if (actionCol.onActionClick) {
//                 actionCol.onActionClick(clickedItem); // Goi callback
//             }
//             return; // Chi xu ly mot hanh dong moi lan click
//         }
//         currentX += actionCol.width;
//     }
// }

// template <typename T>
// void Table<T>::update(sf::Vector2i mousePos) {
//     // Tinh toan hang dang duoc hover
//     if (mousePos.x < position.x || mousePos.x > (position.x + tableWidth)) {
//         hoveredRow = -1; // Nam ngoai chieu rong bang
//         return;
//     }
    
//     float yStart = position.y + headerHeight; // Vi tri Y cua hang 0
//     if (mousePos.y < yStart) {
//         hoveredRow = -1; // Nam tren header
//         return;
//     }

//     int row = static_cast<int>((mousePos.y - yStart) / rowHeight);
    
//     if (row >= 0 && row < displayedItems.size()) {
//         hoveredRow = row;
//     } else {
//         hoveredRow = -1; // Nam ben duoi hang cuoi cung
//     }
// }

// // --- DRAW ---
// template <typename T>
// void Table<T>::draw(sf::RenderTarget& target) {
//     float currentX = position.x;
//     float currentY = position.y;

//     // 1. Ve Header
//     headerBg.setSize(sf::Vector2f(tableWidth, headerHeight));
//     headerBg.setPosition(position);
//     target.draw(headerBg);
    
//     // Ve ten cac cot data
//     for (const auto& col : dataColumns) {
//         drawCell(target, col.headerName, sf::Vector2f(currentX, currentY), col.width, Config::TextMuted);
//         currentX += col.width;
//     }
//     // Ve ten cac cot action
//     for (const auto& col : actionColumns) {
//         drawCell(target, col.actionName, sf::Vector2f(currentX, currentY), col.width, Config::TextMuted);
//         currentX += col.width;
//     }

//     // 2. Ve cac hang du lieu
//     currentY += headerHeight;
//     for (size_t i = 0; i < displayedItems.size(); ++i) {
//         currentX = position.x;
//         T* item = displayedItems[i];
        
//         // Chon mau hang (xen ke)
//         sf::RectangleShape& bg = (i % 2 == 0) ? rowBg : rowBgAlt;
//         bg.setSize(sf::Vector2f(tableWidth, rowHeight));
//         bg.setPosition(currentX, currentY);
        
//         // Highlight hang duoc hover
//         if (i == hoveredRow) {
//             bg.setFillColor(Config::CardLight); // Mau sang hon
//             bg.setOutlineColor(Config::AccentCyan); // Vien
//             bg.setOutlineThickness(1);
//         } else {
//             bg.setOutlineThickness(0); // Tat vien
//         }
        
//         target.draw(bg);
        
//         // Ve du lieu cac cot
//         for (const auto& col : dataColumns) {
//             std::string data = col.dataExtractor(item);
//             drawCell(target, data, sf::Vector2f(currentX, currentY), col.width, Config::TextNormal);
//             currentX += col.width;
//         }
//         // Ve cac cot hanh dong
//         for (const auto& col : actionColumns) {
//             // Ve mau de trong giong nut
//             drawCell(target, col.actionName, sf::Vector2f(currentX, currentY), col.width, col.color);
//             currentX += col.width;
//         }

//         currentY += rowHeight;
//     }
// }

// // Ham tien ich ve 1 cell (co padding)
// template <typename T>
// void Table<T>::drawCell(sf::RenderTarget& target, const std::string& text, sf::Vector2f pos, float width, sf::Color color) {
//     cellText.setString(text);
//     cellText.setFillColor(color);
    
//     // Canh giua cell theo chieu doc
//     float yPos = pos.y + (rowHeight - cellText.getCharacterSize()) / 2.0f - 2.0f;
//     // Canh trai cell theo chieu ngang (co padding)
//     float xPos = pos.x + 10.0f; 

//     cellText.setPosition(xPos, yPos);
//     target.draw(cellText);
// }