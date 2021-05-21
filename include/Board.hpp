//
// Created by Teyber on 1/25/2021.
//

#ifndef DAMES_BOARD_HPP
#define DAMES_BOARD_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include <map>
#include <limits>
#include <windows.h>

class Board : public sf::Drawable, public sf::Transformable
{
	struct Move{
		Coord start;
		Coord arrive;
		std::vector<Coord> targets;
	};

public:
	typedef std::vector<std::vector<Piece *>> State;

	~Board();

	Board(const Board &board) = delete;

	Board &operator=(const Board &board) = delete;

	static Board *getInstance(int size, float radius);
	static void resetInstance();
	[[nodiscard]] const Piece *getCell(int x, int y) const;
	[[nodiscard]] Piece *getCell(int x, int y);
	float getCellSize() const;

	void click(sf::Vector2i pos);
	void update();

	void setAI(Piece::Color color);

private:
	Board(int size, float radius);

	std::array<int, 2> getBoardCoord(const sf::Vector2i &pos);
	void drawReachable(sf::RenderTarget &target, sf::RenderStates states) const;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void remove_captured(const Coord &next);
	bool checkEnd() const;
	static int checkEnd(const std::vector<std::vector<Piece *>>& state);
	void checkPossibilities();
	void drawPossibilities(sf::RenderTarget &target, sf::RenderStates &states) const;
	static int moveEval(Piece *piece);

	void best_move();
	int minmax(const State &state, Move &move, int depth, int currentTurn, bool max, int alpha, int beta);
	int evaluation(const std::vector<std::vector<Piece *>> &state, int currentTurn, bool max);
	std::vector<std::pair<Board::Move, Board::State>> getStates(const State &state, Coord start) const;
	static void applyMovement(State &state, Move move) ;
	static State duplicate(const State& state);
	static void free(State& state);
	void applyCapture(const std::vector<Coord>& targets);

	State _cells{};
	static Board *instance;
	float _ratio;
	int _size;
	Piece *_selected;
	std::vector<Piece *> possibilities;

	int turn{0};
	std::map<Piece::Color, uint8_t> pieces;
	Piece::Capture capture;
	Piece::Color ai;
};

#endif //DAMES_BOARD_HPP
