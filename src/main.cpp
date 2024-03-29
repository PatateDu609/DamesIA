#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.hpp"
#include "Params.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(Params::size, Params::size), "Dames");

	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);

	Board *board = Board::getInstance(Params::size, Params::ratio[R_CELL]);

	if (Params::enableAI)
		board->setAI(Piece::BLACK);
	switch (Params::difficulty)
	{
		case EASY:
			Params::depth = 15;
			break;
		case MEDIUM:
			Params::depth = 25;
			break;
		case HARD:
			Params::depth = 50;
	}

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				board->click(sf::Mouse::getPosition(window));
		}

		window.clear(sf::Color(60, 60, 60));
		window.draw(*board);
		window.display();

		board->update();
	}

	Board::resetInstance();

	return 0;
}