//
// Created by Teyber on 1/25/2021.
//

#include <iostream>
#include "Piece.hpp"
#include "Params.hpp"

#include <fstream>

sf::Texture *Piece::_texture = nullptr;

Piece::Piece(Piece::Color color, Board *board, std::array<int, 2> coord) : _color(color), _board(board), _coord(coord),
                                                                           _isKing(false)
{
	_drawingColor = _color == BLACK ? Params::color[C_MAN_BLACK] : Params::color[C_MAN_WHITE];

	if (_texture)
		return;

	_texture = new sf::Texture();
	if (!_texture->loadFromFile("crown.png"))
	{
		std::cout << "ERROR : crown.png could not be loaded!" << std::endl;
		return;
	}

	_texture->setSmooth(true);
}

Piece::Color Piece::getColor() const
{
	return _color;
}

sf::Color Piece::getDrawingColor() const
{
	return _drawingColor;
}

std::array<int, 2> Piece::getCoord() const
{
	return _coord;
}

bool Piece::valid(const std::array<int, 2> &coord)
{
	return true;
}

bool Piece::move(const std::array<int, 2> &coord)
{
	if (!valid(coord))
		return false;
	_coord = coord;
	return true;
}

void Piece::upgrade()
{
	_isKing = true;
}

std::vector<std::array<int, 2>> Piece::getReachable() const
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

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	float radius = (_board->getCellSize() / 2) * Params::ratio[R_MAN];
	float offset = (_board->getCellSize() - 2 * radius) / 2.f;
	sf::CircleShape piece(radius);
	sf::Vector2f pos = _board->getCellSize() *
	                   sf::Vector2f(static_cast<float>(_coord[0]), static_cast<float>(_coord[1]));

	piece.setFillColor(_drawingColor);
	piece.setPosition(pos + sf::Vector2f(offset, offset));

	target.draw(piece, states);

	if (!_isKing)
		return;
	sf::Sprite sp;
	sp.setTexture(*_texture);

	sf::Vector2f texSize(_texture->getSize().x, _texture->getSize().y);
	sp.setOrigin(texSize * .5f);

	sp.setScale(sf::Vector2f(radius / texSize.x, radius / texSize.y));
	sp.setPosition(pos + sf::Vector2f(_board->getCellSize(), _board->getCellSize()) * .5f);
	sp.setColor(sf::Color::White);
	target.draw(sp, states);
}
