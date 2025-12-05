#pragma once

#include "Selector.h"
#include "SearchService.h"
#include "HoiVien.h"
#include "InputBox.h"

class HoiVienSelector : public Selector<HoiVien> {
public:
    HoiVienSelector(sf::Font& font, const std::string& placeholder);
    void filterList() override;
};