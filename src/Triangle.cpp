/**
 * Author: rodrigo
 * 2015
 */

#include "Triangle.h"

Triangle::Triangle(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3)
{
	vertices.push_back((Vertex *)_vertex1);
	vertices.push_back((Vertex *)_vertex2);
	vertices.push_back((Vertex *)_vertex3);
}

Triangle::~Triangle()
{
}

