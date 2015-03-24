/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include <vector>
#include "Vertex.h"
#include <ostream>
#include <memory>

using namespace std;

class Triangle
{
public:
	// Constructor
	Triangle(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3);
	// Destructor
	~Triangle();

	friend std::ostream &operator<<(std::ostream &_stream, const Triangle &_triangle);

	// Returns the desired neighbor
	inline Triangle * getNeighbor(const int index) const
	{
		return neighbors[index % 3];
	}

	// Returns the desired vertex
	inline Vertex * getVertex(const int index) const
	{
		return vertices[index % 3];
	}

	// Returns the array of vertices of this triangle
	inline vector<Vertex *> getVertices() const
	{
		return vertices;
	}

	// Checks if the given vertex is inside the current triangle
	bool contains(const Vertex *_vertex) const;
	// Returns a boolean value indicating if the given vertex is in one of the triangle's edges
	bool inEdge(const Vertex * _vertex) const;
	// Sets the given triangle as a neighbor. If it doesn't share vertices, then it isn't set as neighbor.
	bool setNeighbor(const Triangle *_t);
	// Returns a boolean value indicating if the given triangle is a neighbor
	bool isNeighbor(const Triangle *_t) const;

private:
	unsigned long id;
	vector<Vertex *> vertices;
	vector<Triangle *> neighbors;

	// Adds the vertices in CCW
	void addInCCW(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3);
	// Id to make easier the identification of each triangle
//	unsigned long getId() const;
};
