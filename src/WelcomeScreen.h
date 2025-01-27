#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class WelcomeScreen {
public:
    RenderWindow window;
    Font font;
    Text welcomeText;
    Text enterNameText;
    Text nameText;
    RectangleShape cursor;
    string playerName;

    WelcomeScreen(int width, int height);
    void run();
    void handleEvent(const Event& event);
    void update();
    void draw();


};