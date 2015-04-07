/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include <vector>
#include "Vertex.h"
#include "Edge.h"
#include <ostream>
#include <memory>

using namespace std;

class Triangle;
typedef shared_ptr<Triangle> TrianglePtr;

class Triangle
{
public:
	// Constructor
	Triangle(VertexPtr &_vertex1, VertexPtr &_vertex2, VertexPtr _vertex3);
	// Copy constructor
	Triangle(const Triangle &_other);
	// Destructor
	~Triangle();

	// Operator to print in the stdout
	friend std::ostream &operator<<(std::ostream &_stream, const Triangle &_triangle);

	// Checks if the given vertex is inside the current triangle
	bool contains(const VertexPtr &_vertex) const;
	// Returns a boolean value indicating if the given vertex is in one of the triangle's edges
	bool inEdge(const VertexPtr &_vertex) const;
	// Sets the given triangle as a neighbor. If it doesn't share vertices, then it isn't set as neighbor.
	bool setNeighbor(const TrianglePtr &_t);
	// Returns a boolean value indicating if the given triangle is a neighbor
	bool isNeighbor(const TrianglePtr &_t) const;
	// Returns the center (x,y) coordinates of the current triangle
	pair<double, double> getCenter() const;
	// Returns a boolean value indicating if the given vertex is a vertex of this triangle (references comparison, not value)
	bool isVertex(const VertexPtr &_vertex) const;
	// Returns a boolean value indicating if the given vertex is inside the circle defined by this triangle or not
	bool isInCircle(const VertexPtr &_vertex) const;
	// Flips the common side with the given triangle
	vector<pair<TrianglePtr, TrianglePtr>> flipSide(TrianglePtr &_neighbor);
	// Returns the opposite vertex to the given neighbor
	VertexPtr getOppositeVertex(const Triangle *_neighbor) const;
	// Returns the n edge
	Edge getEdge(const int _n) const;
	// Check if the current triangle contains the given edge
	bool hasEdge(const Edge &_edge) const;

	// Returns the triangle's id
	inline long getId() const
	{
		return id;
	}

	// Returns the desired neighbor
	inline TrianglePtr getNeighbor(const int index) const
	{
		return neighbors[index % 3];
	}

	// Returns the desired vertex
	inline VertexPtr getVertex(const int index) const
	{
		return vertices[index % 3];
	}

	// Returns the array of vertices of this triangle
	inline vector<VertexPtr> getVertices() const
	{
		return vertices;
	}

private:
	unsigned long id;
	vector<VertexPtr> vertices;
	vector<TrianglePtr> neighbors;

	// Adds the vertices in CCW
	void addInCCW(const VertexPtr &_vertex1, const VertexPtr &_vertex2, const VertexPtr &_vertex3);
	// Id to make easier the identification of each triangle
	unsigned long getNextId() const;
	// Returns the index of the given neighbor
	int getNeighborIndex(const Triangle *_neighbor) const;
	// Performs the update task over the current triangle
	pair<TrianglePtr, TrianglePtr> updateNeighbor(const vector<pair<TrianglePtr, Edge>> &_potentialNeighbors, const TrianglePtr &_instancePtr);
};
