/**
 * Author: rodrigo
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <exception>
#include <fstream>
#include "Helper.h"
#include "Config.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Printer.h"

using namespace std;

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

		if (Config::getDebugLevel() >= MEDIUM)
			cout << "Container > " << *container << "\n";

		// Add new triangles
		int firstAdded = _triangulation.size();
		for (int k = 0; k < 3; k++)
		{
			// Create new triangle and add it to the triangulation
			TrianglePtr newTriangle(new Triangle(vertices[k], vertices[(k + 1) % 3], _vertex));
			_triangulation.push_back(newTriangle);

			// Also add the new triangle to the output vector
			output.push(make_pair(newTriangle, TrianglePtr(NULL)));

			if (Config::getDebugLevel() >= LOW)
				cout << "New triangle > " << *newTriangle << "\n";

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

				if (Config::getDebugLevel() >= MEDIUM)
					cout << "Neighbor added > " << output.top().second->getId() << "\n";
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

		if (Config::getDebugLevel() >= LOW)
			cout << "** Collinear point!\n";

		// First create the 4 new triangles
		int firstAdded = _triangulation.size();
		for (size_t i = 0; i < _containerTriangles.first.size(); i++)
		{
			TrianglePtr container = _containerTriangles.first[i];
			vector<VertexPtr> vertices = container->getVertices();

			if (Config::getDebugLevel() >= MEDIUM)
				cout << "Container > " << *container << "\n";

			// Add new triangles
			for (int k = 0; k < 3; k++)
			{
				if (!Helper::areCollinear(vertices[k].get(), vertices[(k + 1) % 3].get(), _vertex.get()))
				{
					// Create new triangle and add it to the triangulation
					TrianglePtr newTriangle(new Triangle(vertices[k], vertices[(k + 1) % 3], _vertex));
					_triangulation.push_back(newTriangle);

					// Also add the new triangle to the output vector
					output.push(make_pair(newTriangle, TrianglePtr(NULL)));

					if (Config::getDebugLevel() >= LOW)
						cout << "New triangle > " << *newTriangle << "\n";

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

						if (Config::getDebugLevel() >= MEDIUM)
							cout << "Neighbor added > " << output.top().second->getId() << "\n";
					}
				}
			}
		}

		// Set pointers going to the neighbors and coming from them
		for (size_t i = firstAdded; i < _triangulation.size(); i++)
		{
			for (size_t j = firstAdded; j < _triangulation.size(); j++)
			{
				if (i == j)
					continue;

				if (_triangulation[i]->setNeighbor(_triangulation[j]) && Config::getDebugLevel() >= MEDIUM)
					cout << "Neighbor added to " << _triangulation[i]->getId() << " > " << _triangulation[j]->getId() << "\n";
			}
		}

		// Delete replaced triangles
		_triangulation.erase(_triangulation.begin() + _containerTriangles.second[0]);
		int offset = _containerTriangles.second[0] < _containerTriangles.second[1] ? 1 : 0;
		_triangulation.erase(_triangulation.begin() + _containerTriangles.second[1] - offset);
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

		cout << "Legalizing edge " << addedTriangle->getId() << "-" << (neighbor != NULL ? to_string(neighbor->getId()) : "NULL") << "\n";

		if (neighbor.get() != NULL)
		{
			if (!addedTriangle->isNeighbor(neighbor))
			{
				cout << "No neighbors edge, skipping\n";
				continue;
			}

			// Test if it's inside the circumcircle
			if (addedTriangle->isInCircle(neighbor->getOppositeVertex(addedTriangle.get())))
			{
				cout << "Flipping edge " << addedTriangle->getId() << "-" << neighbor->getId() << "\n";

				// If it's inside, then flip the common side
				vector<pair<TrianglePtr, TrianglePtr>> flipped = addedTriangle->flipSide(neighbor);
				for (size_t i = 0; i < flipped.size(); i++)
				{
					cout << "Queueing check " << flipped[i].first->getId() << "-" << (flipped[i].second != NULL ? to_string(flipped[i].second->getId()) : "NULL") << "\n";
					_triangles.push(flipped[i]);
				}

				if (Config::getDebugLevel() >= HIGH)
					Helper::printNeighbors(_triangulation, "legalizing_" + to_string(legalizationCallCount) + "_" + to_string(loopCount++), ".png");
			}
		}
	}

	legalizationCallCount++;
}

void removeSuperTriangle(vector<TrianglePtr> &_triangulation, const TrianglePtr &_t0)
{
	// Vector to hold the indices of the triangles using a vertex of T0
	map<size_t, bool> indices;

	// Build a map with references to the triangles using each vertex
	vector<VertexPtr> vertex = _t0->getVertices();
	for (size_t i = 0; i < _triangulation.size(); i++)
	{
		for (int k = 0; k < 3; k++)
		{
			if (_triangulation[i]->isVertex(vertex[k]))
				indices[i] = true;
		}
	}

	// Remove triangles
	vector<TrianglePtr> cleanTriangulation;
	cleanTriangulation.reserve(_triangulation.size());
	for (size_t i = 0; i < _triangulation.size(); i++)
	{
		if (indices.find(i) == indices.end())
			cleanTriangulation.push_back(_triangulation[i]);
	}

	_triangulation = cleanTriangulation;
}

// Main method
int main(int _nargs, char ** _vargs)
{
	// Clean output folder
	int result = system("exec rm -r ./output/*");
	cout << result << "\n";

	if (_nargs < 2)
		cout << "Not enough arguments given!";
	else
		cout << "Start!\n";

	// Load configuration
	Config::load("./config/config");

	// Generate lists with data
	vector<VertexPtr> vertexList = vector<VertexPtr>();
	if (Helper::isCommand(_vargs[1]))
	{
		vertexList.reserve(stoi(_vargs[2]));
		Helper::generateRandomSet(stoi(_vargs[2]), stoi(_vargs[3]), stoi(_vargs[4]), stoi(_vargs[5]), stoi(_vargs[6]), vertexList);
	}
	else
		Helper::readInput(_vargs[1], vertexList);

	vector<TrianglePtr> triangulation = vector<TrianglePtr>();

	// Initial triangle for triangulation
	TrianglePtr t0 = Helper::calculateSurroundingTriangle(vertexList);
	triangulation.push_back(t0);

	// Print initial state
	Helper::printAll(triangulation, vertexList, "initial_state.png");

	// Shuffle vertices before start adding
	if (Config::randomizeInput())
	{
		std::random_shuffle(vertexList.begin(), vertexList.end(), Helper::shuffleGenerator);
	}

	// Begin Delaunay's triangulation process
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		VertexPtr next = vertexList[i];
		cout << "+++++ Adding vertex " << to_string(i) << ": " << *next << "\n";

		// Get the triangles surrounding the current point
		pair<vector<TrianglePtr>, vector<int>> containers = getContainerTriangles(triangulation, next);

		// Add the new triangles according to the new vertex
		stack<pair<TrianglePtr, TrianglePtr>> newTriangles = addNewTriangles(containers, triangulation, next);
		if (Config::getDebugLevel() >= LOW)
			Helper::printTriangulation(triangulation, "addedPoint_" + to_string(i) + ".png");

		if (Config::getDebugLevel() >= MEDIUM)
			Helper::printNeighbors(triangulation, "triangulation" + to_string(i), ".png");

		// Legalize the new added triangles
		legalizeTriangles(newTriangles, triangulation);
		if (Config::getDebugLevel() >= LOW)
			Helper::printTriangulation(triangulation, "legalizedPoint_" + to_string(i) + ".png");
	}

	// Print final state
	Helper::printAll(triangulation, vertexList, "final_state.png");

	// Remove super-triangle
	removeSuperTriangle(triangulation, t0);

	Helper::printAll(triangulation, vertexList, "final_triangulation.png");

	cout << "Execution finished!\n";

	return EXIT_SUCCESS;
}
