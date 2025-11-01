#ifndef MODERNBUTTON_H
#define MODERNBUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class ModernButton {
private:
    RectangleShape box;
    Text text;
    Color normalColor, hoverColor, clickColor, disabledColor;
    bool isHovered, isClicking, isEnabled;
    Font& font;

public:
    ModernButton(Font& f, const string& buttonText, float x, float y, 
                 float width = 200, float height = 50);
    
    void handleEvent(const Event& event, Vector2f mousePos);
    bool isClicked(const Event& event, Vector2f mousePos);
    void draw(RenderWindow& window);
    void setPosition(float x, float y);
    void setText(const string& newText);
    void setEnabled(bool enabled);
    void setColors(Color normal, Color hover, Color click);
    bool getIsHovered() const;
};

#endif // MODERNBUTTON_H