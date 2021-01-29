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

    int select_x_ = -1, select_y_ = -1;

    // render
    bool rerender_ = true;
    float x_, y_, size_;
    float line_width_ = 1;
    float border_width_ = 2;
    sf::Color line_color_ = sf::Color(150, 150, 150);
    sf::Color border_color_ = sf::Color(50, 50, 50);
    sf::Font font_;
    sf::Color font_color_ = sf::Color::Black;
    sf::Color note_color_ = sf::Color(150, 150, 150);

public:
    Sudoku(float x, float y, float size);

    bool rerender() const { return rerender_; }

    Cell& Get(int x, int y);

    void SetSelect(uint8_t value);
    void MoveSelect(int x, int y);

    // return true to break
    void Map(std::function<bool(int, int, Cell&)>);
    void MapBlock(int cx, int cy, std::function<bool(int, int, Cell&)>);
    void MapLineX(int y, std::function<bool(int, int, Cell&)>);
    void MapLineY(int x, std::function<bool(int, int, Cell&)>);

    void Render(sf::RenderTarget& target);
};
