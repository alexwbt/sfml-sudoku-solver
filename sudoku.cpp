#include "sudoku.h"

#include <iostream>

Sudoku::Sudoku(float x, float y, float size)
    : x_(x), y_(y), size_(size)
{
    for (int x = 0; x < kSize; x++)
        for (int y = 0; y < kSize; y++)
            grid_[x][y] = { 0, false, {0, 0, 0, 0, 0, 0, 0, 0, 0} };

    if (!font_.loadFromFile("font.ttf"))
    {
        std::cout << "Failed to load font" << std::endl;
    }
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
    float cell_size = size_ / (float)kSize;

    sf::RectangleShape line_v(sf::Vector2f(line_width_, size_));
    line_v.setFillColor(line_color_);
    for (int x = 0; x < 9; x++)
    {
        line_v.setPosition(sf::Vector2f(x_ + cell_size * x, y_));
        target.draw(line_v);
    }
    sf::RectangleShape line_h(sf::Vector2f(size_, line_width_));
    line_h.setFillColor(line_color_);
    for (int y = 0; y < 9; y++)
    {
        line_h.setPosition(sf::Vector2f(x_, y_ + cell_size * y));
        target.draw(line_h);
    }

    sf::RectangleShape border_v(sf::Vector2f(border_width_, size_));
    border_v.setFillColor(border_color_);
    for (int x = 0; x < 4; x++)
    {
        border_v.setPosition(sf::Vector2f(x_ + cell_size * x * 3, y_));
        target.draw(border_v);
    }
    sf::RectangleShape border_h(sf::Vector2f(size_, border_width_));
    border_h.setFillColor(border_color_);
    for (int y = 0; y < 4; y++)
    {
        border_h.setPosition(sf::Vector2f(x_, y_ + cell_size * y * 3));
        target.draw(border_h);
    }

    sf::Text number;
    number.setFont(font_);
    number.setFillColor(font_color_);
    number.setCharacterSize(cell_size * 0.8f);

    sf::Text note;
    note.setFont(font_);
    note.setFillColor(note_color_);
    note.setCharacterSize(cell_size * 0.28f);
    
    auto map = [&](int x, int y, Cell& cell)
    {
        if (cell.value > 0)
        {
            number.setString(std::to_string(cell.value));
            number.setPosition(x_ + (x + 0.1f) * cell_size, y_ + (y + 0.05f) * cell_size);
            target.draw(number);
        }
        else
        {
            for (int i = 0; i < kNumCount; i++)
            {
                if (cell.note[i])
                {
                    note.setString(std::to_string(i + 1));
                    note.setPosition(x_ + (x + 0.05f) * cell_size + (i / 3) * cell_size / 3, y_ + (y + 0.015f) * cell_size + (i % 3) * cell_size / 3);
                    target.draw(note);
                }
            }
        }
        return false;
    };
    Map(map);
}
