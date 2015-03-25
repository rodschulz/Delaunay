/**
 * Author: rodrigo
 * 2015
 */

#include <stdlib.h>
#include "Triangle.h"
#include "Helper.h"

Triangle::Triangle(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3)
{
	id = getId();
	addInCCW(_vertex1, _vertex2, _vertex3);
	neighbors = vector<Triangle *>(3, NULL);
}

Triangle::~Triangle()
{
}

unsigned long Triangle::getId() const
{
	static unsigned long nextId = 0;
	return nextId++;
}

void Triangle::addInCCW(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3)
{
	if (Helper::getOrientation(_vertex1, _vertex2, _vertex3) > 0)
	{
		vertices.push_back((Vertex *) _vertex1);
		vertices.push_back((Vertex *) _vertex2);
		vertices.push_back((Vertex *) _vertex3);
	}
	else
	{
		vertices.push_back((Vertex *) _vertex1);
		vertices.push_back((Vertex *) _vertex3);
		vertices.push_back((Vertex *) _vertex2);
	}
}

bool Triangle::contains(const Vertex *_vertex) const
{
	return (Helper::getOrientation(vertices[0], vertices[1], _vertex) >= 0) && (Helper::getOrientation(vertices[1], vertices[2], _vertex) >= 0) && (Helper::getOrientation(vertices[2], vertices[0], _vertex) >= 0);
}

bool Triangle::inEdge(const Vertex * _vertex) const
{
	for (int i = 0; i < 3; i++)
	{
		if (!Helper::areCollinear(vertices[i], vertices[(i + 1) % 3], _vertex))
			return true;
	}

	return false;
}

bool Triangle::setNeighbor(const Triangle *_t)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((vertices[i] == _t->vertices[(j + 1) % 3]) && (vertices[(i + 1) % 3] == _t->vertices[j]))
			{
				neighbors[i] = (Triangle *) _t;
				return true;
			}
		}
	}

	return false;
}

bool Triangle::isNeighbor(const Triangle *_t) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((vertices[i] == _t->vertices[j]) && (vertices[(i + 1) % 3] == _t->vertices[(j + 1) % 3]))
				return true;
		}
	}

	return false;
}

std::ostream &operator<<(std::ostream &_stream, const Triangle &_triangle)
{
	_stream << *_triangle.vertices[0] << "\n" << *_triangle.vertices[1] << "\n"
	                << *_triangle.vertices[2] << "\n" << *_triangle.vertices[0] << "\n\n";
	return _stream;
}
