#include <SFML/Graphics.hpp>

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
	bool a = (bool)3;
	uint8_t b = 13;

	sf::RenderWindow window(
		sf::VideoMode(600, 600),
		"Sudoku Solver",
		sf::Style::Default,
		sf::ContextSettings(0, 0, 10));

	while (window.isOpen())
	{
		HandleEvents(window);
	}
}
