#pragma once

#include <stdint.h>
#include <functional>

#include <SFML/Graphics.hpp>

constexpr int kSize = 9;
constexpr int kNumCount = 9;
constexpr int kBlockSize = 3;

struct Cell
{
    uint8_t value = 0;
    bool is_static = false;
    bool note[kNumCount];
};

class Sudoku
{
private:
    Cell grid_[kSize][kSize];

public:
    Sudoku();

    Cell& Get(int x, int y);

    // return true to break
    void Map(std::function<bool(int, int, Cell&)>);
    void MapBlock(int cx, int cy, std::function<bool(int, int, Cell&)>);
    void MapLineX(int y, std::function<bool(int, int, Cell&)>);
    void MapLineY(int x, std::function<bool(int, int, Cell&)>);

    void Render(sf::RenderTarget& target);
};

void Solve(Sudoku& sudoku)
{
    while (true)
    {
        auto map = [&](int x, int y, Cell& cell)
        {
            // note posible numbers
            int noted_count = 0;
            for (int i = 0; i < kSize; i++)
            {
                bool note = true;
                uint8_t test_number = i + 1;

                // test if number exists
                auto test = [&](int, int, Cell& test_c)
                {
                    if (test_c.value == test_number)
                        note = false;
                    return !note;
                };
                sudoku.MapBlock(x, y, test);
                sudoku.MapLineX(y, test);
                sudoku.MapLineY(x, test);

                if (note)
                {
                    cell.note[i] = true;
                    noted_count++;
                }
            }

            return false;
        };
        sudoku.Map(map);
    }
}
