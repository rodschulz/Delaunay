/**
 * Author: rodrigo
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <exception>
#include <fstream>
#include "Helper.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Printer.h"

using namespace std;

enum debugLevel
{
	DEBUG_LOW, DEBUG_MEDIUM, DEBUG_HIGH
};

pair<vector<TrianglePtr>, vector<int>> getContainerTriangles(const vector<TrianglePtr> &_triangulation, const VertexPtr &_target)
{
	vector<TrianglePtr> triangles = vector<TrianglePtr>();
	vector<int> indexes = vector<int>();
	for (size_t i = 0; i < _triangulation.size(); i++)
	{
		if (_triangulation[i]->contains(_target))
		{
			triangles.push_back(_triangulation[i]);
			indexes.push_back(i);
		}

		// stop if already 2 triangles were found
		if (triangles.size() == 2)
			break;
	}

	return make_pair(triangles, indexes);
}

vector<TrianglePtr> addNewTriangles(const pair<vector<TrianglePtr>, vector<int>> &_containerTriangles, vector<TrianglePtr> &_triangulation, const VertexPtr &_vertex)
{
	vector<TrianglePtr> output = vector<TrianglePtr>();

	if (_containerTriangles.first.size() == 1)
	{
		/**
		 * This is the case when the point added is inside a triangle
		 */

		// Get data
		TrianglePtr container = _containerTriangles.first[0];
		vector<VertexPtr> vertices = container->getVertices();

		// Add new triangles
		int firstAdded = _triangulation.size();
		for (int k = 0; k < 3; k++)
		{
			// Create new triangle and add it to the triangulation
			TrianglePtr newTriangle(new Triangle(vertices[k], vertices[(k + 1) % 3], _vertex));
			_triangulation.push_back(newTriangle);
			// Also add the new triangle to the output vector
			output.push_back(newTriangle);

			// If the container triangle has a neighbor in this side, then set it as a neighbor of the new triangle
			if (container->getNeighbor(k) != NULL)
			{
				// Add the neighbor to the new
				if (!newTriangle->setNeighbor(container->getNeighbor(k)))
					cout << "ERROR: setting a neighbor no-neighbor\n";
				// Add the new to the neighbor
				if (!container->getNeighbor(k)->setNeighbor(newTriangle))
					cout << "ERROR: setting a neighbor no-neighbor\n";
			}
		}

		// Set pending neighbors for new triangles
		int j = firstAdded;
		if (!_triangulation[j]->setNeighbor(_triangulation[j + 1]))
			cout << "ERROR: setting a neighbor no-neighbor\n";
		if (!_triangulation[j]->setNeighbor(_triangulation[j + 2]))
			cout << "ERROR: setting a neighbor no-neighbor\n";

		if (!_triangulation[j + 1]->setNeighbor(_triangulation[j]))
			cout << "ERROR: setting a neighbor no-neighbor\n";
		if (!_triangulation[j + 1]->setNeighbor(_triangulation[j + 2]))
			cout << "ERROR: setting a neighbor no-neighbor\n";

		if (!_triangulation[j + 2]->setNeighbor(_triangulation[j]))
			cout << "ERROR: setting a neighbor no-neighbor\n";
		if (!_triangulation[j + 2]->setNeighbor(_triangulation[j + 1]))
			cout << "ERROR: setting a neighbor no-neighbor\n";

		// Delete replaced triangle
		_triangulation.erase(_triangulation.begin() + _containerTriangles.second[0]);
	}
	else
	{
		/**
		 * This is the case when a collinear point is added, that is the point is
		 * placed right in an edge
		 */
		cout << "collinear point!!\n";
	}

	return output;
}

void legalizeTriangles(vector<TrianglePtr> &_triangles)
{
	for (size_t i = 0; i < _triangles.size(); i++)
	{
	}
}

// Main method
int main(int _nargs, char ** _vargs)
{
	// Clean output folder
	system("exec rm -r ./output/*");

	if (_nargs < 1)
		cout << "Not enough arguments given!";
	else
		cout << "Start!\n";

	// Generate lists with data
	vector<VertexPtr> vertexList = Helper::readInput(_vargs[1]);
	vector<TrianglePtr> triangulation = vector<TrianglePtr>();

	// Initial triangle for triangulation
	TrianglePtr t0 = Helper::calculateSurroundingTriangle(vertexList);
	triangulation.push_back(t0);

	// Print initial state
	Helper::printAll(triangulation, vertexList, "initial_state.png");

	// Shuffle vertices before start adding
	//std::random_shuffle(vertexList.begin(), vertexList.end(), Helper::shuffleGenerator);

	// Begin Delaunay's triangulation process
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		VertexPtr next = vertexList[i];
		cout << "Adding vertex " << *next << "\n";

		// Get the triangles surrounding the current point
		pair<vector<TrianglePtr>, vector<int>> containers = getContainerTriangles(triangulation, next);
		Helper::printSelectedTriangles(triangulation, containers.first, next, "selected_" + to_string(i) + ".png");

		// Add the new triangles according to the new vertex
		vector<TrianglePtr> newTriangles = addNewTriangles(containers, triangulation, next);
		Helper::printTriangulation(triangulation, "addedPoint_" + to_string(i) + ".png");

		Helper::printNeightbors(triangulation, "neighbors_" + to_string(i), ".png");

		// Legalize the new added triangles
		legalizeTriangles(newTriangles);
	}

	// Print final state
	Helper::printAll(triangulation, vertexList, "final_state.png");

	return EXIT_SUCCESS;
}
