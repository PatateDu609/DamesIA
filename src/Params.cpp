//
// Created by Teyber on 1/27/2021.
//

#include "Params.hpp"

std::map<ColorType, sf::Color> Params::color = {
		{C_BG_BLACK,  sf::Color(10, 10, 26)},
		{C_BG_WHITE,  sf::Color(168, 178, 213)},
		{C_MAN_BLACK, sf::Color(61, 64, 156)},
		{C_MAN_WHITE, sf::Color(255, 212, 197)},
		{C_B_OUTLINE, sf::Color(214, 202, 169)},
		{C_W_OUTLINE, sf::Color(202, 96, 141)},
		{C_PREVISION, sf::Color(214, 159, 181)}
};

std::map<RatioType, float> Params::ratio = {
		{R_MAN,     0.8f},
		{R_CELL,    0.1f},
		{R_OUTLINE, 0.35f}
};

int Params::size = 1024;

std::string Params::crown_path = "";
