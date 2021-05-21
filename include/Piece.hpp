//
// Created by Teyber on 1/25/2021.
//

#ifndef DAMES_PIECE_HPP
#define DAMES_PIECE_HPP

typedef std::array<int, 2> Coord;

#include <array>
#include <SFML/Graphics.hpp>
#include "Params.hpp"
#include <algorithm>

class Board;

class Piece : public sf::Drawable, public sf::Transformable
{
public:
	struct Capture
	{
		uint8_t nbPieces;
		uint8_t nbKings;
		std::vector<std::vector<Coord>> paths;
		std::vector<std::vector<Coord>> targets;

		bool operator>(const Capture &a) const;
		bool operator==(const Capture &a) const;
		bool operator!=(const Capture &a) const;
	};

	enum Color
	{
		BLACK,
		WHITE
	};

	Piece(Color color, Board *board, Coord coord);
	Piece(Color color, std::vector<std::vector<Piece *>>  board, Coord coord);

	[[nodiscard]] Color getColor() const;
	[[nodiscard]] sf::Color getDrawingColor() const;
	[[nodiscard]] Coord getCoord() const;
	bool isKing() const;

	[[nodiscard]] std::vector<std::pair<Coord, Capture>> getReachable();

	bool move(const Coord &coord);

	void setBoard(const std::vector<std::vector<Piece *>> &vector);
private:
	Color _color;
	Board *_board;
	std::vector<std::vector<Piece *>> board;
	Coord _coord;
	sf::Color _drawingColor;
	bool _isKing;
	static sf::Texture *_texture;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	bool valid(const std::array<int, 2> &coord);

	const Piece *getCell(Coord coord) const;
	Piece *getCell(Coord coord);
	const Piece *getCell(int x, int y) const;
	Piece *getCell(int x, int y);

	bool check_capture(const Coord &start, std::vector<Coord> dirs, Capture &quality);
	bool search_capture(Coord start, const Coord &dir, Coord &target) const;

	void king_moves(std::vector<std::pair<Coord, Piece::Capture>> &reachable, const std::vector<Coord> &dirs) const;
	std::vector<Coord> expand_move(Coord start, Coord dir) const;
	static std::vector<Coord> regenerate_dirs(const Coord& last);
};

#include "Board.hpp"

#endif //DAMES_PIECE_HPP
