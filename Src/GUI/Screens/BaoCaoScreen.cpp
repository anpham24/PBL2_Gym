#include "BaoCaoScreen.h"
#include "DateTimeUils.h"
#include "Validator.h"
#include <iostream>
#include <iomanip> // De format so

BaoCaoScreen::BaoCaoScreen(App &app)
    : BaseScreen(app),
      font(app.getGlobalFont()),
      tabManager(app.getGlobalFont()),
      tuNgayInput(),
      denNgayInput(),
      nhanVienSelector(app.getGlobalFont(), "Tat ca Nhan Vien"),
      clearNVButton(),
      locButton()
{
    float contentX = 250.0f;
    float pY = 40.0f;

    tuNgayInput.setup("Tu ngay (DD/MM/YYYY)", font, false);
    tuNgayInput.setSize(180, 35);
    tuNgayInput.setPosition(contentX, pY);

    denNgayInput.setup("Den ngay (DD/MM/YYYY)", font, false);
    denNgayInput.setSize(180, 35);
    denNgayInput.setPosition(contentX + 190, pY);

    nhanVienSelector.setSize(200, 35);
    nhanVienSelector.setPosition(contentX + 410, pY);
    nhanVienSelector.setItems(
        app.getQuanLy().getDsNhanVien(),
        [](NhanVien *nv)
        { return nv->getHoTen(); });

    clearNVButton.setup("X", font);
    clearNVButton.setSize(35, 35);
    clearNVButton.setPosition(contentX + 615, pY);
    clearNVButton.setColors(Config::Danger, Config::Danger);
    clearNVButton.setOnClick([this]()
                             {
        this->nhanVienSelector.clear(); 
        this->loadReportData(); });

    locButton.setup("Loc Bao Cao", font);
    locButton.setSize(120, 35);
    locButton.setPosition(contentX + 660, pY);
    locButton.setOnClick([this]()
                         { loadReportData(); });

    tabManager.setPosition(contentX, pY + 60);
    tabManager.addTab("Doanh Thu Thang");
    tabManager.addTab("Doanh Thu Nam");

    // Mac dinh thang hien tai
    std::string today = DateTimeUtils::getCurrentDateTime();
    std::string currentMonthYear = today.substr(3);
    tuNgayInput.setString("01/" + currentMonthYear.substr(0, 7));
    denNgayInput.setString(today.substr(0, 10));

    loadReportData();
}

void BaoCaoScreen::loadReportData()
{
    thangReportLines.clear();
    chartData.assign(12, 0.0); // Reset data bieu do

    std::string tuNgay = tuNgayInput.getString();
    std::string denNgay = denNgayInput.getString();
    NhanVien *nv = nhanVienSelector.getSelected();
    std::string nvID = (nv != nullptr) ? nv->getID() : "";

    // === GỌI SERVICE ĐỂ TÍNH TOÁN ===
    ReportResult result = BaoCaoService::getReport(tuNgay, denNgay, nvID);

    // === CẬP NHẬT UI TỪ KẾT QUẢ ===

    if (!result.errorMsg.empty())
    {
        createReportLine(thangReportLines, "Loi", result.errorMsg, 180);
        return;
    }

    // Cache data bieu do
    chartData = result.monthlyRevenue;

    // Hien thi text (Tab 1)
    float pY = 180;
    createReportLine(thangReportLines, "TONG DOANH THU", std::to_string((long long)result.totalRevenue) + " VND", pY);

    pY += 60;
    createReportLine(thangReportLines, "Doanh thu Goi Tap", std::to_string((long long)result.revenueGoiTap) + " VND", pY);
    for (size_t i = 0; i < result.topGoiTap.size() && i < 3; ++i)
    {
        createReportLine(thangReportLines, "  Top " + std::to_string(i + 1),
                         result.topGoiTap[i].name + " (" + std::to_string((long long)result.topGoiTap[i].revenue) + ")",
                         pY + 30 + (i * 25));
    }

    pY += 120;
    createReportLine(thangReportLines, "Doanh thu Hang Hoa", std::to_string((long long)result.revenueHangHoa) + " VND", pY);
    for (size_t i = 0; i < result.topHangHoa.size() && i < 3; ++i)
    {
        createReportLine(thangReportLines, "  Top " + std::to_string(i + 1),
                         result.topHangHoa[i].name + " (" + std::to_string((long long)result.topHangHoa[i].revenue) + ")",
                         pY + 30 + (i * 25));
    }

    pY += 120;
    createReportLine(thangReportLines, "Top Nhan Vien", "", pY);
    for (size_t i = 0; i < result.topNhanVien.size() && i < 3; ++i)
    {
        createReportLine(thangReportLines, "  Top " + std::to_string(i + 1),
                         result.topNhanVien[i].name + " (" + std::to_string((long long)result.topNhanVien[i].revenue) + ")",
                         pY + 30 + (i * 25));
    }
}

void BaoCaoScreen::drawBarChart(sf::RenderTarget &target)
{
    float chartX = 300;
    float chartY = 550;
    float chartWidth = 1000;
    float chartHeight = 350;
    float barWidth = chartWidth / 12 / 1.5f;

    double maxRevenue = 1.0;
    for (double r : chartData)
    {
        if (r > maxRevenue)
            maxRevenue = r;
    }

    sf::RectangleShape bar;
    bar.setFillColor(Config::AccentCyan);

    sf::Text label(font, "", 14);
    label.setFillColor(Config::TextMuted);

    for (int i = 0; i < 12; ++i)
    {
        double revenue = chartData[i];
        float barHeight = (float)((revenue / maxRevenue) * chartHeight);
        float barX = chartX + (i * (chartWidth / 12)) + (barWidth / 2);
        float barY = chartY - barHeight;

        bar.setSize(sf::Vector2f(barWidth, barHeight));
        bar.setPosition(sf::Vector2f(barX, barY));
        target.draw(bar);

        label.setString("T" + std::to_string(i + 1));
        label.setPosition(sf::Vector2f(barX + (barWidth / 2) - 10, chartY + 5));
        target.draw(label);

        // Hien thi gia tri tren cot neu co
        if (revenue > 0)
        {
            label.setString(std::to_string((int)(revenue / 1000)) + "k");
            label.setPosition(sf::Vector2f(barX, barY - 20));
            target.draw(label);
        }
    }

    sf::RectangleShape baseLine(sf::Vector2f(chartWidth, 2));
    baseLine.setPosition(sf::Vector2f(chartX, chartY));
    baseLine.setFillColor(Config::CardLight);
    target.draw(baseLine);
}

// ... (Cac ham handleEvent, update, draw, createReportLine GIU NGUYEN logic cu, chi cap nhat ten bien neu can) ...

void BaoCaoScreen::handleEvent(sf::Event event)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    tabManager.handleEvent(event, mousePos);
    nhanVienSelector.handleEvent(event, mousePos);
    clearNVButton.handleEvent(event, mousePos);

    if (nhanVienSelector.getIsOpen())
        return;

    if (auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            tuNgayInput.setFocus(tuNgayInput.isMouseOver(mousePos));
            denNgayInput.setFocus(denNgayInput.isMouseOver(mousePos));
        }
    }

    // Input
    tuNgayInput.handleEvent(event);
    denNgayInput.handleEvent(event);

    if (auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyEvent->code == sf::Keyboard::Key::Enter)
        {
            if (tuNgayInput.getFocus() || denNgayInput.getFocus())
            {
                loadReportData();
            }
        }
    }

    locButton.handleEvent(event, mousePos);
}

void BaoCaoScreen::update(sf::Time dt)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());
    tuNgayInput.update(dt);
    denNgayInput.update(dt);
    nhanVienSelector.update(mousePos);
    clearNVButton.update(mousePos);
    locButton.update(mousePos);
}

void BaoCaoScreen::draw(sf::RenderTarget &target)
{
    tuNgayInput.draw(target);
    denNgayInput.draw(target);
    nhanVienSelector.draw(target);
    clearNVButton.draw(target);
    locButton.draw(target);
    tabManager.draw(target);

    if (tabManager.getActiveTab() == 0)
    {
        for (const auto &line : thangReportLines)
        {
            target.draw(line);
        }
    }
    else
    {
        drawBarChart(target);
    }
}

void BaoCaoScreen::createReportLine(std::vector<sf::Text> &lines, const std::string &label, const std::string &value, float yPos)
{
    float labelX = 280;
    float valueX = 550;

    sf::Text labelText(font, sf::String::fromUtf8(label.begin(), label.end()) + ":", 18);
    labelText.setFillColor(Config::TextMuted);
    labelText.setPosition(sf::Vector2f(labelX, yPos));
    lines.push_back(labelText);

    sf::Text valueText(font, sf::String::fromUtf8(value.begin(), value.end()), 18);
    valueText.setFillColor(Config::TextNormal);
    valueText.setStyle(sf::Text::Bold);
    valueText.setPosition(sf::Vector2f(valueX, yPos));
    lines.push_back(valueText);
}