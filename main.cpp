#include <SFML/Graphics.hpp>

#include "sudoku.h"

#include <iostream>

void Solve(Sudoku& sudoku)
{
    auto note_possible_numbers = [&](int x, int y, Cell& cell)
    {
        if (cell.value != 0) return false;
        int noted_count = 0;
        for (int i = 0; i < kNumCount; i++)
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
    sudoku.Map(note_possible_numbers);

    while (true)
    {
        bool set_value = false;
        auto find_only_possibility = [&](int x, int y, Cell& cell)
        {
            if (cell.value != 0) return false;
            for (int i = 0; i < kNumCount; i++)
            {
                bool only = true;
                uint8_t test_number = i + 1;
                if (!cell.note[i]) continue;

                // test only one possibility
                auto test = [&](int tx, int ty, Cell& test_c)
                {
                    if ((tx != x || ty != y) && test_c.note[i])
                        only = false;
                    return !only;
                };
                sudoku.MapBlock(x, y, test);
                if (!only) sudoku.MapLineX(y, test);
                if (!only) sudoku.MapLineY(x, test);

                if (only)
                {
                    // answer
                    cell.value = test_number;
                    set_value = true;

                    // remove note
                    auto remove_note = [&](int, int, Cell& remove_c)
                    {
                        remove_c.note[i] = false;
                        return false;
                    };
                    sudoku.MapBlock(x, y, remove_note);
                    sudoku.MapLineX(y, remove_note);
                    sudoku.MapLineY(x, remove_note);
                }
            }
            return false;
        };
        sudoku.Map(find_only_possibility);

        if (!set_value) break;
    }
}

void HandleEvents(sf::Window& window, std::function<void(sf::Event&)> key_pressed)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            key_pressed(event);
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

    auto key_pressed = [&sudoku](sf::Event& event)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::W: sudoku.MoveSelect(0, -1); break;
        case sf::Keyboard::A: sudoku.MoveSelect(-1, 0); break;
        case sf::Keyboard::S: sudoku.MoveSelect(0, 1); break;
        case sf::Keyboard::D: sudoku.MoveSelect(1, 0); break;
        case sf::Keyboard::Num1: sudoku.SetSelect(1); break;
        case sf::Keyboard::Num2: sudoku.SetSelect(2); break;
        case sf::Keyboard::Num3: sudoku.SetSelect(3); break;
        case sf::Keyboard::Num4: sudoku.SetSelect(4); break;
        case sf::Keyboard::Num5: sudoku.SetSelect(5); break;
        case sf::Keyboard::Num6: sudoku.SetSelect(6); break;
        case sf::Keyboard::Num7: sudoku.SetSelect(7); break;
        case sf::Keyboard::Num8: sudoku.SetSelect(8); break;
        case sf::Keyboard::Num9: sudoku.SetSelect(9); break;
        case sf::Keyboard::Numpad1: sudoku.SetSelect(1); break;
        case sf::Keyboard::Numpad2: sudoku.SetSelect(2); break;
        case sf::Keyboard::Numpad3: sudoku.SetSelect(3); break;
        case sf::Keyboard::Numpad4: sudoku.SetSelect(4); break;
        case sf::Keyboard::Numpad5: sudoku.SetSelect(5); break;
        case sf::Keyboard::Numpad6: sudoku.SetSelect(6); break;
        case sf::Keyboard::Numpad7: sudoku.SetSelect(7); break;
        case sf::Keyboard::Numpad8: sudoku.SetSelect(8); break;
        case sf::Keyboard::Numpad9: sudoku.SetSelect(9); break;
        case sf::Keyboard::F: Solve(sudoku); break;
        }
    };

    while (window.isOpen())
    {
        HandleEvents(window, key_pressed);

        if (sudoku.rerender())
        {
            window.clear(sf::Color(240, 240, 240, 255));
            sudoku.Render(window);
            window.display();
        }
    }
}
