#include <iostream>

#include <SFML/Graphics.hpp>

#include "sudoku.h"
#include "solver.h"

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
        case sf::Keyboard::Up: sudoku.MoveSelect(0, -1); break;
        case sf::Keyboard::Left: sudoku.MoveSelect(-1, 0); break;
        case sf::Keyboard::Down: sudoku.MoveSelect(0, 1); break;
        case sf::Keyboard::Right: sudoku.MoveSelect(1, 0); break;
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
        case sf::Keyboard::Delete: sudoku.SetSelect(0); break;
        case sf::Keyboard::Space: sudoku.Clear(); break;
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
