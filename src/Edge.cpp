/**
 * Author: rodrigo
 * 2015
 */
#include "Edge.h"

Edge::Edge(const VertexPtr &_vertex1, const VertexPtr &_vertex2)
{
	vertex1 = _vertex1;
	vertex2 = _vertex2;
}

Edge::~Edge()
{
}

bool Edge::equals(const Edge &_other, const bool &_oriented) const
{
	if (_oriented)
		return vertex1 == _other.vertex1 && vertex2 == _other.vertex2;
	else
		return (vertex1 == _other.vertex1 && vertex2 == _other.vertex2) || (vertex1 == _other.vertex2 && vertex2 == _other.vertex1);
}

bool Edge::contains(const VertexPtr &_vertex) const
{
	return _vertex == vertex1 || _vertex == vertex2;
}

std::ostream &operator<<(std::ostream &_stream, const Edge &_edge)
{
	_stream << *_edge.vertex1 << " - " << *_edge.vertex2;
	return _stream;
}

VertexPtr Edge::getVertex(const int _n) const
{
	if (_n == 0)
		return vertex1;
	else
		return vertex2;
}
