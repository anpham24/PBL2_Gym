// GUI/Popups/ConfirmDeletePopup.h
#pragma once

#include "BasePopup.h"
#include <functional> // De su dung std::function

/*
 * Lop ConfirmDeletePopup la mot popup chung de xac nhan hanh dong xoa.
 * No ke thua BasePopup va them cac nut "Xac Nhan", "Huy".
 */
class ConfirmDeletePopup : public BasePopup {
private:
    sf::Text mainMessage;     // "Ban co chac muon xoa..."
    sf::Text warningMessage;  // "Hoi vien nay con dang hoat dong" (Tuy chon)
    
    Button confirmButton;
    Button cancelButton;

    // Ham callback se duoc goi khi nguoi dung nhan "Xac Nhan"
    std::function<void()> onConfirmCallback;

protected:
    /**
     * @brief Ve noi dung cua popup nay (cac nut va message).
     */
    void drawContent(sf::RenderTarget& target) override;

public:
    /**
     * @brief Ham dung.
     * @param app Tham chieu den App chinh.
     */
    ConfirmDeletePopup(App& app);
    
    /**
     * @brief Hien thi popup xac nhan xoa.
     * @param itemName Ten cua doi tuong (VD: "Hoi Vien HV001").
     * @param onConfirm Ham se duoc goi khi nhan "Xac Nhan".
     * @param warning (Tuy chon) Mot canh bao phu mau do.
     */
    void show(const std::string& itemName, std::function<void()> onConfirm, const std::string& warning = "");

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};