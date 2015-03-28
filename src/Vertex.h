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
	Vertex(double _x, double _y);
	// Copy constructor
	Vertex(const Vertex &_other);
	// Destructor
	~Vertex();

	// Operator to be able to print in the stdout
	friend std::ostream &operator<<(std::ostream &_stream, const Vertex &_vertex);

	// Comparison operator
	bool operator==(const Vertex &_other) const;

	// Get x position
	inline double getX() const
	{
		return x;
	}

	// Set x position
	inline void setX(double x)
	{
		this->x = x;
	}

	// Get y position
	inline double getY() const
	{
		return y;
	}

	// Set y position
	inline void setY(double y)
	{
		this->y = y;
	}

private:
	double x;
	double y;
};

typedef shared_ptr<Vertex> VertexPtr;
