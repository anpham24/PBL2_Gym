// GUI/Sidebar.h
#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
// #include "MainScreen.h" // De lay enum ContentScreenType
#include "App.h"               // De lay thong tin Account
#include <vector>

class MainScreen;
class Sidebar {
private:
    MainScreen& mainScreen; // De goi ham changeContentScreen
    App& app;               // De lay thong tin user va logout
    
    sf::RectangleShape background; // Nen cua sidebar
    sf::Text* title;
    sf::Text* staffName;
    
    // Su dung std::vector thay vi MyVector vi Button khong phai con tro
    std::vector<Button> navButtons; 
    Button logoutButton;
    
    // Con tro den nut dang duoc kich hoat (de to mau khac)
    Button* activeButton;
    
    float sidebarWidth;

public:
    Sidebar(MainScreen& screen, App& app);
    ~Sidebar();
    void setup(Account* currentUser);
    
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderTarget& target);

    float getWidth() const;
};