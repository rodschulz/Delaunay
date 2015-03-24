/**
 * Author: rodrigo
 * 2015
 */

#include "Vertex.h"
#include "Helper.h"

Vertex::Vertex()
{
	x = y = 0;
}

Vertex::Vertex(const float _x, const float _y)
{
	x = _x;
	y = _y;
}

Vertex::~Vertex()
{
}

bool Vertex::operator==(const Vertex &_other) const
{
	return abs(x - _other.x) < COMPARISON_EPSILON && abs(y - _other.y) < COMPARISON_EPSILON;
}

std::ostream &operator<<(std::ostream &_stream, const Vertex &_vertex)
{
	_stream << _vertex.getX() << "\t" << _vertex.getY();
	return _stream;
}
