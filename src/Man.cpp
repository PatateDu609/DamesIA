//
// Created by Teyber on 1/25/2021.
//

#include "Man.hpp"
#include "Params.hpp"

Man::Man(Man::Color color, Board *board, std::array<int, 2> coord) : _color(color), _board(board), _coord(coord)
{
	_drawingColor = _color == BLACK ? Params::color[C_MAN_BLACK] : Params::color[C_MAN_WHITE];
}

Man::Color Man::getColor() const
{
	return _color;
}

sf::Color Man::getDrawingColor() const
{
	return _drawingColor;
}

std::array<int, 2> Man::getCoord() const
{
	return _coord;
}

void Man::draw(sf::RenderWindow &window, float radius)
{
	float offset = (_board->getCellSize() - 2 * radius) / 2.f;
	sf::CircleShape piece(radius);
	sf::Vector2f pos = _board->getCellSize() *
	                   sf::Vector2f(static_cast<float>(_coord[0]), static_cast<float>(_coord[1]));

	piece.setFillColor(_drawingColor);
	piece.setPosition(pos + sf::Vector2f(offset, offset));

	window.draw(piece);
}

bool Man::valid(const std::array<int, 2> &coord)
{
	return true;
}

bool Man::move(const std::array<int, 2> &coord)
{
	if (!valid(coord))
		return false;
	_coord = coord;
	return true;
}

std::vector<std::array<int, 2>> Man::getReachable() const
{
	std::vector<std::array<int, 2>> reachable;

	if (!((_color == WHITE && _coord[1] < 9) ||
	      (_color == BLACK && _coord[1])))
		return {};

	int newY = _coord[1] + (_color == WHITE ? -1 : 1);

	if (_coord[0])
		reachable.push_back(std::array<int, 2>{_coord[0] - 1, newY});
	if (_coord[0] < 9)
		reachable.push_back(std::array<int, 2>{_coord[0] + 1, newY});
	return reachable;
}

void Man::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	float radius = (_board->getCellSize() / 2) * Params::ratio[R_MAN];
	float offset = (_board->getCellSize() - 2 * radius) / 2.f;
	sf::CircleShape piece(radius);
	sf::Vector2f pos = _board->getCellSize() *
	                   sf::Vector2f(static_cast<float>(_coord[0]), static_cast<float>(_coord[1]));

	piece.setFillColor(_drawingColor);
	piece.setPosition(pos + sf::Vector2f(offset, offset));

	target.draw(piece, states);
}
