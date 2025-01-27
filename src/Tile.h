#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Tile{
public:
    bool GetIsHidden();
    void SetIsHiddenFalse();
    void SetIsHiddenTrue();
    bool GetIsMine();
    void SetIsMineTrue();
    void SetIsMineFalse();
    bool GetIsFlagged();
    void SwapIsFlagged();
    int GetNeighborMines();
    void AddNeighborMines();
    void ResetNeighborMines();

private:
    bool _isHidden = true; //intialized to true as all tiles are hidden at start
    bool _isMine = false;
    bool _isFlagged = false;
    int _neighborMines = 0; //Num of mines nearby
};
