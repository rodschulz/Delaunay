/**
 * Author: rodrigo
 * 2015
 */

#include <stdlib.h>
#include "Triangle.h"
#include "Helper.h"

Triangle::Triangle(VertexPtr &_vertex1, VertexPtr &_vertex2, VertexPtr _vertex3)
{
	id = getId();
	addInCCW(_vertex1, _vertex2, _vertex3);
	neighbors = vector<TrianglePtr>(3, NULL);
}

Triangle::Triangle(const Triangle &_other)
{
	vertices = _other.vertices;
	neighbors = _other.neighbors;
}

Triangle::~Triangle()
{
}

unsigned long Triangle::getId() const
{
	static unsigned long nextId = 0;
	return nextId++;
}

void Triangle::addInCCW(const VertexPtr &_vertex1, const VertexPtr &_vertex2, const VertexPtr &_vertex3)
{
	if (Helper::getOrientation(_vertex1.get(), _vertex2.get(), _vertex3.get()) > 0)
	{
		vertices.push_back(_vertex1);
		vertices.push_back(_vertex2);
		vertices.push_back(_vertex3);
	}
	else
	{
		vertices.push_back(_vertex1);
		vertices.push_back(_vertex3);
		vertices.push_back(_vertex2);
	}
}

bool Triangle::contains(const VertexPtr &_vertex) const
{
	return (Helper::getOrientation(vertices[0].get(), vertices[1].get(), _vertex.get()) >= 0)
			&& (Helper::getOrientation(vertices[1].get(), vertices[2].get(), _vertex.get()) >= 0)
			&& (Helper::getOrientation(vertices[2].get(), vertices[0].get(), _vertex.get()) >= 0);
}

bool Triangle::inEdge(const Vertex * _vertex) const
{
	for (int i = 0; i < 3; i++)
	{
		if (!Helper::areCollinear(vertices[i].get(), vertices[(i + 1) % 3].get(), _vertex))
			return true;
	}

	return false;
}

bool Triangle::setNeighbor(TrianglePtr _t)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((vertices[i] == _t->vertices[(j + 1) % 3]) && (vertices[(i + 1) % 3] == _t->vertices[j]))
			{
				neighbors[i] = _t;
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
