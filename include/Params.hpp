//
// Created by Teyber on 1/27/2021.
//

#ifndef DAMES_PARAMS_HPP
#define DAMES_PARAMS_HPP

#include <SFML/Graphics.hpp>

enum ColorType
{
	C_BG_BLACK,
	C_BG_WHITE,
	C_MAN_BLACK,
	C_MAN_WHITE,
	C_B_OUTLINE,
	C_W_OUTLINE,
	C_PREVISION
};

enum RatioType
{
	R_CELL,
	R_MAN,
	R_OUTLINE
};

struct Params
{
	static std::map<ColorType, sf::Color> color;
	static std::map<RatioType, float> ratio;
	static int size;
};

#endif //DAMES_PARAMS_HPP
