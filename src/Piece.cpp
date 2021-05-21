//
// Created by Teyber on 1/25/2021.
//

#include <iostream>
#include <utility>
#include "Piece.hpp"
#include "Params.hpp"

sf::Texture *Piece::_texture = nullptr;

bool Piece::Capture::operator>(const Capture &a) const
{
	return nbPieces > a.nbPieces || nbKings > a.nbKings;
}

bool Piece::Capture::operator==(const Capture &a) const
{
	return nbPieces == a.nbPieces && nbKings == a.nbKings;
}

bool Piece::Capture::operator!=(const Capture &a) const
{
	return !(*this == a);
}

Piece::Piece(Piece::Color color, Board *board, std::array<int, 2> coord) : _color(color), _board(board), _coord(coord),
                                                                           _isKing(false)
{
	_drawingColor = _color == BLACK ? Params::color[C_MAN_BLACK] : Params::color[C_MAN_WHITE];

	if (_texture)
		return;

	_texture = new sf::Texture();
	if (!_texture->loadFromFile(Params::crown_path))
	{
		std::cout << "ERROR : crown.png could not be loaded!" << std::endl;
		return;
	}

	_texture->setSmooth(true);
}

Piece::Piece(Color color, std::vector<std::vector<Piece *>> b, Coord coord) :
	_color(color), board(std::move(b)), _coord(coord), _isKing(false), _board(nullptr)
{

}

Piece::Color Piece::getColor() const
{
	return _color;
}

sf::Color Piece::getDrawingColor() const
{
	return _drawingColor;
}

Coord Piece::getCoord() const
{
	return _coord;
}

bool Piece::valid(const Coord &coord)
{
	auto reachable = getReachable();

	return std::find_if(reachable.begin(), reachable.end(), [coord](const std::pair<Coord, Capture> &possible) {
		return coord == possible.first;
	}) != reachable.end();
}

bool Piece::move(const Coord &coord)
{
	if (!valid(coord))
		return false;
	_coord = coord;
	if (!_isKing && _coord[1] == (_color == BLACK ? 9 : 0))
		_isKing = true;
	return true;
}

std::vector<Coord> Piece::expand_move(Coord start, Coord dir) const
{
	std::vector<Coord> expanded{start};

	for (int k = 1; k < 9; k++)
	{
		Coord current{
			start[0] + k * dir[0],
			start[1] + k * dir[1]
		};

		if (current[0] < 0 || current[0] > 9 ||
		    current[1] < 0 || current[1] > 9)
			return expanded;
		if (getCell(current))
			return expanded;
		expanded.push_back(current);
	}
	return expanded;
}

void Piece::king_moves(std::vector<std::pair<Coord, Piece::Capture>> &reachable, const std::vector<Coord> &dirs) const
{
	for (const auto &dir : dirs)
	{
		Coord start{
			_coord[0] + dir[0],
			_coord[1] + dir[1]
		};

		if (start[0] < 0 || start[0] > 9 ||
		    start[1] < 0 || start[1] > 9)
			continue;
		if (getCell(_coord))
			continue;
		std::vector<Coord> expanded = expand_move(start, dir);

		for (const Coord coord : expanded)
			reachable.emplace_back(coord, Capture{});
	}
}

std::vector<Coord> Piece::regenerate_dirs(const Coord &last)
{
	std::vector<Coord> dirs;

	dirs.push_back({1, 1});
	dirs.push_back({-1, 1});
	dirs.push_back({1, -1});
	dirs.push_back({-1, -1});

	dirs.erase(std::find(dirs.begin(), dirs.end(), Coord{-last[0], -last[1]}));

	return dirs;
}

std::vector<std::pair<Coord, Piece::Capture>> Piece::getReachable()
{
	std::vector<std::pair<Coord, Capture>> reachable;

	int factor = _color == WHITE ? -1 : 1;
	int newY = _coord[1] + factor;

	std::vector<Coord> dirs;
	dirs.push_back({1, factor});
	dirs.push_back({-1, factor});

	if (_isKing)
	{
		dirs.push_back({1, -factor});
		dirs.push_back({-1, -factor});
	}

	if (_isKing)
		king_moves(reachable, dirs);
	else
	{
		if (_coord[0] < 9 && !getCell(_coord[0] + 1, newY))
			reachable.emplace_back(Coord{_coord[0] + 1, newY}, Capture{});
		if (_coord[0] && !getCell(_coord[0] - 1, newY))
			reachable.emplace_back(Coord{_coord[0] - 1, newY}, Capture{});
	}

	Capture capture{};

	if (check_capture(_coord, dirs, capture))
	{
		reachable.clear();
		for (const auto &path : capture.paths)
			for (auto step : path)
				reachable.emplace_back(step, capture);
	}
	return reachable;
}

bool Piece::check_capture(const Coord &start, std::vector<Coord> dirs, Capture &quality)
{
	std::vector<Capture> captures;
	std::vector<std::pair<Coord, Coord>> targets;

	for (const auto &dir : dirs)
	{
		Coord target;
		if (!search_capture(start, dir, target))
			continue;
		targets.emplace_back(target, dir);
	}
	if (targets.empty())
		return false;

	for (std::pair<Coord, Coord> &target : targets)
	{
		auto cell = getCell(target.first);
		Coord startTmp{
			target.first[0] + target.second[0],
			target.first[1] + target.second[1]
		};
		std::vector<Coord> nextStart;
		if (_isKing)
			nextStart = expand_move(startTmp, target.second);
		else
			nextStart.assign(1, startTmp);

		for (Coord next : nextStart)
		{
			captures.push_back(quality);
			auto &currentCapture = captures.back();

			if (!currentCapture.targets.empty())
			{
				for (int i = 0; i < currentCapture.targets.size(); i++)
				{
					auto it = std::find(currentCapture.targets[i].begin(), currentCapture.targets[i].end(),
					                    target.first);

					if (it != currentCapture.targets[i].end())
					{
						currentCapture.targets.erase(currentCapture.targets.begin() + i);
						currentCapture.paths.erase(currentCapture.paths.begin() + i);
					}
				}
				if (currentCapture.targets.empty())
				{
					captures.pop_back();
					break;
				}
			}

			currentCapture.nbPieces++;
			if (cell->_isKing)
				currentCapture.nbKings++;

			if (currentCapture.paths.empty())
			{
				currentCapture.paths.emplace_back();
				currentCapture.targets.emplace_back();
			}
			for (auto &path : currentCapture.paths)
				path.push_back(next);
			for (auto &targetsCapture : currentCapture.targets)
				targetsCapture.push_back(target.first);

			if (_isKing)
				dirs = regenerate_dirs(target.second);
			check_capture(next, dirs, currentCapture);
		}
	}
	Capture old = quality;
	for (auto capture : captures)
	{
		if (capture > quality)
			quality = capture;
		else if (capture == quality)
		{
			quality.paths.insert(quality.paths.end(), capture.paths.begin(), capture.paths.end());
			quality.targets.insert(quality.targets.end(), capture.targets.begin(), capture.targets.end());
		}
	}
	return old != quality;
}

bool Piece::search_capture(Coord start, const Coord &dir, Coord &target) const
{
	Coord targetTmp;
	Coord nextTargetTmp;
	const Piece *cell = nullptr;
	const Piece *nextCell;

	for (int k = 1; k < (_isKing ? 10 : 2); k++)
	{
		targetTmp = {start[0] + k * dir[0], start[1] + k * dir[1]};
		nextTargetTmp = {targetTmp[0] + dir[0], targetTmp[1] + dir[1]};

		if (targetTmp[0] < 0 || targetTmp[0] > 9 ||
		    targetTmp[1] < 0 || targetTmp[1] > 9)
			return false;
		if (nextTargetTmp[0] < 0 || nextTargetTmp[0] > 9 ||
		    nextTargetTmp[1] < 0 || nextTargetTmp[1] > 9)
			return false;
		cell = getCell(targetTmp);
		nextCell = getCell(nextTargetTmp);

		if (cell)
			break;
	}
	target = targetTmp;
	return cell && cell->getColor() != _color && !nextCell;
}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!_board)
		return;

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
	if (!_texture)
		throw std::runtime_error("_texture should not be nullptr");

	sf::Sprite sp;
	sp.setTexture(*_texture);

	sf::Vector2f texSize((float) _texture->getSize().x, (float) _texture->getSize().y);
	sp.setOrigin(texSize * .5f);

	sp.setScale(sf::Vector2f(radius / texSize.x, radius / texSize.y));
	sp.setPosition(pos + sf::Vector2f(_board->getCellSize(), _board->getCellSize()) * .5f);
	sp.setColor(sf::Color::White);
	target.draw(sp, states);
}

bool Piece::isKing() const
{
	return _isKing;
}

const Piece *Piece::getCell(Coord coord) const
{
	return _board ? _board->getCell(coord[0], coord[1]) : board[coord[1]][coord[0]];
}

Piece *Piece::getCell(Coord coord)
{
	return _board ? _board->getCell(coord[0], coord[1]) : board[coord[1]][coord[0]];
}

const Piece *Piece::getCell(int x, int y) const
{
	return getCell({x, y});
}

Piece *Piece::getCell(int x, int y)
{
	return getCell({x, y});
}

void Piece::setBoard(const Board::State &b)
{
	board = b;
}
