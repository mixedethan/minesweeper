#pragma once
#include "Tile.h"
#include <vector>

using namespace std;


class Board{
private:
    int _columns;
    int _rows;
    int _mineCount;
    vector<Tile> tiles;

public:
    void SetColumns(int columns);
    void SetRows(int rows);
    void SetMineCount(int mineCount);
};

