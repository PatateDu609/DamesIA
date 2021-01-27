#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Params.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(Params::size, Params::size), "Dames");

	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);

	Board *board = Board::getInstance(Params::size, Params::ratio[R_CELL]);

//	board->setPosition(static_cast<float>(Params::size), 0);

	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				board->click(sf::Mouse::getPosition(window));
		}

		window.clear(sf::Color(60, 60, 60));
		window.draw(*board);
		window.display();
	}

	Board::resetInstance();

	return 0;
}