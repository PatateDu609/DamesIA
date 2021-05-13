//
// Created by Teyber on 1/25/2021.
//

#include <iostream>
#include "Piece.hpp"
#include "Params.hpp"

#include <fstream>

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

coord Piece::getCoord() const
{
	return _coord;
}

bool Piece::valid(const coord &coord) const
{
	auto reachable = getReachable();

	return std::find(reachable.begin(), reachable.end(), coord) != reachable.end();
}

bool Piece::move(const coord &coord)
{
	if (!valid(coord))
		return false;
	_coord = coord;
	if (!_isKing && _coord[1] == (_color == BLACK ? 8 : 0))
		_isKing = true;
	return true;
}

std::vector<coord> Piece::getReachable() const
{
	std::vector<coord> reachable;

	int factor = _color == WHITE ? -1 : 1;
	int newY = _coord[1] + factor;

	if (_coord[0] && _board->getCell(_coord[0] - 1, newY) == nullptr)
		reachable.push_back(std::array<int, 2>{_coord[0] - 1, newY});
	if (_coord[0] < 9 && _board->getCell(_coord[0] + 1, newY) == nullptr)
		reachable.push_back(std::array<int, 2>{_coord[0] + 1, newY});

	Capture capture{};
	decltype(reachable) dirs;
	dirs.push_back({1, factor});
	dirs.push_back({-1, factor});

	if (check_capture(_coord, dirs, capture))
	{
		for (const auto &path : capture.paths)
			reachable.insert(reachable.end(), path.begin(), path.end());
	}
	return reachable;
}

bool Piece::check_capture(const coord &start, const std::vector<coord> &dirs, Capture &quality) const
{
	std::vector<Capture> captures;
	std::vector<std::pair<coord, coord>> targets;

	for (const auto &dir : dirs)
	{
		coord target;
		if (!search_capture(start, dir, target))
			continue;
		targets.emplace_back(target, dir);
	}
	if (targets.empty())
		return false;

	captures.assign(targets.size(), quality);

	for (size_t i = 0; i < captures.size(); i++)
	{
		auto cell = _board->getCell(targets[i].first[0], targets[i].first[1]);
		coord nextStart{
			targets[i].first[0] + targets[i].second[0],
			targets[i].first[1] + targets[i].second[1]
		};

		captures[i].nbPieces++;
		if (cell->_isKing)
			captures[i].nbKings++;

		if (captures[i].paths.empty())
			captures[i].paths.emplace_back();
		for (auto &path : captures[i].paths)
			path.push_back(nextStart);

		check_capture(nextStart, dirs, captures[i]);
	}
	Capture old = quality;
	for (const auto &capture : captures)
	{
		if (capture > quality)
			quality = capture;
		else if (capture == quality)
			quality.paths.insert(quality.paths.end(), capture.paths.begin(), capture.paths.end());
	}
	return old != quality;
}

bool Piece::search_capture(coord start, const coord &dir, coord &target) const
{
	coord real = {start[0] + dir[0], start[1] + dir[1]};
	coord next = {real[0] + dir[0], real[1] + dir[1]};

	if (real[0] < 0 || real[0] > 9 ||
	    real[1] < 0 || real[1] > 9)
		return false;
	if (next[0] < 0 || next[0] > 9 ||
	    next[1] < 0 || next[1] > 9)
		return false;
	auto cell = _board->getCell(real[0], real[1]);
	auto nextCell = _board->getCell(next[0], next[1]);

	target = real;

	return cell && cell->getColor() != _color && !nextCell;
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
