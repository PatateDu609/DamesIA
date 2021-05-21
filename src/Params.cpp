//
// Created by Teyber on 1/27/2021.
//

#include "Params.hpp"

std::map<ColorType, sf::Color> Params::color = {
	{C_BG_BLACK,       sf::Color(10, 10, 26)},
	{C_BG_WHITE,       sf::Color(168, 178, 213)},
	{C_MAN_BLACK,      sf::Color(61, 64, 156)},
	{C_MAN_WHITE,      sf::Color(255, 212, 197)},
	{C_B_OUTLINE,      sf::Color(214, 202, 169)},
	{C_W_OUTLINE,      sf::Color(202, 96, 141)},
	{C_M_OUTLINE,      sf::Color(214, 85, 41)},
	{C_PREVISION_MOVE, sf::Color(159, 214, 162)},
	{C_PREVISION_CAP,  sf::Color(214, 159, 181)}
};

std::map<RatioType, float> Params::ratio = {
	{R_MAN,     0.8f},
	{R_CELL,    0.1f},
	{R_OUTLINE, 0.35f}
};

int Params::size = 1024;

const char *Params::crown_path = "crown.png";

Difficulty Params::difficulty = HARD;
int Params::depth = 6;
bool Params::enableAI = false;