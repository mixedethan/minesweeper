#include "Tile.h"

bool Tile::GetIsHidden()
{
    return _isHidden;
}
void Tile::SetIsHiddenFalse()
{
    _isHidden = false;
}
void Tile::SetIsHiddenTrue()
{
    _isHidden = true;
}
bool Tile::GetIsMine()
{
    return _isMine;
}
void Tile::SetIsMineTrue()
{
    _isMine = true;
}
void Tile::SetIsMineFalse()
{
    _isMine = false;
}
bool Tile::GetIsFlagged()
{
    return _isFlagged;
}
void Tile::SwapIsFlagged()
{
    if (_isFlagged == false)
    {
        _isFlagged = true;
    }
    else if (_isFlagged == true)
    {
        _isFlagged = false;
    }

}
int Tile::GetNeighborMines()
{
    return _neighborMines;
}
void Tile::AddNeighborMines()
{
    _neighborMines++;
}
void Tile::ResetNeighborMines()
{
    _neighborMines = 0;
}
