/**
 * Author: rodrigo
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stack>
#include <exception>
#include <fstream>
#include "Helper.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Printer.h"

using namespace std;

typedef enum DebugLevel
{
	DEBUG_LOW, DEBUG_MEDIUM, DEBUG_HIGH
} DebugLevel;

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

stack<pair<TrianglePtr, TrianglePtr>> addNewTriangles(const pair<vector<TrianglePtr>, vector<int>> &_containerTriangles, vector<TrianglePtr> &_triangulation, const VertexPtr &_vertex)
{
	stack<pair<TrianglePtr, TrianglePtr>> output = stack<pair<TrianglePtr, TrianglePtr>>();

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
			output.push(make_pair(newTriangle, TrianglePtr(NULL)));

			// If the container triangle has a neighbor in this side, then set it as a neighbor of the new triangle
			if (container->getNeighbor(k) != NULL)
			{
				// Add the neighbor to the new
				if (!newTriangle->setNeighbor(container->getNeighbor(k)))
					cout << "ERROR: setting a neighbor no-neighbor\n";
				// Add the new to the neighbor
				if (!container->getNeighbor(k)->setNeighbor(newTriangle))
					cout << "ERROR: setting a neighbor no-neighbor\n";

				// Update the neighbor in the output vector
				output.top().second = container->getNeighbor(k);
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

void legalizeTriangles(stack<pair<TrianglePtr, TrianglePtr>> &_triangles, const vector<TrianglePtr> &_triangulation)
{
	static int legalizationCallCount = 0;

	int loopCount = 0;
	while (!_triangles.empty())
	{
		TrianglePtr addedTriangle = _triangles.top().first;
		TrianglePtr neighbor = _triangles.top().second;
		_triangles.pop();

		if (neighbor != NULL)
			cout << "legalizing edge " << addedTriangle->getId() << "-" << neighbor->getId() << "\n";

		// Test if it's inside the circumcircle
		if (neighbor.get() != NULL && addedTriangle->isInCircle(neighbor->getOppositeVertex(addedTriangle.get())))
		{
			cout << "flipping side " << addedTriangle->getId() << "-" << neighbor->getId() << "\n";

			// If it's inside, then flip the common side
			vector<pair<TrianglePtr, TrianglePtr>> flipped = addedTriangle->flipSide(neighbor);
			for (size_t i = 0; i < flipped.size(); i++)
				_triangles.push(flipped[i]);

			Helper::printNeightbors(_triangulation, "legalizing_" + to_string(legalizationCallCount) + "_" + to_string(loopCount++), ".png");
		}
	}

	legalizationCallCount++;
}

// Main method
int main(int _nargs, char ** _vargs)
{
	// Clean output folder
	system("exec rm -r ./output/*");

	if (_nargs < 2)
		cout << "Not enough arguments given!";
	else
		cout << "Start!\n";

	//DebugLevel level = (DebugLevel) atoi(_vargs[2]);

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
		stack<pair<TrianglePtr, TrianglePtr>> newTriangles = addNewTriangles(containers, triangulation, next);
		Helper::printTriangulation(triangulation, "addedPoint_" + to_string(i) + ".png");

		// Legalize the new added triangles
		legalizeTriangles(newTriangles, triangulation);
		Helper::printTriangulation(triangulation, "legalizedPoint_" + to_string(i) + ".png");
	}

	// Print final state
	Helper::printAll(triangulation, vertexList, "final_state.png");

	// Remove super-triangle
	//removeSuperTriangle(triagulation, t0);

	Helper::printAll(triangulation, vertexList, "final_triangulation.png");

	cout << "Execution finished!\n";

	return EXIT_SUCCESS;
}
