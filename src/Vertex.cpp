/**
 * Author: rodrigo
 * 2015
 */

#include "Vertex.h"

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

std::ostream &operator<<(std::ostream &_stream, const Vertex &_vertex)
{
	_stream << "[" << _vertex.getX() << ", " << _vertex.getY() << "]";
	return _stream;
}
