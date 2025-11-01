#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

class DropdownMenu {
private:
    RectangleShape box;
    Text currentText, label;
    vector<string> options;
    vector<RectangleShape> optionBoxes;
    vector<Text> optionTexts;
    int selectedIndex;
    bool isExpanded;
    Font& font;
    float boxX, boxY, boxWidth, boxHeight;

public:
    DropdownMenu(Font& f, const string& labelStr, const vector<string>& opts,
                 float x, float y, float width = 350, float height = 50);
    
    void handleEvent(const Event& event, Vector2f mousePos);
    void draw(RenderWindow& window);
    
    int getSelectedIndex() const;
    string getSelectedValue() const;
    void setSelectedIndex(int idx);
    void setSize(Vector2f newSize);
    void setPosition(float x, float y);
    void updateOptions(const vector<string>& newOptions);
    bool isOpen() const;
};

#endif // DROPDOWNMENU_H