#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class LeaderboardScreen{
public:
    RenderWindow window;
    Font font;
    Text leaderBoardText;
    Text nameText;
    string playerName;
    string names;
    int _width;
    int _height;

    LeaderboardScreen(int width, int height,const vector<string>& indvNames);
    void run();
    void handleEvent(const Event& event);
    void draw();

};