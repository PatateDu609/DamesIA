//
// Created by Teyber on 1/26/2021.
//

#include "RingShape.hpp"
#include <numbers>
#include <cmath>

#define PI std::numbers::pi

RingShape::RingShape(int sides, float radius, float outline) : _vertices(sf::Quads, sides * 8)
{
	double x, y, oldX, oldY;
	double outX, outY, oldOutX, oldOutY;
	double inX, inY, oldInX, oldInY;

	sf::Color full(sf::Color::Blue), faded(sf::Color::Black);

	oldX = radius;
	oldY = 0;
	oldOutX = radius + outline;
	oldOutY = outline;
	oldInX = radius - outline;
	oldInY = -outline;

	for (int i = 0, j = 0; i < sides * 8; i += 4, j++)
	{
		double angle = 2. * PI * ((j + 1) % sides) / sides;
		x = oldX;
		y = oldY;
		outX = oldOutX;
		outY = oldOutY;
		inX = oldInX;
		inY = oldInY;

		// Bordure intérieure
		_vertices[i].position = sf::Vector2f(x, y);
		_vertices[i].color = full;

		oldX = radius * std::cos(angle);
		oldY = radius * std::sin(angle);
		_vertices[i + 1].position = sf::Vector2f(oldX, oldY);
		_vertices[i + 1].color = full;

		oldInX = oldX - outline * (oldX < 0 ? -1.f : 1.f);
		oldInY = oldY - outline * (oldY < 0 ? -1.f : 1.f);
		_vertices[i + 2].position = sf::Vector2f(oldInX, oldInY);
		_vertices[i + 2].color = faded;

		_vertices[i + 3].position = sf::Vector2f(inX, inY);
		_vertices[i + 3].color = faded;


		i += 4; // Bordure extérieure
		_vertices[i].position = sf::Vector2f(x, y);
		_vertices[i].color = full;

		oldX = radius * std::cos(angle);
		oldY = radius * std::sin(angle);
		_vertices[i + 1].position = sf::Vector2f(oldX, oldY);
		_vertices[i + 1].color = full;

		oldOutX = oldX + outline * (oldX < 0 ? -1.f : 1.f);
		oldOutY = oldY + outline * (oldY < 0 ? -1.f : 1.f);
		_vertices[i + 2].position = sf::Vector2f(oldOutX, oldOutY);
		_vertices[i + 2].color = faded;

		_vertices[i + 3].position = sf::Vector2f(outX, outY);
		_vertices[i + 3].color = faded;
	}
}

void RingShape::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(_vertices, states);
}

void RingShape::setOutFadedColor(sf::Color color)
{
	for (size_t i = 0; i < _vertices.getVertexCount(); i += 8)
	{
		_vertices[i + 6].color = color;
		_vertices[i + 7].color = color;
	}
}

void RingShape::setInFadedColor(sf::Color color)
{
	for (size_t i = 0; i < _vertices.getVertexCount(); i += 8)
	{
		_vertices[i + 2].color = color;
		_vertices[i + 3].color = color;
	}
}

void RingShape::setFullColor(sf::Color color)
{
	for (size_t i = 0; i < _vertices.getVertexCount(); i += 8)
	{
		_vertices[i].color = color;
		_vertices[i + 1].color = color;
		_vertices[i + 4].color = color;
		_vertices[i + 5].color = color;
	}
}
