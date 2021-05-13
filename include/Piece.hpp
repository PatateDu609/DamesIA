//
// Created by Teyber on 1/25/2021.
//

#ifndef DAMES_PIECE_HPP
#define DAMES_PIECE_HPP

typedef std::array<int, 2> coord;

#include <array>
#include <SFML/Graphics.hpp>
#include "Params.hpp"

class Board;

class Piece : public sf::Drawable, public sf::Transformable
{
	struct Capture
	{
		uint8_t nbPieces;
		uint8_t nbKings;
		std::vector<std::vector<coord>> paths;

		bool operator>(const Capture& a) const;
		bool operator==(const Capture& a) const;
		bool operator!=(const Capture& a) const;
	};

public:
	
	enum Color
	{
		BLACK,
		WHITE
	};

	Piece(Color color, Board *board, coord coord);

	[[nodiscard]] Color getColor() const;
	[[nodiscard]] sf::Color getDrawingColor() const;
	[[nodiscard]] coord getCoord() const;

	[[nodiscard]] std::vector<coord> getReachable() const;

	bool move(const coord& coord);

private:
	Color _color;
	Board *_board;
	coord _coord;
	sf::Color _drawingColor;
	bool _isKing;
	static sf::Texture *_texture;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool valid(const std::array<int, 2> &coord) const;

	bool check_capture(const coord& start, const std::vector<coord>& dirs, Capture& quality) const;
	bool search_capture(coord start, const coord& dir, coord& target) const;
};

#include "Board.hpp"

#endif //DAMES_PIECE_HPP
