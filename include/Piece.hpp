//
// Created by Teyber on 1/25/2021.
//

#ifndef DAMES_PIECE_HPP
#define DAMES_PIECE_HPP

#include <array>
#include <SFML/Graphics.hpp>
#include "Params.hpp"

class Board;

class Piece : public sf::Drawable, public sf::Transformable
{
public:
	enum Color
	{
		BLACK,
		WHITE
	};

	Piece(Color color, Board *board, std::array<int, 2> coord);

	[[nodiscard]] Color getColor() const;
	[[nodiscard]] sf::Color getDrawingColor() const;
	[[nodiscard]] std::array<int, 2> getCoord() const;

	[[nodiscard]] std::vector<std::array<int, 2>> getReachable() const;

	bool move(const std::array<int, 2>& coord);

	void upgrade();

private:
	Color _color;
	Board *_board;
	std::array<int, 2> _coord;
	sf::Color _drawingColor;
	bool _isKing;
	static sf::Texture *_texture;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool valid(const std::array<int, 2> &coord);
};

#include "Board.hpp"

#endif //DAMES_PIECE_HPP
