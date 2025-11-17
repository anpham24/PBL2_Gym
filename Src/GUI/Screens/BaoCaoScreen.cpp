// GUI/Screens/BaoCaoScreen.cpp
#include "BaoCaoScreen.h"
#include "QuanLy.h"
#include "HoaDon.h"
#include "HoiVien.h"
#include "GoiTap.h"
#include "HangHoa.h"
#include "ChiTietHoaDon_GT.h"
#include "ChiTietHoaDon_HH.h"
#include "DateTimeUils.h" // Can ham lay ngay hien tai
#include "Validator.h"
#include <map>
#include <vector>
#include <algorithm> // De sort

// --- Ham so sanh cho std::sort (de tim Top 3) ---
template<typename K, typename V>
bool compareMapValueDesc(const std::pair<K, V>& a, const std::pair<K, V>& b) {
    return a.second > b.second;
}


// --- Constructor ---
BaoCaoScreen::BaoCaoScreen(App& app) 
    : BaseScreen(app), 
        font(app.getGlobalFont()),
      tabManager(app.getGlobalFont()),
      tuNgayInput(),
      denNgayInput(),
      nhanVienSelector(app.getGlobalFont(), "Tat ca Nhan Vien"),
      locButton()
{
    float contentX = 250.0f; 
    float pY = 40.0f; // Vi tri Y cho bo loc
    
    // --- Setup Bo loc (o tren cung) ---
    tuNgayInput.setup("Tu ngay (DD/MM/YYYY)",font, false);
    tuNgayInput.setSize(180, 35);
    tuNgayInput.setPosition(contentX, pY);

    denNgayInput.setup("Den ngay (DD/MM/YYYY)", font, false);
    denNgayInput.setSize(180, 35);
    denNgayInput.setPosition(contentX + 190, pY);

    nhanVienSelector.setSize(200, 35);
    nhanVienSelector.setPosition(contentX + 380, pY);
    nhanVienSelector.setItems(
        app.getQuanLy().getDsNhanVien(),
        [](NhanVien* nv){ return nv->getHoTen(); }
    );
    
    locButton.setup("Loc Bao Cao", font);
    locButton.setSize(120, 35);
    locButton.setPosition(contentX + 590, pY);
    locButton.setOnClick([this]() {
        loadReportData();
    });

    // --- Setup Tabs ---
    tabManager.setPosition(contentX, pY + 60);
    tabManager.addTab("Doanh Thu Thang");
    tabManager.addTab("Doanh Thu Nam");
    
    // --- Mac dinh 12 cot cho bieu do ---
    monthlyRevenue.resize(12, 0.0);
    barChart.resize(12);
    // barChartLabels.resize(12);

    // --- Load data lan dau (cho thang hien tai) ---
    std::string today = DateTimeUtils::getCurrentDateTime(); // "DD/MM/YYYY HH:MM"
    std::string currentMonthYear = today.substr(3); // "MM/YYYY HH:MM"
    tuNgayInput.setString("01/" + currentMonthYear.substr(0, 7)); // "01/MM/YYYY"
    denNgayInput.setString(today.substr(0, 10)); // "DD/MM/YYYY"
    
    loadReportData();
}

/**
 * @brief Ham chinh tinh toan du lieu bao cao
 */
void BaoCaoScreen::loadReportData() {
    thangReportLines.clear();
    namReportLines.clear();
    std::fill(monthlyRevenue.begin(), monthlyRevenue.end(), 0.0);

    // Lay du lieu loc
    std::string tuNgay = tuNgayInput.getString();//.toAnsiString();
    std::string denNgay = denNgayInput.getString();//.toAnsiString();
    NhanVien* nvLoc = nhanVienSelector.getSelected();
    
    // (Ban can ham DateUtils::isBetween(hoaDonNgay, tuNgay, denNgay))
    // Neu loc ngay bi loi, return
    if (Validator::validateNgay(tuNgay) != "" || Validator::validateNgay(denNgay) != "") {
        createReportLine(thangReportLines, "Loi:", "Dinh dang ngay khong hop le (DD/MM/YYYY)", 180);
        return;
    }
    
    // --- Data tinh toan ---
    double totalRevenue = 0;
    double revenueGoiTap = 0;
    double revenueHangHoa = 0;
    int newMembers = 0; // (Logic nay phuc tap, tam thoi de 0)

    // Maps de dem Top 3
    std::map<std::string, double> goiTapRevenueMap;
    std::map<std::string, double> hangHoaRevenueMap;
    std::map<std::string, double> nhanVienRevenueMap;

    MyVector<HoaDon*> allHoaDon = app.getQuanLy().getDsHoaDon().getAllValues();

    for (size_t i = 0; i < allHoaDon.size(); ++i) {
        HoaDon* hd = allHoaDon[i];
        if (hd == nullptr || !hd->getDaThanhToan()) continue;

        // Loc theo Nhan Vien (neu co)
        if (nvLoc != nullptr && hd->getNhanVien() != nvLoc) {
            continue;
        }
        
        // Loc theo Ngay
        std::string ngayLap = hd->getNgayLap(); // "DD/MM/YYYY"
        // (Gia su ham so sanh ngay ton tai)
        // if (!DateUtils::isBetween(ngayLap, tuNgay, denNgay)) {
        //     continue;
        // }
        
        // (Gia lap loc ngay don gian: chi lay thang 11/2025)
        if (ngayLap.substr(3) != "11/2025") continue;
        
        // --- Tinh toan ---
        double hdTotal = hd->getTotal();
        totalRevenue += hdTotal;
        
        // Tinh doanh thu cho Nhan Vien
        if (hd->getNhanVien() != nullptr) {
            nhanVienRevenueMap[hd->getNhanVien()->getHoTen()] += hdTotal;
        }

        // Tinh cho bieu do (Tab 2)
        int month = std::stoi(ngayLap.substr(3, 2)); // Lay thang
        monthlyRevenue[month - 1] += hdTotal; // month 1 = index 0

        // Tinh Top 3 Goi Tap / Hang Hoa
        for (size_t j = 0; j < hd->getDsChiTietHoaDon_GT().size(); ++j) {
            auto* ct = hd->getDsChiTietHoaDon_GT()[j];
            double ctTotal = ct->tinhTien();
            revenueGoiTap += ctTotal;
            goiTapRevenueMap[ct->getGoiTap()->getTenGoi()] += ctTotal;
        }
        for (size_t j = 0; j < hd->getDsChiTietHoaDon_HH().size(); ++j) {
            auto* ct = hd->getDsChiTietHoaDon_HH()[j];
            double ctTotal = ct->tinhTien();
            revenueHangHoa += ctTotal;
            hangHoaRevenueMap[ct->getHangHoa()->getTenHH()] += ctTotal;
        }
    }

    // --- Chuyen Maps sang Vectors de sap xep ---
    std::vector<std::pair<std::string, double>> topNV(nhanVienRevenueMap.begin(), nhanVienRevenueMap.end());
    std::vector<std::pair<std::string, double>> topGT(goiTapRevenueMap.begin(), goiTapRevenueMap.end());
    std::vector<std::pair<std::string, double>> topHH(hangHoaRevenueMap.begin(), hangHoaRevenueMap.end());

    std::sort(topNV.begin(), topNV.end(), compareMapValueDesc<std::string, double>);
    std::sort(topGT.begin(), topGT.end(), compareMapValueDesc<std::string, double>);
    std::sort(topHH.begin(), topHH.end(), compareMapValueDesc<std::string, double>);

    // --- Hien thi ket qua (Tab 1) ---
    float pY = 180; // Duoi TabManager
    createReportLine(thangReportLines, "TONG DOANH THU", std::to_string((int)totalRevenue) + " VND", pY);
    createReportLine(thangReportLines, "So thanh vien moi", std::to_string(newMembers), pY + 40);
    
    pY += 80;
    createReportLine(thangReportLines, "Doanh thu Goi Tap", std::to_string((int)revenueGoiTap) + " VND", pY);
    for (size_t i = 0; i < topGT.size() && i < 3; ++i) {
        createReportLine(thangReportLines, "  Top " + std::to_string(i+1), topGT[i].first + " (" + std::to_string((int)topGT[i].second) + ")", pY + 30 + (i*25));
    }

    pY += 120;
    createReportLine(thangReportLines, "Doanh thu Hang Hoa", std::to_string((int)revenueHangHoa) + " VND", pY);
    for (size_t i = 0; i < topHH.size() && i < 3; ++i) {
        createReportLine(thangReportLines, "  Top " + std::to_string(i+1), topHH[i].first + " (" + std::to_string((int)topHH[i].second) + ")", pY + 30 + (i*25));
    }
    
    pY += 120;
    createReportLine(thangReportLines, "Top Nhan Vien", "", pY);
    for (size_t i = 0; i < topNV.size() && i < 3; ++i) {
        createReportLine(thangReportLines, "  Top " + std::to_string(i+1), topNV[i].first + " (" + std::to_string((int)topNV[i].second) + ")", pY + 30 + (i*25));
    }

    // (Logic so sanh thang truoc - tam thoi de trong)
}

void BaoCaoScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());
    
    tabManager.handleEvent(event, mousePos);
    tuNgayInput.handleEvent(event);
    denNgayInput.handleEvent(event);
    nhanVienSelector.handleEvent(event, mousePos);
    locButton.handleEvent(event, mousePos);
}

void BaoCaoScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());
    
    tuNgayInput.update(dt);
    denNgayInput.update(dt);
    nhanVienSelector.update(mousePos);
    locButton.update(mousePos);
}

void BaoCaoScreen::draw(sf::RenderTarget& target) {
    // 1. Ve bo loc (luon hien thi)
    tuNgayInput.draw(target);
    denNgayInput.draw(target);
    nhanVienSelector.draw(target);
    locButton.draw(target);
    
    // 2. Ve tabs
    tabManager.draw(target);

    // 3. Ve noi dung tab
    if (tabManager.getActiveTab() == 0) {
        // Tab Thang
        for (const auto& line : thangReportLines) {
            target.draw(line);
        }
    } else {
        // Tab Nam (Ve Bieu Do)
        drawBarChart(target);
    }
}

/**
 * @brief Ham Helper: Ve bieu do cot
 */
void BaoCaoScreen::drawBarChart(sf::RenderTarget& target) {
    float chartX = 300;
    float chartY = 550;
    float chartWidth = 1000;
    float chartHeight = 350; // Chieu cao toi da cua bieu do
    float barWidth = chartWidth / 12 / 1.5f; // 12 cot, co khoang cach

    // 1. Tim doanh thu max
    double maxRevenue = 0;
    for (double revenue : monthlyRevenue) {
        if (revenue > maxRevenue) maxRevenue = revenue;
    }
    if (maxRevenue == 0) maxRevenue = 1; // Tranh chia cho 0

    sf::Text labelText(font, "", 14); // <--- THÊM DÒNG NÀY
    labelText.setFillColor(Config::TextMuted);
    // 2. Ve 12 cot
    for (int i = 0; i < 12; ++i) {
        double revenue = monthlyRevenue[i];
        float barHeight = (revenue / maxRevenue) * chartHeight;
        float barX = chartX + (i * (chartWidth / 12)) + (barWidth / 2);
        float barY = chartY - barHeight;

        barChart[i].setSize(sf::Vector2f(barWidth, barHeight));
        barChart[i].setPosition(sf::Vector2f(barX, barY));
        barChart[i].setFillColor(Config::AccentCyan);
        
        target.draw(barChart[i]);
        
        // Ve nhan (T1, T2...)
        // barChartLabels[i].setFont(font);
        // barChartLabels[i].setString("T" + std::to_string(i + 1));
        // barChartLabels[i].setCharacterSize(14);
        // barChartLabels[i].setFillColor(Config::TextMuted);
        // barChartLabels[i].setPosition(sf::Vector2f(barX + (barWidth / 2) - 10, chartY + 10));
        // target.draw(barChartLabels[i]);
        labelText.setString("T" + std::to_string(i + 1));
        labelText.setPosition(sf::Vector2f(barX + (barWidth / 2) - 10, chartY + 10));
        target.draw(labelText);
    }
    
    // Ve duong ke chan bieu do
    sf::RectangleShape baseLine(sf::Vector2f(chartWidth, 2));
    baseLine.setPosition(sf::Vector2f(chartX, chartY));
    baseLine.setFillColor(Config::CardLight);
    target.draw(baseLine);
}

/**
 * @brief Ham Helper: Tao va them 2 sf::Text (label va value) vao vector
 */
void BaoCaoScreen::createReportLine(std::vector<sf::Text>& lines, const std::string& label, const std::string& value, float yPos) {
    float labelX = 280; // Cot 1
    float valueX = 550; // Cot 2

    sf::Text labelText(font, sf::String::fromUtf8(label.begin(), label.end()) + ":",18);
    labelText.setFillColor(Config::TextMuted);
    labelText.setPosition(sf::Vector2f(labelX, yPos));
    lines.push_back(labelText);

    sf::Text valueText(font, sf::String::fromUtf8(value.begin(), value.end()),18);
    valueText.setFillColor(Config::TextNormal);
    valueText.setStyle(sf::Text::Bold);
    valueText.setPosition(sf::Vector2f(valueX, yPos));
    lines.push_back(valueText);
}