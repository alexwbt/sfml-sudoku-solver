#include "sudoku.h"

Sudoku::Sudoku()
{
    for (int x = 0; x < kSize; x++)
        for (int y = 0; y < kSize; y++)
            grid_[x][y] = { 0, false, {0, 0, 0, 0, 0, 0, 0, 0, 0} };
}

Cell& Sudoku::Get(int x, int y)
{
    return grid_[x][y];
}

void Sudoku::Map(std::function<bool(int, int, Cell&)> callback)
{
    for (int x = 0; x < kSize; x++)
        for (int y = 0; y < kSize; y++)
            if (callback(x, y, grid_[x][y]))
                return;
}

void Sudoku::MapBlock(int cx, int cy, std::function<bool(int, int, Cell&)> callback)
{
    int sx = cx / kBlockSize;
    int sy = cy / kBlockSize;
    for (int x = sx; x < sx + kBlockSize; x++)
        for (int y = sy; y < sy + kBlockSize; y++)
            if (callback(x, y, grid_[x][y]))
                return;
}

void Sudoku::MapLineX(int y, std::function<bool(int, int, Cell&)> callback)
{
    for (int x = 0; x < kSize; x++)
        if (callback(x, y, grid_[x][y]))
            return;
}

void Sudoku::MapLineY(int x, std::function<bool(int, int, Cell&)> callback)
{
    for (int y = 0; y < kSize; y++)
        if (callback(x, y, grid_[x][y]))
            return;
}

void Sudoku::Render(sf::RenderTarget& target)
{

}
