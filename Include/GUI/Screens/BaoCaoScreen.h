#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "TabManager.h"
#include "InputBox.h"
#include "Button.h"
#include "Selector.h"
#include "Models/NhanVien.h"
#include "BaoCaoService.h" // Include Service
#include <vector>
#include <string>

class BaoCaoScreen : public BaseScreen
{
private:
    sf::Font &font;
    TabManager tabManager;

    // --- Components Bo Loc ---
    InputBox tuNgayInput;
    InputBox denNgayInput;
    Selector<NhanVien> nhanVienSelector;
    Button clearNVButton;
    Button locButton;

    // --- Data Hien Thi ---
    std::vector<sf::Text> thangReportLines;

    // Data bieu do (cache lai ket qua tu Service de ve)
    std::vector<double> chartData;

    // --- Ham Helper UI ---
    void loadReportData(); // Bay gio chi goi Service va cap nhat UI
    void drawBarChart(sf::RenderTarget &target);
    void createReportLine(std::vector<sf::Text> &lines, const std::string &label, const std::string &value, float yPos);

public:
    BaoCaoScreen(App &app);

    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget &target) override;
};