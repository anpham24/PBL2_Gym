#ifndef MODERNINPUTBOX_H
#define MODERNINPUTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class ModernInputBox {
private:
    RectangleShape box;
    Text text, label, placeholder;
    string inputText;
    bool isActive, isPassword;
    int cursorPosition;
    float accumulatedTime;
    bool cursorVisible, showPlaceholder;
    RectangleShape toggleButton;
    Text toggleText;
    bool showPassword;
    Font& font;

public:
    ModernInputBox(Font& font, const string& labelStr, const string& placeholderStr, 
                   float x, float y, float width = 350, float height = 50, 
                   bool password = false);
    
    void handleEvent(const Event& event);
    void update(float deltaTime);
    void updateDisplayText();
    void draw(RenderWindow& window);
    
    string getText() const;
    void clear();
    void setText(const string& t);
    void setActive(bool active);
    bool getIsActive() const;
    void setSize(Vector2f newSize);
    void setPosition(float x, float y);
};

#endif // MODERNINPUTBOX_H