#include <SFML/Graphics.hpp>

#include "sudoku.h"

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
                if (note) sudoku.MapLineX(y, test);
                if (note) sudoku.MapLineY(x, test);

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

void HandleEvents(sf::Window& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default: break;
        }
    }
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(600, 600),
        "Sudoku Solver",
        sf::Style::Default,
        sf::ContextSettings(0, 0, 10));

    Sudoku sudoku(10, 10, 578);

    window.clear(sf::Color(240, 240, 240, 255));
    sudoku.Render(window);
    window.display();

    while (window.isOpen())
    {
        HandleEvents(window);
    }
}
