// GUI/Screens/BaoCaoScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "TabManager.h"
#include "InputBox.h"
#include "Button.h"
#include "Selector.h"
#include "Models/NhanVien.h"
#include <vector>
#include <string>

/*
 * Lop BaoCaoScreen hien thi cac thong ke doanh thu.
 * Day la man hinh "Chi Doc" (Read-Only) va chi danh cho Admin.
 */
class BaoCaoScreen : public BaseScreen {
private:
    sf::Font& font;
    TabManager tabManager;

    // --- Components Bo Loc (Filters) ---
    InputBox tuNgayInput;
    InputBox denNgayInput;
    Selector<NhanVien> nhanVienSelector;
    Button locButton;

    // --- Data Da Xu Ly (de ve) ---
    // Tab 1 (Thang)
    std::vector<sf::Text> thangReportLines; // Cac dong text (VD: "Tong doanh thu: 50000")
    std::vector<sf::Text> thangPreviousLines; // Text so sanh voi thang truoc
    
    // Tab 2 (Nam)
    std::vector<sf::Text> namReportLines;
    // Data cho bieu do cot (12 thang)
    std::vector<double> monthlyRevenue; 
    std::vector<sf::RectangleShape> barChart; // Cac cot
    std::vector<sf::Text> barChartLabels; // Nhan (T1, T2...)

    /**
     * @brief Ham chinh: Goi QuanLy, tinh toan tat ca so lieu, va cap nhat UI.
     */
    void loadReportData();
    
    /**
     * @brief Ham ve bieu do cot (Tab 2).
     */
    void drawBarChart(sf::RenderTarget& target);
    
    /**
     * @brief Ham helper de tao text va them vao vector.
     */
    void createReportLine(std::vector<sf::Text>& lines, const std::string& label, const std::string& value, float yPos);
    
    // (Ban se can them mot DateUtils::isBetween de so sanh ngay)

public:
    BaoCaoScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};