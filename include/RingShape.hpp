//
// Created by Teyber on 1/26/2021.
//

#ifndef DAMES_RINGSHAPE_HPP
#define DAMES_RINGSHAPE_HPP

#include <SFML/Graphics.hpp>

class RingShape : public sf::Drawable, public sf::Transformable
{
public:
	explicit RingShape(int sides, float radius, float outline);

	void setOutFadedColor(sf::Color color);
	void setInFadedColor(sf::Color color);
	void setFullColor(sf::Color color);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::VertexArray _vertices;
};


#endif //DAMES_RINGSHAPE_HPP
