/**
 * Author: rodrigo
 * 2015
 */

#include <stdlib.h>
#include "Triangle.h"
#include "Helper.h"

Triangle::Triangle(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3)
{
	addInCCW(_vertex1, _vertex2, _vertex3);
	neighbors = vector<Triangle *>(3, NULL);
}

Triangle::~Triangle()
{
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
	return (Helper::getOrientation(vertices[0], vertices[1], _vertex) >= 0)
	                && (Helper::getOrientation(vertices[1], vertices[2], _vertex) >= 0)
	                && (Helper::getOrientation(vertices[2], vertices[0], _vertex) >= 0);
}

vector<Triangle> Triangle::split(const Vertex &_vertex) const
{
	vector<Triangle> triangles = vector<Triangle>();
	if (contains(&_vertex))
	{
		for (int i = 0; i < 3; i++)
		{
			if (!Helper::areCollinear(vertices[i], vertices[(i + 1) % 3], &_vertex))
			{
				Triangle t = Triangle(vertices[i], vertices[(i + 1) % 3], &_vertex);
				t.setNeighbors();
				triangles.push_back();
			}
		}
	}

	return triangles;
}

void Triangle::setNeighbors(const Triangle *_t1, const Triangle *_t2, const Triangle *_t3)
{}

std::ostream &operator<<(std::ostream &_stream, const Triangle &_triangle)
{
	_stream << _triangle.vertices[0] << " | " << _triangle.vertices[1] << " | "
	                << _triangle.vertices[2];
	return _stream;
}
