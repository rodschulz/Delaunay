/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include "Vertex.h"

class Edge
{
public:
	Edge(const VertexPtr &_vertex1, const VertexPtr &_vertex2);
	~Edge();

	// Operator to print in the stdout
	friend std::ostream &operator<<(std::ostream &_stream, const Edge &_edge);

	// Checks if two edges are equal (true) or not (false)
	bool equals(const Edge &_other, const bool &_oriented = false) const;
	// Checks if the given vertex is part of the current edge
	bool contains(const VertexPtr &_vertex) const;

private:
	VertexPtr vertex1;
	VertexPtr vertex2;
};

