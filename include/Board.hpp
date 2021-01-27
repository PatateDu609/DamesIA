//
// Created by Teyber on 1/25/2021.
//

#ifndef DAMES_BOARD_HPP
#define DAMES_BOARD_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Man.hpp"

class Board : public sf::Drawable, public sf::Transformable
{
public:
	~Board();

	Board(const Board &board) = delete;

	Board &operator=(const Board &board) = delete;

	static Board *getInstance(int size, float radius);
	static void resetInstance();
	[[nodiscard]] const Man* getCell(int x, int y) const;
	float getCellSize() const;

	void click(sf::Vector2i pos);

private:
	Board(int size, float radius);

	std::array<int, 2> getBoardCoord(const sf::Vector2i& pos);
	void drawReachable(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<std::vector<Man *>> _cells{};
	static Board *instance;
	float _ratio;
	int _size;
	Man *_selected;
};

#endif //DAMES_BOARD_HPP
