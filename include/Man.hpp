//
// Created by Teyber on 1/25/2021.
//

#ifndef DAMES_MAN_HPP
#define DAMES_MAN_HPP

#include <array>
#include <SFML/Graphics.hpp>

class Board;

class Man : public sf::Drawable, public sf::Transformable
{
public:
	enum Color
	{
		BLACK,
		WHITE
	};

	Man(Color color, Board *board, std::array<int, 2> coord);

	[[nodiscard]] Color getColor() const;
	[[nodiscard]] sf::Color getDrawingColor() const;
	[[nodiscard]] std::array<int, 2> getCoord() const;

	virtual void draw(sf::RenderWindow& window, float radius);
	[[nodiscard]] virtual std::vector<std::array<int, 2>> getReachable() const;

	bool move(const std::array<int, 2>& coord);

private:
	Color _color;
	Board *_board;
	std::array<int, 2> _coord;
	sf::Color _drawingColor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool valid(const std::array<int, 2> &coord);
};

#include "Board.hpp"

#endif //DAMES_MAN_HPP
