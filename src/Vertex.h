/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include <ostream>
#include <memory>

using namespace std;

class Vertex
{
public:
	// Empty constructor
	Vertex();
	// Constructor
	Vertex(float _x, float _y);
	// Copy constructor
	Vertex(const Vertex &_other);
	// Destructor
	~Vertex();

	// Operator to be able to print in the stdout
	friend std::ostream &operator<<(std::ostream &_stream, const Vertex &_vertex);

	// Comparison operator
	bool operator==(const Vertex &_other) const;

	/** Getters and setters */
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

private:
	float x;
	float y;
};

typedef shared_ptr<Vertex> VertexPtr;
