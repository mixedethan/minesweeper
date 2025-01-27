#include <SFML/Graphics.hpp>
#include <iostream>
#include "WelcomeScreen.h"
using namespace std;
using namespace sf;


    WelcomeScreen::WelcomeScreen(int width, int height) {
        window.create(VideoMode(width, height), "Minesweeper", Style::Close | Style::Titlebar);
        window.setFramerateLimit(60);

        if (!font.loadFromFile("files/font.ttf"))
        {
            cout << "Error loading font file" << endl;
            exit(1);
        }

        welcomeText.setString("WELCOME TO MINESWEEPER!");
        welcomeText.setFont(font);
        welcomeText.setCharacterSize(24);
        welcomeText.setStyle(Text::Underlined | Text::Bold);
        welcomeText.setPosition(width/2.0f - welcomeText.getLocalBounds().width/2.0f, height/2.0f - 150);

        enterNameText.setString("Enter your name: ");
        enterNameText.setFont(font);
        enterNameText.setCharacterSize(20);
        enterNameText.setPosition(width/2.0f - enterNameText.getLocalBounds().width/2.0f, height/2.0f - 75);
        enterNameText.setStyle(Text::Bold);

        nameText.setFont(font);
        nameText.setCharacterSize(18);
        nameText.setPosition(width/2.0f - enterNameText.getLocalBounds().width/2.0f + enterNameText.getLocalBounds().width, height/2.0f - 73); //Or 75
        nameText.setFillColor(Color::Yellow);

        cursor.setSize(Vector2f(5.0f, 20.0f));
        cursor.setFillColor(Color::White);
    }

    void WelcomeScreen::run()
    {
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                handleEvent(event);
            }

            update();
            draw();
        }
    }

    void WelcomeScreen::handleEvent(const Event& event)
    {
        if (event.type == Event::Closed)
        {
            window.close();
            exit(0);
        }
        else if (event.type == Event::TextEntered)
        {
            if (event.text.unicode == 8 && nameText.getString().getSize() > 0) // backspace, deletes previous character
            {
                string str = nameText.getString();
                str.erase(str.end() - 1);
                nameText.setString(str);
            }
            else if (event.text.unicode >= 97 && event.text.unicode <= 122 && nameText.getString().getSize() < 10) // alphabets only
            {
                string str = nameText.getString();
                if (str.empty())
                {
                    str += toupper(static_cast<char>(event.text.unicode));
                }
                else
                {
                    str += tolower(static_cast<char>(event.text.unicode));
                }
                nameText.setString(str);
            }

        }
        else if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Enter && nameText.getString().getSize() > 0) // Enter key
            {
                // Save name and close the window
                playerName = nameText.getString();
                window.close();
            }
        }
    }

    void WelcomeScreen::update()
    {
        cursor.setPosition(nameText.getPosition().x + nameText.getLocalBounds().width, nameText.getPosition().y + 2); //Can remove + 2
    }

    void WelcomeScreen::draw()
    {
        window.clear(Color::Blue);
        window.draw(welcomeText);
        window.draw(enterNameText);
        window.draw(nameText);
        window.draw(cursor);
        window.display();
    }
