// GUI/Screens/BaseScreen.h
// Lop truu tuong cho tat ca cac man hinh
#pragma once
#include <SFML/Graphics.hpp>

class App; // Forward declaration

class BaseScreen {
protected:
    App& app; // De man hinh co the truy cap App (vi du: doi screen)
public:
    BaseScreen(App& app) : app(app) {}
    virtual ~BaseScreen() {}

    virtual void handleEvent(sf::Event event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;
};