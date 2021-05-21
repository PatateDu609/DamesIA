//
// Created by Teyber on 1/25/2021.
//

#include "Board.hpp"
#include <iostream>
#include "RingShape.hpp"
#include "Params.hpp"

Board *Board::instance = nullptr;

Board::Board(int size, float ratio) : _cells{}, _size(size), _ratio(ratio), _selected(nullptr)
{
	Piece::Color colors[] = {Piece::BLACK, Piece::WHITE};

	for (int i = 0; i < 10; i++)
	{
		_cells.emplace_back();
		for (int j = 0; j < 10; j++)
		{
			if ((j + (i % 2)) % 2 || (3 < i && i < 6))
				_cells[i].push_back(nullptr);
			else
			{
//				_cells[i].push_back(nullptr);
				_cells[i].push_back(new Piece(colors[i >= 6], this, Coord{j, i}));
				pieces[_cells[i].back()->getColor()]++;
			}
		}
	}
//	_cells[1][1] = new Piece(colors[1], this, Coord{1, 1});
//	_cells[4][4] = new Piece(colors[0], this, Coord{4, 4});
//	_cells[7][7] = new Piece(colors[0], this, Coord{7, 7});
//	_cells[1][1] = new Piece(colors[1], this, Coord{1, 1});

//	_cells[4][8] = new Piece(colors[0], this, Coord{8, 4});
//	_cells[3][5] = new Piece(colors[0], this, Coord{5, 3});
//	_cells[4][6] = new Piece(colors[0], this, Coord{6, 4});
//	_cells[7][3] = new Piece(colors[0], this, Coord{3, 7});
//	_cells[7][5] = new Piece(colors[0], this, Coord{5, 7});
}

Board::~Board()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			delete _cells[i][j];
}

Board *Board::getInstance(int size, float ratio)
{
	if (!instance)
		instance = new Board(size, ratio);
	return instance;
}

void Board::resetInstance()
{
	delete instance;
	instance = nullptr;
}

const Piece *Board::getCell(int x, int y) const
{
	return _cells[y][x];
}

Piece *Board::getCell(int x, int y)
{
	return _cells[y][x];
}

float Board::getCellSize() const
{
	return static_cast<float>(_size) * _ratio;
}

void Board::remove_captured(const Coord &next)
{
	if (!capture.nbPieces)
		return;

	for (decltype(capture.paths)::size_type i = 0; i < capture.paths.size(); i++)
	{
		if (next != capture.paths[i][0])
		{
			capture.paths.erase(capture.paths.begin() + (long long) i);
			capture.targets.erase(capture.targets.begin() + (long long) i);
			continue;
		}

		Coord target = capture.targets[i][0];
		Piece *piece = _cells[target[1]][target[0]];

		pieces[piece->getColor()]--;
		capture.nbPieces--;
		if (piece->isKing())
			capture.nbKings--;

		delete _cells[target[1]][target[0]];
		_cells[target[1]][target[0]] = nullptr;
		break;
	}

	for (decltype(capture.paths)::size_type i = 0; i < capture.paths.size(); i++)
	{
		if (next != capture.paths[i][0])
			continue;
		capture.targets[i].erase(capture.targets[i].begin());
		capture.paths[i].erase(capture.paths[i].begin());

		if (capture.paths[i].empty())
			capture.paths.erase(capture.paths.begin() + (long long) i);
		if (capture.targets[i].empty())
			capture.targets.erase(capture.targets.begin() + (long long) i);
	}
}

void Board::click(sf::Vector2i pos)
{
	std::array<int, 2> coord = getBoardCoord(pos);

	if (turn < 0)
		return;

	if (_cells[coord[1]][coord[0]] && _cells[coord[1]][coord[0]]->getColor() != (turn + 1) % 2)
		return;

	if (!_selected && _cells[coord[1]][coord[0]])
	{
		if (std::find(possibilities.begin(), possibilities.end(), _cells[coord[1]][coord[0]]) != possibilities.end())
		{
			_selected = _cells[coord[1]][coord[0]];
			capture = _selected->getReachable()[0].second;
		}
		else
			std::cerr << "Please select a possible piece." << std::endl;
	}
	else if (_selected)
	{
		auto old = _selected->getCoord();

		if (_cells[coord[1]][coord[0]] && !capture.nbPieces)
		{
			_selected = _cells[coord[1]][coord[0]];
			return;
		}
		if (capture.nbPieces)
		{
			bool found = false;
			for (size_t i = 0; !found && i < capture.paths.size(); i++)
				if (capture.paths[i][0] == coord)
					found = true;
			if (!found)
				return;
		}
		if (!_selected->move(coord))
		{
			_selected = nullptr;
			return;
		}
		remove_captured(coord);
		_cells[old[1]][old[0]] = nullptr;
		_cells[coord[1]][coord[0]] = _selected;

		if (!capture.nbPieces)
		{
			_selected = nullptr;
			turn++;
		}
	}
	else
		_selected = nullptr;
}

std::array<int, 2> Board::getBoardCoord(const sf::Vector2i &pos)
{
	sf::Vector2i offset((int) getPosition().x, (int) getPosition().y);
	std::array<int, 2> result{0, 0};
	int square = static_cast<int>(std::roundf((float) _size * _ratio));

	for (; offset.x + result[0] * square < pos.x; result[0]++);
	for (; offset.y + result[1] * square < pos.y; result[1]++);

	result[0] = std::max(std::min(--result[0], 9), 0);
	result[1] = std::max(std::min(--result[1], 9), 0);

	return result;
}

void Board::drawReachable(sf::RenderTarget &target, sf::RenderStates states) const
{
	float radius = (getCellSize() / 2) * 0.2f;
	sf::CircleShape shape(radius);
	auto reachable = _selected->getReachable();
	float offset = (getCellSize() - 2 * radius) / 2.f;

	float radiusOutline = (getCellSize() / 2.f) * Params::ratio[R_MAN];
	float offsetOutline = getCellSize() / 2.f;
	RingShape shading(64, radiusOutline, radius * Params::ratio[R_OUTLINE]);

	shape.setPointCount(128);
	shading.setPosition((float) _selected->getCoord()[0] * getCellSize() + offsetOutline,
	                    (float) _selected->getCoord()[1] * getCellSize() + offsetOutline);

	if (_selected->getColor() == Piece::BLACK)
		shading.setFullColor(Params::color[C_B_OUTLINE]);
	else
		shading.setFullColor(Params::color[C_W_OUTLINE]);
	shading.setInFadedColor(_selected->getDrawingColor());

	target.draw(shading, states);

	for (const auto &current : reachable)
	{
		auto paths = current.second.paths;

		shape.setFillColor(current.second.nbPieces ?
		                   Params::color[C_PREVISION_CAP] : Params::color[C_PREVISION_MOVE]);
		if (!current.second.nbPieces)
		{
			auto coord = current.first;
			shape.setPosition((float) coord[0] * getCellSize() + offset, (float) coord[1] * getCellSize() + offset);
			target.draw(shape, states);
		}
		else
		{
			for (const auto &path : paths)
			{
				auto coord = path[0];
				shape.setPosition((float) coord[0] * getCellSize() + offset, (float) coord[1] * getCellSize() + offset);
				target.draw(shape, states);
			}
		}
	}
}

void Board::drawPossibilities(sf::RenderTarget &target, sf::RenderStates &states) const
{
	float radius = (getCellSize() / 2) * 0.2f;
	sf::CircleShape shape(radius);

	float radiusOutline = (getCellSize() / 2.f) * Params::ratio[R_MAN];
	float offsetOutline = getCellSize() / 2.f;
	RingShape shading(64, radiusOutline, radius * Params::ratio[R_OUTLINE]);

	shape.setPointCount(128);

	shading.setOutFadedColor(Params::color[C_BG_BLACK]);
	shading.setFullColor(Params::color[C_M_OUTLINE]);

	for (const auto &current : possibilities)
	{
		shading.setInFadedColor(current->getDrawingColor());

		shading.setPosition((float) current->getCoord()[0] * getCellSize() + offsetOutline,
		                    (float) current->getCoord()[1] * getCellSize() + offsetOutline);

		target.draw(shading, states);
	}
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	auto size = static_cast<float>(_size);
	sf::RectangleShape rectangle(sf::Vector2f(size * _ratio, size * _ratio));
	sf::Color colors[2] = {Params::color[C_BG_BLACK], Params::color[C_BG_WHITE]};

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			rectangle.setFillColor(colors[(j + (i % 2)) % 2]);
			rectangle.setPosition(rectangle.getSize().x * (float) j, rectangle.getSize().y * (float) i);
			target.draw(rectangle, states);
			if (_cells[i][j])
				target.draw(*_cells[i][j], states);
		}
	drawPossibilities(target, states);
	if (_selected)
		drawReachable(target, states);
}

void Board::update()
{
	if (turn == -1)
		return;

	if (turn != -1 && checkEnd())
	{
		turn = -1;
		return;
	}

	checkPossibilities();

	if (_selected && capture.nbPieces)
		capture = _selected->getReachable()[0].second;

	if (Params::enableAI && (turn + 1) % 2 == ai)
		best_move();
}

bool Board::checkEnd() const
{
	int result = 0;

	if (!pieces.at(Piece::WHITE))
	{
		result = 1;
		std::cout << "Black wins" << std::endl;
	}
	if (!pieces.at(Piece::BLACK))
	{
		result = 2;
		std::cout << "White wins" << std::endl;
	}

	return result != 0;
}

int Board::checkEnd(const std::vector<std::vector<Piece *>> &state)
{
	bool w = false, b = false;

	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 10; x++)
		{
			const auto *piece = state[y][x];

			if (piece && piece->getColor() == Piece::BLACK)
				b = true;
			else if (piece)
				w = true;
			if (w && b)
				return -1;
		}
	if (w)
		return 1;
	else
		return 0;
}

void Board::checkPossibilities()
{
	possibilities.clear();

	if (_selected)
		return;

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			Piece *piece = _cells[y][x];
			if (!piece || piece->getColor() != (turn + 1) % 2)
				continue;

			auto piecePossibilities = piece->getReachable();

			if (!piecePossibilities.empty() && possibilities.empty())
				possibilities.push_back(piece);
			else if (!possibilities.empty())
			{
				int value = moveEval(piece), ref = moveEval(possibilities[0]);
				if (value == ref)
					possibilities.push_back(piece);
				else if (value > ref)
				{
					possibilities.clear();
					possibilities.push_back(piece);
				}
			}
		}
	}
}

int Board::moveEval(Piece *piece)
{
	auto piecePossibilities = piece->getReachable();

	if (piecePossibilities.empty())
		return -1;

	Piece::Capture cap = piecePossibilities[0].second;
	return !cap.nbPieces ? 0 : cap.nbPieces + cap.nbKings;
}

void Board::best_move()
{
	State copy = duplicate(_cells);
	Move move{};

	int val = std::numeric_limits<int>::min();
	int alpha = std::numeric_limits<int>::min();
	int beta = std::numeric_limits<int>::max();

	Sleep(200);

	for (size_t i = 0; i < possibilities.size(); i++)
	{
		const auto& possibility = possibilities[i];
		move.start = possibility->getCoord();
		auto states = getStates(copy, move.start);

		for (size_t j = 0; j < states.size(); j++)
		{
			auto& state = states[j];
			Move tmp = state.first;
			tmp.start = tmp.arrive;
			val = std::max(val, minmax(state.second, tmp, Params::depth, turn, true, alpha, beta));
			if (val >= beta)
			{
				move = state.first;
				break;
			}
			alpha = std::max(alpha, val);
		}
		if (val >= beta)
			break;
	}
	if (_cells[move.start[1]][move.start[0]]->move({move.arrive[0], move.arrive[1]}))
	{
		applyMovement(_cells, move);
		applyCapture(move.targets);
	}
	turn++;
}

int Board::minmax(const State &state, Move &move, int depth, int currentTurn, bool max, int alpha, int beta)
{
	if (depth < 0 || checkEnd(state) != -1)
		return evaluation(state, currentTurn, max);

	int val;
	Move m{};
	auto states = getStates(state, move.start);
	Coord old = move.start;
	if (!max)
	{
		val = std::numeric_limits<int>::max();

		for (const auto &currentState : states)
		{
			m = currentState.first;
			old = m.start;
			m.start = m.arrive;
			val = std::min(val, minmax(currentState.second, m, depth - 1,
			                           currentTurn + 1, !max, alpha, beta));
			if (alpha >= val)
			{
				m.start = old;
				move = m;
				return val;
			}
			beta = std::min(beta, val);
		}
	}
	else
	{
		val = std::numeric_limits<int>::min();

		for (const auto &currentState : states)
		{
			m = currentState.first;
			old = m.start;
			m.start = m.arrive;
			val = std::max(val, minmax(currentState.second, m, depth - 1,
			                           currentTurn + 1, !max, alpha, beta));
			if (val >= beta)
			{
				m.start = old;
				move = m;
				return val;
			}
			alpha = std::max(alpha, val);
		}

	}
	m.start = old;
	move = m;
	return val;
}

int Board::evaluation(const std::vector<std::vector<Piece *>> &state, int currentTurn, bool max)
{
	int end = checkEnd(state);

	if (end != -1)
		return end == (currentTurn + 1) % 2 ? 100 : -100;

	int piecesBlack = 0;
	int piecesWhite = 0;

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			const auto *piece = state[y][x];

			if (piece)
				(piece->getColor() == Piece::BLACK ? piecesBlack : piecesWhite)++;
		}
	}

	int signum;
	if (ai == (currentTurn + 1) % 2 && max)
		signum = 1;
	else
		signum = -1;
	return (piecesWhite - piecesBlack) * signum;
}

std::vector<std::pair<Board::Move, Board::State>> Board::getStates(const State &state, Coord start) const
{
	std::vector<std::pair<Move, State>> states;

	auto cell = state[start[1]][start[0]];

	if (!cell)
		return states;
	auto reachable = cell->getReachable();
	if (reachable.empty())
		return states;
	auto currentCapture = reachable[0].second;
	Move move{};

	move.start = start;

	if (currentCapture.nbPieces == 0)
	{
		for (const auto &coord : reachable)
		{
			move.arrive = coord.first;
			states.emplace_back(move, duplicate(state));
			applyMovement(states.back().second, move);
		}
	}
	else
	{
		std::vector<std::vector<Coord>> paths = currentCapture.paths;

		for (int i = 0; i < paths.size(); i++)
		{
			Coord lastTarget = currentCapture.targets[i].back();

			for (auto rit = paths[i].rbegin(); rit != paths[i].rend() && lastTarget != *rit; rit++)
			{
				move.arrive = *rit;
				move.targets = currentCapture.targets[i];
				states.emplace_back(move, duplicate(state));
				applyMovement(states.back().second, move);
			}
		}
	}
	return states;
}

void Board::applyMovement(State &state, Move move)
{
	state[move.arrive[1]][move.arrive[0]] = state[move.start[1]][move.start[0]];
	state[move.start[1]][move.start[0]] = nullptr;
}

void Board::applyCapture(const std::vector<Coord> &targets)
{
	for (const auto & targetCoord : targets)
	{
		Piece *& target = _cells[targetCoord[1]][targetCoord[0]];

		delete target;
		target = nullptr;
	}
}

void Board::setAI(Piece::Color color)
{
	ai = color;
}

Board::State Board::duplicate(const State &state)
{
	State dup;

	for (size_t y = 0; y < state.size(); y++)
	{
		dup.emplace_back();
		for (size_t x = 0; x < state[y].size(); x++)
		{
			if (state[y][x])
			{
				dup[y].push_back(new Piece(*state[y][x]));
				dup[y].back()->setBoard(dup);
			}
			else
				dup[y].push_back(nullptr);
		}
	}
	return dup;
}

void Board::free(State &state)
{
	for (size_t y = 0; y < state.size(); y++)
	{
		for (size_t x = 0; x < state.size(); x++)
		{
			delete state[y][x];
			state[y][x] = nullptr;
		}
	}
	state.clear();
}