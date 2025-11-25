// Config.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

/*
 * File Config chua tat ca cac hang so cua ung dung,
 * bao gom mau sac, font chu, va cac quy tac nghiep vu.
 */

namespace Config {

    // ========================================================================
    // GIAO DIEN (UI)
    // ========================================================================

    // Kich thuoc cua so
    const unsigned int WINDOW_WIDTH = 1600;
    const unsigned int WINDOW_HEIGHT = 900;

    // Duong dan Font
    // BAN PHAI TAO MOT THU MUC "assets/fonts/" VA DAT FONT NAY VAO
    const std::string FONT_PATH = "assets/fonts/Roboto_Condensed-Regular.ttf";

    // Bang mau (Palette)
    const sf::Color NavyBackground(30, 39, 46); // Nen chinh
    const sf::Color CardDark(47, 54, 64);       // Nen phu (Sidebar, Panel)
    const sf::Color CardLight(53, 61, 72);      // Nen sang hon (Input, Table row)
    const sf::Color AccentCyan(0, 184, 148);    // Mau nhan (Nut, Highlight)
    const sf::Color TextNormal(223, 228, 234);  // Chu trang
    const sf::Color TextMuted(109, 117, 125);   // Chu mo (placeholder)
    const sf::Color Success(46, 204, 113);      // Mau thanh cong
    const sf::Color Danger(231, 76, 60);        // Mau loi, xoa
    const sf::Color Warning(241, 196, 15);      // Mau canh bao, sua

    // ========================================================================
    // QUY TAC NGHIEP VU (BUSINESS LOGIC)
    // ========================================================================

    // --- Hoi Vien & Rank ---
    
    // So tien (VND) de doi lay 1 point
    const double MONEY_TO_POINT_RATIO = 1000.0; // 1000 VND = 1 point

    // Dinh nghia cac muc Rank
    enum Rank {
        BRONZE,
        SILVER,
        GOLD,
        PLATINUM
    };

    // So point toi thieu de dat duoc Rank
    const std::map<Rank, int> RANK_MIN_POINTS = {
        {Rank::BRONZE, 0},
        {Rank::SILVER, 5000},
        {Rank::GOLD, 15000},
        {Rank::PLATINUM, 30000}
    };

    // Giam gia hoa don theo Rank (0.05 = 5%)
    const std::map<Rank, double> RANK_DISCOUNT = {
        {Rank::BRONZE, 0.0},
        {Rank::SILVER, 0.03},
        {Rank::GOLD, 0.05},
        {Rank::PLATINUM, 0.08}
    };

    // --- Luong & Hoa Hong ---

    // Hoa hong cho Nhan Vien (Staff) khi ban hang (0.05 = 5%)
    const double COMMISSION_RATE_STAFF = 0.05;

    // Luong cho HLV (theo buoi)
    const double HLV_LUONG_PT_MOI_BUOI = 50000.0;      // 50k/buổi PT
    const double HLV_LUONG_LOP_MOI_BUOI = 100000.0;
}