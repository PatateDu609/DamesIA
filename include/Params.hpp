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
	C_M_OUTLINE,
	C_PREVISION_MOVE,
	C_PREVISION_CAP
};

enum RatioType
{
	R_CELL,
	R_MAN,
	R_OUTLINE
};

enum Difficulty
{
	EASY,
	MEDIUM,
	HARD
};

struct Params
{
	static std::map<ColorType, sf::Color> color;
	static std::map<RatioType, float> ratio;
	static int size;
	static const char *crown_path;

	static Difficulty difficulty;
	static int depth;

	static bool enableAI;
};

#endif //DAMES_PARAMS_HPP
