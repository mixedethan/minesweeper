#include <SFML/Graphics.hpp>
#include <iostream>
#include "LeaderboardScreen.h"
#include <sstream>
using namespace std;
using namespace sf;


LeaderboardScreen::LeaderboardScreen(int width, int height, const vector<string>& indvNames) {
    window.create(VideoMode(width, height), "Minesweeper", Style::Close | Style::Titlebar);
    window.setFramerateLimit(60);
    _width = width;
    _height = height;

    if (!font.loadFromFile("files/font.ttf")) {
        cout << "Error loading font file" << endl;
        exit(1);
    }

    leaderBoardText.setString("LEADERBOARD");
    leaderBoardText.setFont(font);
    leaderBoardText.setCharacterSize(20);
    leaderBoardText.setStyle(Text::Underlined | Text::Bold);
    leaderBoardText.setPosition(_width/2.0f - leaderBoardText.getLocalBounds().width/2.0f, _height/2.0f - 120);

    stringstream ss;
    for (int i = 0; i < indvNames.size(); i += 2) {
        ss << to_string((i/2)+1) << ".\t" << indvNames[i] << "\t" << indvNames[i+1] << endl;
    }

    nameText.setString(ss.str());
    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setStyle(Text::Bold);
    nameText.setPosition(_width/2.0f - nameText.getLocalBounds().width/2.0f, _height/2.0f - nameText.getLocalBounds().height/2.0f);
    //nameText.setPosition(width/2.0f - nameText.getLocalBounds().width/2.0f, height/2.0f + 120);
}

void LeaderboardScreen::run()
{
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            handleEvent(event);
        }
        window.requestFocus();
        draw();
    }
}

void LeaderboardScreen::handleEvent(const Event& event)
{
    if (event.type == Event::Closed)
    {
        window.close();
    }
}


void LeaderboardScreen::draw()
{
    window.clear(Color::Blue);
    window.draw(leaderBoardText);
    window.draw(nameText);
    window.display();
}
