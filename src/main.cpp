#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "WelcomeScreen.h"
#include "TextureLoader.h"
#include "LeaderboardScreen.h"
#include "Tile.h"
#include "Board.h"
#include <ctime>
#include <random>

#include <unistd.h>

////    Welcome to Ethan's Minesweeper     ////

using namespace sf;
using namespace std;

//TODO: ADD A TIMER
//FIXME: LEADERBOARD
//TODO: ADD VICTORY SCREEN
//TODO: ADD COUNTER
//TODO: ADD PAUSE AND PLAY



//RECURSIVE HELPER FUNCTIONS
void checkWest(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count);
void checkEast(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count);
void checkNorth(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count);
void checkSouth(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count);
void checkDiag(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count);
static mt19937 rng(time(0)); //Random Number Generator

static int intRNG(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
} //Generates a random integer between min and max
static float floatRNG(float min, float max) {
    uniform_real_distribution<float> dist(min, max);
    return dist(rng);
} //Generates a random float between min and max
vector<int> readcfg() { //Reads the board_config.cfg file and returns a vector containing the width, height, and number of mines
    vector<int> vec;
    string temp;
    int columns;
    int rows;
    int numMines;

    fstream inFS;
    inFS.open("files/board_config.cfg");
    if(!inFS.is_open()) {
        cout << "Could not open file board_config.cfg." << endl;
        exit(1);
    }
    getline(inFS, temp);
    columns = stoi(temp); //35
    getline(inFS, temp);
    rows = stoi(temp); //16
    getline(inFS, temp);
    numMines = stoi(temp);
    vec.push_back(columns);
    vec.push_back(rows);
    vec.push_back(numMines);

    return vec;
} //Calculates the width and height by multiplying values found in a .cfg file
vector<string> readLeaderboardText() { //Reads the board_config.cfg file and returns a vector containing the width, height, and number of mines
    vector<string> vec;
    string temp;

    fstream inFS;
    inFS.open("files/leaderboard.txt");
    if(!inFS.is_open()) {
        cout << "Could not open file leaderboard.txt." << endl;
        exit(1);
    }
    while(!inFS.eof()) {
        getline(inFS, temp, ',');
        vec.push_back(temp);
    }
    return vec;
}

// initializer timer sprites
void initTimerSprites(vector<Sprite>& timerSprites, Texture& digitTexture, vector<IntRect>& digitRects, int columns, int rows) {
    for (int i = 0; i < 4; i++) {
        timerSprites.emplace_back(digitTexture, digitRects[0]);  // Start at 0
        timerSprites[i].setPosition(((columns * 32) - 97) + (i * 21), 32 * (rows + 0.5f) + 16);
    }
}

// updates timer sprites based on the time
void updateTimerSprites(vector<Sprite>& timerSprites, vector<IntRect>& digitRects, int timeElapsed) {
    int minutes = (timeElapsed / 60) % 100;
    int seconds = timeElapsed % 60;

    timerSprites[0].setTextureRect(digitRects[minutes / 10]); // First digit (minutes tens place)
    timerSprites[1].setTextureRect(digitRects[minutes % 10]); // Second digit (minutes ones place)
    timerSprites[2].setTextureRect(digitRects[seconds / 10]); // Third digit (seconds tens place)
    timerSprites[3].setTextureRect(digitRects[seconds % 10]); // Fourth digit (seconds ones place)
}


// FIXME: Optimize & simplify the recursive functions
//RECURSIVE HELPERS
void checkWest(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count)
{
    int r;
    r = firstPosition + 1;

    if (r >= 0 && r < columns * rows)
    {
        if ((!(r % columns == 0)) && tiles[r].GetIsMine() == false)
        {
            if (tiles[r].GetNeighborMines() > 0)
            {
                tiles[r].SetIsHiddenFalse();
                if ((r - 1) - columns >= 0)
                {
                    if (tiles[(r - 1) - columns].GetIsHidden() == 1)
                    {
                        checkNorth(tiles, columns, rows, gameOver, r - 1, count);
                    }
                }
                if ((r - 1) + columns < (rows * columns))
                {
                    if (tiles[(r - 1) + columns].GetIsHidden() == 1)
                    {
                        checkSouth(tiles, columns, rows, gameOver, r - 1, count);
                    }
                }
                if ((r - columns) >= 0)
                {
                    if (tiles[(r - columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, r - 1, count);
                    }
                }
                if ((r + columns) < (rows * columns))
                {
                    if (tiles[(r + columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, r - 1, count);
                    }
                }

            }
            else
            {
                tiles[r].SetIsHiddenFalse();
                checkWest(tiles, columns, rows, gameOver, r, count);
                if (r - columns >= 0)
                {
                    if (tiles[(r)-columns].GetIsHidden() == 1)
                    {
                        checkNorth(tiles, columns, rows, gameOver, r, count);
                    }
                }
                if (r + columns < (rows * columns))
                {
                    if (tiles[(r)+columns].GetIsHidden() == 1)
                    {
                        checkSouth(tiles, columns, rows, gameOver, r, count);
                    }
                }
                if ((r - columns) >= 0)
                {
                    if (tiles[(r - columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, r - 1, count);
                    }
                }
                if ((r + columns) < (rows * columns))
                {
                    if (tiles[(r + columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, r - 1, count);
                    }
                }
                if ((r + 1) < (columns * rows) && (r - 1) < (columns * rows) && tiles[r + 1].GetNeighborMines() > 0 && tiles[r - 1].GetNeighborMines() > 0)
                {

                    checkDiag(tiles, columns, rows, gameOver, r, count);
                }
            }

        }
    }
}
void checkEast(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count)
{
    int l, r, u, d;
    l = firstPosition - 1;

    if (l >= 0 && l < columns * rows)
    {

        if (!((l + 1) % columns == 0) && tiles[l].GetIsMine() == false)
        {

            if (tiles[l].GetNeighborMines() > 0)
            {

                tiles[l].SetIsHiddenFalse();

                if ((l + 1) - columns >= 0 && (l + 1) - columns < (rows * columns))
                {

                    if (tiles[(l + 1) - columns].GetIsHidden() == 1)
                    {
                        checkNorth(tiles, columns, rows, gameOver, l + 1, count);
                    }
                }

                if ((l + 1) + columns < (rows * columns))
                {

                    if (tiles[(l + 1) + columns].GetIsHidden() == 1)
                    {

                        checkSouth(tiles, columns, rows, gameOver, l + 1, count);
                    }
                }
                if ((l - columns) >= 0) //had -1
                {
                    if (tiles[(l - columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, l + 1, count);
                    }
                }
                if ((l + columns) < (rows * columns)) //had -1
                {
                    if (tiles[(l + columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, l + 1, count);
                    }
                }

            }
            else
            {
                tiles[l].SetIsHiddenFalse();
                checkEast(tiles, columns, rows, gameOver, l, count);
                if ((l)-columns >= 0)
                {
                    if (tiles[(l)-columns].GetIsHidden() == 1)
                    {
                        checkNorth(tiles, columns, rows, gameOver, l, count);
                    }
                }
                if (l + columns < (rows * columns))
                {
                    if (tiles[(l)+columns].GetIsHidden() == 1)
                    {
                        checkSouth(tiles, columns, rows, gameOver, l, count);
                    }
                }
                if ((l - columns) >= 0) //was (l - columns) - 1
                {
                    if (tiles[(l - columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, l + 1, count);
                    }
                }
                if ((l + columns) < (rows * columns)) //was (l + columns) - 1
                {
                    if (tiles[(l + columns)].GetIsHidden() == 1)
                    {
                        checkDiag(tiles, columns, rows, gameOver, l + 1, count);
                    }
                }
            }
        }
    }
}
void checkNorth(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count)
{
    int l, r, u, d;
    u = firstPosition - columns;
    if (u >= 0 && u < (rows * columns) && tiles[u].GetIsMine() == false)
    {
        if (tiles[u].GetNeighborMines() > 0)
        {
            tiles[u].SetIsHiddenFalse();
            if ((0 <= ((u + columns) - 1) && ((u + columns) - 1) < (columns * rows)) && tiles[(u + columns) - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, u + columns, count);
            }
            if ((0 <= ((u + columns) + 1) && ((u + columns) + 1) < (columns * rows)) && tiles[(u + columns) + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, u + columns, count);
            }
            if (0 <= (u - 1) && (u - 1) > (columns * rows) && tiles[u - 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, u + columns, count);
            }
            if (0 <= (u + 1) && (u + 1) > (columns * rows) && tiles[u + 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, u + columns, count);
            }


        }
        else
        {
            tiles[u].SetIsHiddenFalse();
            checkNorth(tiles, columns, rows, gameOver, u, count);
            if ((0 <= (u - 1) && (u - 1) < (columns * rows)) && tiles[(u) - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, u, count);
            }
            if ((0 <= (u + 1) && (u + 1) < (columns * rows)) && tiles[(u) + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, u, count);
            }
            if (0 <= (u - 1) && (u - 1) > (columns * rows) && tiles[u - 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, u + columns, count);
            }
            if (0 <= (u + 1) && (u + 1) > (columns * rows) && tiles[u + 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, u + columns, count);
            }
        }
    }
}
void checkSouth(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count)
{
    int l, r, u, d;
    d = firstPosition + columns;
    if (d >= 0 && d < (rows * columns) && tiles[d].GetIsMine() == false)
    {
        if (tiles[d].GetNeighborMines() > 0)
        {
            tiles[d].SetIsHiddenFalse();
            if ((0 <= (d - columns) - 1 && (d - columns) - 1 < (columns * rows)) && tiles[(d - columns) - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, d - columns, count);

            }
            if ((0 <= (d - columns) + 1 && (d - columns) + 1 < (columns * rows)) && tiles[(d - columns) + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, d - columns, count);
            }
            if (0 <= (d - 1) && (d - 1) > (columns * rows) && tiles[d - 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, d - columns, count);
            }
            if (0 <= (d + 1) && (d + 1) > (columns * rows) && tiles[d + 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, d - columns, count);
            }
        }
        else
        {
            tiles[d].SetIsHiddenFalse();
            checkSouth(tiles, columns, rows, gameOver, d, count);
            if ((0 <= d - 1 && d - 1 < (columns * rows)) && tiles[(d) - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, d, count);
            }
            if ((0 <= d + 1 && d + 1 < (columns * rows)) && tiles[(d) + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, d, count);
            }
            if (0 <= (d - 1) && (d - 1) > (columns * rows) && tiles[d - 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, d - columns, count);
            }
            if (0 <= (d + 1) && (d + 1) > (columns * rows) && tiles[d + 1].GetIsHidden() == 1)
            {
                checkDiag(tiles, columns, rows, gameOver, d - columns, count);
            }
            if ((d + 1) < (columns * rows) && (d + columns) < (columns * rows) && tiles[d + 1].GetNeighborMines() > 0 && tiles[d + columns].GetNeighborMines() > 0)
            {

                checkDiag(tiles, columns, rows, gameOver, d, count);
            }
        }
    }


}
void checkDiag(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count)
{
    int l, r, u, d, z;
    //TOP RIGHT
    z = (firstPosition - columns) + 1;
    if (z >= 0 && z < (rows * columns) && (!(z % columns == 0)) && tiles[z].GetIsMine() == false && tiles[z].GetIsHidden() == 1)
    {
        if (tiles[z].GetNeighborMines() > 0)
        {
            tiles[z].SetIsHiddenFalse();
            if ((0 <= (firstPosition - columns) && (firstPosition - columns) < (columns * rows)) && tiles[firstPosition - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + columns) && (firstPosition + columns) < (columns * rows)) && tiles[firstPosition + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + 1) && (firstPosition + 1) < (columns * rows)) && tiles[firstPosition + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition - 1) && (firstPosition - 1) < (columns * rows)) && tiles[firstPosition - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, firstPosition, count);
            }
        }
        else
        {
            tiles[z].SetIsHiddenFalse();
            checkDiag(tiles, columns, rows, gameOver, z, count);
            if ((0 <= (z - columns) && (z - columns) < (columns * rows)) && tiles[z - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + columns) && (z + columns) < (columns * rows)) && tiles[z + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + 1) && (z + 1) < (columns * rows)) && tiles[z + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z - 1) && (z - 1) < (columns * rows)) && tiles[z - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, z, count);
            }


        }
    }
    //TOP LEFT
    z = (firstPosition - columns) - 1;
    if (z >= 0 && z < (rows * columns) && (!((z + 1) % columns == 0)) && tiles[z].GetIsMine() == false && tiles[z].GetIsHidden() == 1)
    {
        if (tiles[z].GetNeighborMines() > 0)
        {
            tiles[z].SetIsHiddenFalse();
            if ((0 <= (firstPosition - columns) && (firstPosition - columns) < (columns * rows)) && tiles[firstPosition - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + columns) && (firstPosition + columns) < (columns * rows)) && tiles[firstPosition + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + 1) && (firstPosition + 1) < (columns * rows)) && tiles[firstPosition + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition - 1) && (firstPosition - 1) < (columns * rows)) && tiles[firstPosition - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, firstPosition, count);
            }
        }
        else
        {
            tiles[z].SetIsHiddenFalse();
            checkDiag(tiles, columns, rows, gameOver, z, count);
            if ((0 <= (z - columns) && (z - columns) < (columns * rows)) && tiles[z - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + columns) && (z + columns) < (columns * rows)) && tiles[z + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + 1) && (z + 1) < (columns * rows)) && tiles[z + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z - 1) && (z - 1) < (columns * rows)) && tiles[z - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, z, count);
            }
        }
    }
    //BOTTOM LEFT
    z = (firstPosition + columns) - 1;
    if (z >= 0 && z < (rows * columns) && !((z + 1) % columns == 0) && tiles[z].GetIsMine() == false && tiles[z].GetIsHidden() == 1)
    {
        if (tiles[z].GetNeighborMines() > 0)
        {
            tiles[z].SetIsHiddenFalse();
            if ((0 <= (firstPosition - columns) && (firstPosition - columns) < (columns * rows)) && tiles[firstPosition - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + columns) && (firstPosition + columns) < (columns * rows)) && tiles[firstPosition + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + 1) && (firstPosition + 1) < (columns * rows)) && tiles[firstPosition + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition - 1) && (firstPosition - 1) < (columns * rows)) && tiles[firstPosition - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, firstPosition, count);
            }
        }
        else
        {
            tiles[z].SetIsHiddenFalse();
            checkDiag(tiles, columns, rows, gameOver, z, count);
            if ((0 <= (z - columns) && (z - columns) < (columns * rows)) && tiles[z - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + columns) && (z + columns) < (columns * rows)) && tiles[z + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + 1) && (z + 1) < (columns * rows)) && tiles[z + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z - 1) && (z - 1) < (columns * rows)) && tiles[z - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, z, count);
            }
        }
    }
    //BOTTOM RIGHT
    z = (firstPosition + columns) + 1;
    //cout << z << " ";
    if (z >= 0 && z < (rows * columns) && (!(z % columns == 0)) && tiles[z].GetIsMine() == false && tiles[z].GetIsHidden() == 1)
    {
        if (tiles[z].GetNeighborMines() > 0)
        {
            tiles[z].SetIsHiddenFalse();
            if ((0 <= (firstPosition - columns) && (firstPosition - columns) < (columns * rows)) && tiles[firstPosition - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + columns) && (firstPosition + columns) < (columns * rows)) && tiles[firstPosition + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition + 1) && (firstPosition + 1) < (columns * rows)) && tiles[firstPosition + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, firstPosition, count);
            }
            if ((0 <= (firstPosition - 1) && (firstPosition - 1) < (columns * rows)) && tiles[firstPosition - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, firstPosition, count);
            }
        }
        else
        {
            tiles[z].SetIsHiddenFalse();
            checkDiag(tiles, columns, rows, gameOver, z, count);
            if ((0 <= (z - columns) && (z - columns) < (columns * rows)) && tiles[z - columns].GetIsHidden() == 1)
            {
                checkNorth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + columns) && (z + columns) < (columns * rows)) && tiles[z + columns].GetIsHidden() == 1)
            {
                checkSouth(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z + 1) && (z + 1) < (columns * rows)) && tiles[z + 1].GetIsHidden() == 1)
            {
                checkWest(tiles, columns, rows, gameOver, z, count);
            }
            if ((0 <= (z - 1) && (z - 1) < (columns * rows)) && tiles[z - 1].GetIsHidden() == 1)
            {
                checkEast(tiles, columns, rows, gameOver, z, count);
            }
        }
    }
}

void recursiveCheck(vector<Tile>& tiles, int columns, int rows, int gameOver, int firstPosition, int count)
{
    //The game hasn't ended && the tile isn't a mine
    if (gameOver == 0 && !(tiles[firstPosition].GetIsMine()))
    {
        checkNorth(tiles, columns, rows, gameOver, firstPosition, count);
        checkSouth(tiles, columns, rows, gameOver, firstPosition, count);
        checkWest(tiles, columns, rows, gameOver, firstPosition, count);
        checkEast(tiles, columns, rows, gameOver, firstPosition, count);
        checkDiag(tiles, columns, rows, gameOver, firstPosition, count);
    }
}


int main() {

    char* cwd = getcwd(NULL, 0);
    std::cout << "Current working directory: " << cwd << std::endl;

    string playerName; // player's name which provided during the WelcomeScreen
    int columns = readcfg().at(0);
    int rows = readcfg().at(1);
    int numMines = readcfg().at(2);
    int width = columns * 32;
    int height = (rows * 32) + 100;


    cout << "Columns: " << columns << " Rows: " << rows << " numMines: " << numMines << endl; // debuggin purposes
    cout << "Width: " << width << " Height: " << height << endl; // debuggin purposes

    WelcomeScreen wscreen(width, height); // generates the WelcomeScreen object
    wscreen.run(); // runs the WelcomeScreen
    playerName = wscreen.playerName;
    cout << "Player Name: " << playerName << endl;

    // loads all Textures required for the game
    Sprite debug(TextureMaker::GetTexture("debug"));
    Sprite happyface(TextureMaker::GetTexture("face_happy"));
    Sprite winface(TextureMaker::GetTexture("face_win"));
    Sprite loseface(TextureMaker::GetTexture("face_lose"));
    Sprite flag(TextureMaker::GetTexture("flag"));
    Sprite leaderboard(TextureMaker::GetTexture("leaderboard"));
    Sprite mine(TextureMaker::GetTexture("mine"));
    Sprite numOne(TextureMaker::GetTexture("number_1"));
    Sprite numTwo(TextureMaker::GetTexture("number_2"));
    Sprite numThree(TextureMaker::GetTexture("number_3"));
    Sprite numFour(TextureMaker::GetTexture("number_4"));
    Sprite numFive(TextureMaker::GetTexture("number_5"));
    Sprite numSix(TextureMaker::GetTexture("number_6"));
    Sprite numSeven(TextureMaker::GetTexture("number_7"));
    Sprite numEight(TextureMaker::GetTexture("number_8"));
    Sprite pause(TextureMaker::GetTexture("pause"));
    Sprite play(TextureMaker::GetTexture("play"));
    Sprite tileHidden(TextureMaker::GetTexture("tile_hidden"));
    Sprite tileRevealed(TextureMaker::GetTexture("tile_revealed"));


    Texture& digitTexture = TextureMaker::GetTexture("digits");
    vector<IntRect> digitRects;
    for (int i = 0; i <= 9; i++) {
        digitRects.emplace_back(i * 21, 0, 21, 32);
    }

    // intialize timer sprites
    vector<Sprite> timerSprites;
    initTimerSprites(timerSprites, digitTexture, digitRects, columns, rows);


    Board board;
    Tile tile;

    // TODO: Implement flagging
    int flagCount = 0;

    int tilesWithMines = 0; //Number of tiles with mines

    board.SetColumns(columns);
    board.SetRows(rows);
    board.SetMineCount(numMines);

    vector<Tile> tilesVec(rows * columns); //Sets up a vector of tiles to be used in the game

    // Initializes tiles in the tilesVec
    for (unsigned int i = 0; i < rows * columns; i++)
    {
        tilesVec.at(i) = tile;
    }

    // Loop to intialize the mines in the game

    // Sets random tiles w/i tilesVec to be mines
    for(unsigned int i = 0; i < numMines - tilesWithMines; i++)
    {
        tilesVec.at(intRNG(0, rows * columns - 1)).SetIsMineTrue();
        tilesWithMines++;
    }


    for (unsigned int i = 0; i < tilesVec.size(); i++)
    {
        if (tilesVec[i].GetIsMine() == 0) //If the tile is not a mine
        {
            int Up = -10, UpRight = -10, Right = -10, DownRight = -10, Down = -10, DownLeft = -10, Left = -10, UpLeft = -10;

            if ((i % (columns) == 0)) //If the tile is on the left edge
            {
                Up = i - columns;
                Right = i + 1;
                Down = i + columns;
                UpRight = Up + 1;
                DownRight = Down + 1;

            }
            else if ((i + 1) % columns == (0))
            {
                Up = i - columns;
                Down = i + columns;
                Left = i - 1;
                DownLeft = Down - 1;
                UpLeft = Up - 1;
            }
            else
            {
                Up = i - columns;
                Right = i + 1;
                Down = i + columns;
                Left = i - 1;

                UpRight = Up + 1;
                DownRight = Down + 1;
                DownLeft = Down - 1;
                UpLeft = Up - 1;
            }


            if (Up > 0 && Up < rows * columns)
            {
                if (tilesVec[Up].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }

            }
            if (UpRight > 0 && UpRight < rows * columns)
            {
                if (tilesVec[UpRight].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
            if (DownRight > 0 && DownRight < rows * columns)
            {
                if (tilesVec[DownRight].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
            if (Down > 0 && Down < rows * columns)
            {
                if (tilesVec[Down].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
            if (Right > 0 && Right < rows * columns && (Right) % columns != 0)
            {
                if (tilesVec[Right].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
            if (Left > 0 && Left < rows * columns && (Left + 1) % columns != 0)
            {
                if (tilesVec[Left].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
            if (UpLeft > 0 && UpLeft < rows * columns)
            {
                if (tilesVec[UpLeft].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
            if (DownLeft > 0 && DownLeft < rows * columns)
            {
                if (tilesVec[DownLeft].GetIsMine() == 1)
                {
                    tilesVec[i].AddNeighborMines();
                }
            }
        }
    }


    int gameOver = 0;
    int restart = 0;
    bool showMines = false;
    bool click = false;
    bool win = false;

    RenderWindow window(sf::VideoMode(width, height), "Minesweeper", Style::Close);
    window.setFramerateLimit(60);

    Clock gameClock;
    Time prevTime = Time::Zero;
    int elapsedSeconds = 0;


    // While the game window is open
    while(window.isOpen())
    {

        Event event;

        // While there are events to be processed
        while(window.pollEvent(event)) {

            // Clear the window with white
            window.clear(Color::White);

            // Close window clause
            if (event.type == Event::Closed) {
                window.close();
            }

            //If the game is still active
            if (gameOver == 0) {
                //Draws the happy face
                happyface.setPosition(((columns / 2) * 32) - 32, 32 * (rows + 0.5f));
                window.draw(happyface);

                // TODO: Draw timer here


                //If the mouse button is clicked
                if (event.type == Event::MouseButtonPressed) {
                    //If the left mouse button is clicked
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        //If the mouse is clicked on somewhere within the tiles
                        if (event.mouseButton.y >= 0 && event.mouseButton.y <= height - 100)
                        {
                            //If the mouse is clicked within the tiles for X
                            if (event.mouseButton.x >= 0 && event.mouseButton.x <= width) {
                                //Creates a temp X and Y for the mouse position and divides by 32
                                int tempX = event.mouseButton.x / 32;
                                int tempY = event.mouseButton.y / 32;

                                //Creates a index for a tile's position
                                int position = (tempY * (columns - 1)) + tempX + tempY;

                                //If the tile is not flagged
                                if (!tilesVec[position].GetIsFlagged()) { // or == false
                                    tilesVec[position].SetIsHiddenFalse();
                                    if (tilesVec[position].GetNeighborMines() == 0) {
                                        recursiveCheck(tilesVec, columns, rows, gameOver, position, 1);
                                    }
                                }
                                //debug
                                cout << "LMB was pressed at: " << event.mouseButton.x << ", " << event.mouseButton.y << endl;
                            }
                        }

                        //If the mouse is clicked on somewhere within the lower buttons
                        if (event.mouseButton.y >= (height - 100) && event.mouseButton.y <= (height - 32)) {

                            //If you click on the pause button
                            if (event.mouseButton.x >= 560 && event.mouseButton.x <= 624) {
                                if(event.mouseButton.y >= 528 && event.mouseButton.y <= 592) {
                                    cout << "Pause button was pressed" << endl;
                                }
                            }

                            //If you click on the leaderboard sprite
                            if (event.mouseButton.x >= 624 && event.mouseButton.x <= 688) {
                                if(event.mouseButton.y >= 528 && event.mouseButton.y <= 592) {
                                    cout << "Leaderboard button was pressed" << endl;
                                    LeaderboardScreen lscreen(16 * columns, (rows * 16) + 50, readLeaderboardText());
                                    lscreen.run();
                                }
                            }

                            //DEBUG WORKS
                            if (event.mouseButton.x >= 496 && event.mouseButton.x <= 560) {
                                if (showMines == true) {
                                    showMines = false;
                                    click = true;
                                } else {
                                    showMines = true;
                                    click = false;
                                }
                            }

                        }
                    }

                    // FIXME: Get this working
                    ////RMB is clicked
                    //If the RMB is clicked/Flag placement
                    if (event.mouseButton.button == Mouse::Right) {
                        if (event.mouseButton.y >= 0 && event.mouseButton.y <= height - 100) {
                            if (event.mouseButton.x >= 0 && event.mouseButton.x <= width) {
                                int tempX = event.mouseButton.x / 32;
                                int tempY = event.mouseButton.y / 32;
                                int position = (tempY * (columns - 1)) + tempX + tempY;
                                //Swap the flag status
                                tilesVec.at(position).SwapIsFlagged();
                                cout << "RMB was pressed at: " << event.mouseButton.x << ", " << event.mouseButton.y
                                     << endl;
                            }
                        }
                    }
                }
            }

            //If the player wins

            //If the game is over/Mine is clicked
            else {
                int counter = 0;
                for (unsigned int i = 0; i < rows; i++)
                {
                    for (unsigned int j = 0; j < columns; j++)
                    {
                        if (tilesVec[counter].GetIsMine()) {
                            tilesVec.at(counter).SetIsHiddenFalse();
                        }
                        counter++;
                    }
                }
                //Draws the lose face after losing
                loseface.setPosition(((columns / 2) * 32) - 32, 32 * (rows + 0.5f));
                window.draw(loseface);

                //If the mouse button is clicked
                if (event.type == Event::MouseButtonPressed) {
                    //If the left mouse button is clicked
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.y >= height - 100 && event.mouseButton.y <= height) {

                            if (event.mouseButton.x >= ((width / 2) - 32) &&
                                event.mouseButton.x <= ((width / 2) + 32)) {
                                gameOver = 0;
                                for (unsigned int i = 0; i < tilesVec.size(); i++) {
                                    tilesVec.at(i).SetIsMineFalse();
                                    tilesVec.at(i).SetIsHiddenTrue();
                                    tilesVec.at(i).ResetNeighborMines();
                                    if (tilesVec.at(i).GetIsFlagged()) {
                                        tilesVec.at(i).SwapIsFlagged();
                                    }
                                }

                                restart = 1;
                                cout << "LMB was pressed at: " << event.mouseButton.x << ", " << event.mouseButton.y
                                     << endl;
                            }
                        }
                    }
                }
            }

            int counter = 0;

            //Nested loops to check each tile *DONE*

            for (unsigned int i = 0; i < rows; i++) {
                for (unsigned int j = 0; j < columns; j++) {
                    const unsigned int idx = i * columns + j;
                     Tile& tile = tilesVec[idx];

                    const unsigned int x = j * 32;
                    const unsigned int y = i * 32;

                    if (tile.GetIsHidden()) {
                        tileHidden.setPosition(x, y);
                        window.draw(tileHidden);

                        if (tile.GetIsFlagged()) {
                            flag.setPosition(x, y);
                            window.draw(flag);
                        }
                    }
                    else {
                        tileRevealed.setPosition(x, y);
                        window.draw(tileRevealed);

                        if (tile.GetIsMine()) {
                            if (tile.GetIsFlagged()) {
                                flag.setPosition(x, y);
                                window.draw(flag);
                            }

                            mine.setPosition(x, y);
                            window.draw(mine);
                            gameOver = true;
                        }
                        else {
                            const unsigned int neighborMines = tile.GetNeighborMines();
                            if (neighborMines > 0 && neighborMines <= 8) {
                                switch (neighborMines) {
                                    case 1: numOne.setPosition(x, y); window.draw(numOne); break;
                                    case 2: numTwo.setPosition(x, y); window.draw(numTwo); break;
                                    case 3: numThree.setPosition(x, y); window.draw(numThree); break;
                                    case 4: numFour.setPosition(x, y); window.draw(numFour); break;
                                    case 5: numFive.setPosition(x, y); window.draw(numFive); break;
                                    case 6: numSix.setPosition(x, y); window.draw(numSix); break;
                                    case 7: numSeven.setPosition(x, y); window.draw(numSeven); break;
                                    case 8: numEight.setPosition(x, y); window.draw(numEight); break;
                                    default: break;
                                }
                            }
                        }
                    }
                }
            }


            //Restart clause
            if (restart == 1) {
                // Randomize mine positions
                int tilesWithMines = 0;
                while (tilesWithMines != numMines) {
                    int temp = numMines - tilesWithMines;
                    tilesWithMines = 0;
                    for (unsigned int i = 0; i < temp; i++) {
                        int randIndex = intRNG(0, ((rows * columns) - 1));
                        if (!tilesVec[randIndex].GetIsMine()) {
                            tilesVec[randIndex].SetIsMineTrue();
                        }
                    }

                    for (unsigned int i = 0; i < tilesVec.size(); i++) {
                        if (tilesVec.at(i).GetIsMine() == 1) {
                            tilesWithMines++;
                        }
                    }
                }

                // Set the number of mines around each tile
                for (unsigned int i = 0; i < tilesVec.size(); i++) {
                    if (!tilesVec.at(i).GetIsMine()) {
                        int N = i - columns;
                        int NE = N + 1;
                        int E = i + 1;
                        int SE = E + columns;
                        int S = i + columns;
                        int SW = S - 1;
                        int W = i - 1;
                        int NW = N - 1;

                        if (i % columns == 0) {
                            N = -1;
                            NE = -1;
                            W = -1;
                            NW = -1;
                        } else if ((i + 1) % columns == 0) {
                            E = -1;
                            SE = -1;
                            S = -1;
                            SW = -1;
                        }

                        if (N >= 0 && tilesVec[N].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (NE >= 0 && (NE % columns != 0) && tilesVec[NE].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (E % columns != 0 && tilesVec[E].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (SE < (rows * columns) && (SE % columns != 0) && tilesVec[SE].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (S < (rows * columns) && tilesVec[S].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (W >= 0 && (W + 1) % columns != 0 && tilesVec[W].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (NW >= 0 && (NW + 1) % columns != 0 && tilesVec[NW].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                        if (SW < (rows * columns) && SW % columns != 0 && tilesVec[SW].GetIsMine()) {
                            tilesVec[i].AddNeighborMines();
                        }
                    }
                }
                restart = 0;
            }

            //If click is true, show all mines
            if (click == true) {
                int count = 0;
                for (unsigned int i = 0; i < rows; i++)
                {
                    for (unsigned int j = 0; j < columns; j++)
                    {
                        if (tilesVec.at(count).GetIsMine() == true)
                        {
                            mine.setPosition(j * 32, i * 32);
                            window.draw(mine);
                        }
                        count++;
                    }
                }
            }


            //// TODO: buttons work but need to fix leaderboard screen and add pause screen

            debug.setPosition((columns * 32) -  304, 32 * (rows + 0.5f)); //
            window.draw(debug);

            leaderboard.setPosition((columns * 32) -176, 32 * (rows + 0.5f)); //
            window.draw(leaderboard);

            pause.setPosition((columns * 32) - 240, 32 * (rows + 0.5f)); //
            window.draw(pause);

        }

        // get the time difference
        Time currentTime = gameClock.getElapsedTime();
        Time deltaTime = currentTime - prevTime;

        // updates timer every second
        if (deltaTime.asSeconds() >= 1.0f) {
            prevTime = currentTime;  // Reset the reference time
            elapsedSeconds++;

            updateTimerSprites(timerSprites, digitRects, elapsedSeconds);

            // timer debugger
            cout << "Timer: " << elapsedSeconds / 60 << ":"
                 << setw(2) << setfill('0') << (elapsedSeconds % 60) << endl;
        }

        for (auto& sprite : timerSprites) {
            window.draw(sprite);
        }

        window.display();
    }
    return 0;
}

