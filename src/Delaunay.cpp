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
#include <fstream>
#include <limits>
#include <chrono>
#include "Helper.h"
#include "Config.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Printer.h"

using namespace std;
using namespace std::chrono;

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

pair<vector<TrianglePtr>, vector<int>> jumpAndWalk(const vector<TrianglePtr> &_triangulation, const VertexPtr &_target)
{
	vector<TrianglePtr> triangles = vector<TrianglePtr>();
	vector<int> indices = vector<int>();
	map<TrianglePtr, int> indexMap;

	// Build a map with the triangles indices
	for (size_t i = 0; i < _triangulation.size(); i++)
		indexMap[_triangulation[i]] = i;

	// Get the starting triangle randomly
	TrianglePtr t = _triangulation[Helper::getRandomNumber(0, _triangulation.size() - 1)];
	while (!t->contains(_target))
	{
		int index = Helper::getRandomNumber(0, 2);
		for (int k = 0; k < 3; k++)
		{
			if (Helper::getOrientation(t->getVertex((index + k) % 3).get(), t->getVertex((index + k + 1) % 3).get(), _target.get()) < 0)
			{
				t = t->getNeighbor((index + k) % 3);
				break;
			}
		}
	}

	triangles.push_back(t);
	indices.push_back(indexMap[t]);

	for (int k = 0; k < 3; k++)
	{
		TrianglePtr n = t->getNeighbor(k);
		if (n == NULL)
			continue;

		if (n->contains(_target))
		{
			triangles.push_back(n);
			indices.push_back(indexMap[n]);
			break;
		}
	}

	return make_pair(triangles, indices);
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

void preparePrinter(const TrianglePtr &_t0)
{
	double minX = numeric_limits<double>::max();
	double maxX = -numeric_limits<double>::max();
	double minY = numeric_limits<double>::max();
	double maxY = -numeric_limits<double>::max();

	for (int i = 0; i < 3; i++)
	{
		VertexPtr v = _t0->getVertex(i);
		maxX = maxX < v->getX() ? v->getX() : maxX;
		minX = minX > v->getX() ? v->getX() : minX;
		maxY = maxY < v->getY() ? v->getY() : maxY;
		minY = minY > v->getY() ? v->getY() : minY;
	}

	double width = (abs(maxX) > abs(minX) ? abs(maxX) : abs(minX)) * 2.0;
	double height = (abs(maxY) > abs(minY) ? abs(maxY) : abs(minY)) * 2.0;

	Printer::getInstance()->calculateConversionRate(width, height);
}

// Main method
int main(int _nargs, char ** _vargs)
{
	if (_nargs < 2)
	{
		cout << "Not enough arguments given!\nUsage:\n\tDelaunay [options] [<input_file>]\n\n";
		cout << "Options:\n\t-r\trandom input generated using 5 parameters: number of points, ";
		cout << "minimum X,\n\t\tmaximum X, minimum Y, maximum Y.\n\n";

		return EXIT_FAILURE;
	}
	else
		cout << "Start!\n";

	// Clean output folder
	cout << "Cleaning output folder (code: " << system("exec rm -rf ./output/*") << ")\n";

	// Load configuration
	Config::load("./config/config");

	// Generate lists with data
	vector<VertexPtr> vertexList = vector<VertexPtr>();
	if (Helper::isCommand(_vargs[1]))
	{
		vertexList.reserve(stoi(_vargs[2]));
		Helper::generateRandomSet(stoi(_vargs[2]), stoi(_vargs[3]), stoi(_vargs[4]), stoi(_vargs[5]), stoi(_vargs[6]), vertexList);

		Helper::writePoints(vertexList, "./input/input7");
	}
	else
		Helper::readInput(_vargs[1], vertexList);

	// Initial triangle for triangulation
	TrianglePtr t0 = Helper::calculateSurroundingTriangle(vertexList);
	vector<TrianglePtr> triangulation = vector<TrianglePtr>();
	triangulation.push_back(t0);

	// Prepare printer limits
	preparePrinter(t0);

	// Print initial state
	Helper::printAll(triangulation, vertexList, "initial_state.png");

	// Shuffle vertices before start adding
	if (Config::randomizeInput())
	{
		std::random_shuffle(vertexList.begin(), vertexList.end(), Helper::shuffleGenerator);
	}

	// Begin Delaunay's triangulation process
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		VertexPtr next = vertexList[i];
		cout << "+++++ Adding vertex " << to_string(i) << ": " << *next << "\n";


		se podría usar un mapa en vez de un vector para almacenar la triangulación y de esa forma las eliminaciones sería más simples
		no se requeriría el índice y en consecuencia no se requeriría generar el mapa de índices en el jump and walk

		// Get the triangles surrounding the current point
		pair<vector<TrianglePtr>, vector<int>> containers;
		if (Config::getWalkingMethod() == JUMP_AND_WALK)
			containers = jumpAndWalk(triangulation, next);
		else
			containers = getContainerTriangles(triangulation, next);

		if (Config::getDebugLevel() >= MEDIUM)
			Helper::printSelectedTriangles(triangulation, containers.first, next, "selectedTriangle" + to_string(i) + ".png");

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
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	// Print final state
	Helper::printAll(triangulation, vertexList, "final_state.png");
	// Remove super-triangle
	removeSuperTriangle(triangulation, t0);
	// Print result
	Helper::printAll(triangulation, vertexList, "final_triangulation.png");

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	cout << "Triangulation done in " << duration << "[us] using method " << Config::getWalkingMethod() << "\n";

	return EXIT_SUCCESS;
}
