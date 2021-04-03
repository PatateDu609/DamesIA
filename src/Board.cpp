//
// Created by Teyber on 1/25/2021.
//

#include "Board.hpp"
#include <iostream>
#include "RingShape.hpp"
#include "Params.hpp"

Board *Board::instance = nullptr;

Board::Board(int size, float ratio) : _cells{}, _size(size), _ratio(ratio), _selected(
		nullptr)
{
	Piece::Color colors[] = {Piece::BLACK, Piece::WHITE};

	for (int i = 0; i < 10; i++) {
		_cells.emplace_back();
		for (int j = 0; j < 10; j++) {
			if ((j + (i % 2)) % 2 || (3 < i && i < 6))
				_cells[i].push_back(nullptr);
			else
				_cells[i].push_back(new Piece(colors[i >= 6], this, std::array<int, 2>{j, i}));
		}
	}
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

float Board::getCellSize() const
{
	return static_cast<float>(_size) * _ratio;
}

void Board::click(sf::Vector2i pos)
{
	std::array<int, 2> coord = getBoardCoord(pos);

	if (!_selected && _cells[coord[1]][coord[0]])
		_selected = _cells[coord[1]][coord[0]];
	else if (_selected) {
		auto old = _selected->getCoord();

		if (_cells[coord[1]][coord[0]]) {
			_selected = _cells[coord[1]][coord[0]];
			return;
		}
		if (!_selected->move(coord)) {
			_selected = nullptr;
			return;
		}
		_cells[old[1]][old[0]] = nullptr;
		_cells[coord[1]][coord[0]] = _selected;
		_selected = nullptr;
	}
	else
		_selected = nullptr;
}

std::array<int, 2> Board::getBoardCoord(const sf::Vector2i &pos)
{
	sf::Vector2i offset(getPosition().x, getPosition().y);
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
	std::vector<std::array<int, 2>> reachable = _selected->getReachable();
	float offset = (getCellSize() - 2 * radius) / 2.f;

	float radiusOutline = (getCellSize() / 2.f) * Params::ratio[R_MAN];
	float offsetOutline = getCellSize() / 2.f;
	RingShape shading(64, radiusOutline, radius * Params::ratio[R_OUTLINE]);

	shape.setPointCount(128);
	shading.setPosition(_selected->getCoord()[0] * getCellSize() + offsetOutline,
	                    _selected->getCoord()[1] * getCellSize() + offsetOutline);

	if (_selected->getColor() == Piece::BLACK)
		shading.setFullColor(Params::color[C_B_OUTLINE]);
	else
		shading.setFullColor(Params::color[C_W_OUTLINE]);
	shading.setInFadedColor(_selected->getDrawingColor());

	target.draw(shading, states);

	shape.setFillColor(Params::color[C_PREVISION]);
	for (auto current : reachable) {
		shape.setPosition(current[0] * getCellSize() + offset, current[1] * getCellSize() + offset);
		target.draw(shape, states);
	}
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	auto size = static_cast<float>(_size);
	sf::RectangleShape rectangle(sf::Vector2f(size * _ratio, size * _ratio));
	sf::Color colors[2] = {Params::color[C_BG_BLACK], Params::color[C_BG_WHITE]};

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			rectangle.setFillColor(colors[(j + (i % 2)) % 2]);
			rectangle.setPosition(rectangle.getSize().x * (float) j, rectangle.getSize().y * (float) i);
			target.draw(rectangle, states);
			if (_cells[i][j])
				target.draw(*_cells[i][j], states);
		}
	if (_selected)
		drawReachable(target, states);
}
