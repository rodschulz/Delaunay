/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include <ostream>

class Vertex
{
public:
	Vertex();
	Vertex(float _x, float _y);
	~Vertex();

	float getX() const
	{
		return x;
	}

	void setX(float x)
	{
		this->x = x;
	}

	float getY() const
	{
		return y;
	}

	void setY(float y)
	{
		this->y = y;
	}

	// Operator to be able to print the vertices in stdout
	friend std::ostream &operator<<(std::ostream &_stream, const Vertex &_vertex);

private:
	float x;
	float y;
};
