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

vector<TrianglePtr> getContainerTriangles(const map<TrianglePtr, bool> &_triangulation, const VertexPtr &_target)
{
	vector<TrianglePtr> triangles = vector<TrianglePtr>();
	for (pair<TrianglePtr, bool> pair : _triangulation)
	{
		if (pair.first->contains(_target))
			triangles.push_back(pair.first);

		// stop if already 2 triangles were found
		if (triangles.size() == 2)
			break;
	}

	return triangles;
}

vector<TrianglePtr> jumpAndWalk(const map<TrianglePtr, bool> &_triangulation, const VertexPtr &_target)
{
	vector<TrianglePtr> triangles = vector<TrianglePtr>();

	bool storeWalk = Config::getWalkNumber() == (int) _triangulation.size();
	vector<pair<TrianglePtr, Edge>> walk = vector<pair<TrianglePtr, Edge>>();

	// Get the starting triangle randomly
	int randomIndex = Helper::getRandomNumber(0, _triangulation.size() - 1);
	map<TrianglePtr, bool>::const_iterator it = _triangulation.begin();
	std::advance(it, randomIndex);
	TrianglePtr t = it->first;

	// Iterate searching
	while (!t->contains(_target))
	{
		int index = Helper::getRandomNumber(0, 2);
		for (int k = 0; k < 3; k++)
		{
			if (Helper::getOrientation(t->getVertex((index + k) % 3).get(), t->getVertex((index + k + 1) % 3).get(), _target.get()) < 0)
			{
				if (storeWalk)
					walk.push_back(make_pair(t, t->getEdge((index + k) % 3)));
				t = t->getNeighbor((index + k) % 3);
				break;
			}
		}
	}

	triangles.push_back(t);

	for (int k = 0; k < 3; k++)
	{
		TrianglePtr n = t->getNeighbor(k);
		if (n == NULL)
			continue;

		if (n->contains(_target))
		{
			triangles.push_back(n);
			break;
		}
	}

	if (storeWalk)
	{
		Helper::printWalk(walk, _triangulation, _target, "Walk_" + to_string(Config::getWalkNumber()) + ".png");
	}

	return triangles;
}

stack<pair<TrianglePtr, TrianglePtr>> addNewTriangles(const vector<TrianglePtr> &_containerTriangles, map<TrianglePtr, bool> &_triangulation, const VertexPtr &_vertex)
{
	stack<pair<TrianglePtr, TrianglePtr>> output = stack<pair<TrianglePtr, TrianglePtr>>();

	if (_containerTriangles.size() == 1)
	{
		/**
		 * This is the case when the point added is inside a triangle
		 */

		// Get data
		TrianglePtr container = _containerTriangles[0];
		vector<VertexPtr> vertices = container->getVertices();

		if (Config::getDebugLevel() >= MEDIUM)
			cout << "Container > " << *container << "\n";

		// Add new triangles
		vector<TrianglePtr> addedTriangles;
		for (int k = 0; k < 3; k++)
		{
			// Create new triangle and add it to the triangulation
			TrianglePtr newTriangle(new Triangle(vertices[k], vertices[(k + 1) % 3], _vertex));
			_triangulation[newTriangle] = true;

			// Save a reference for later
			addedTriangles.push_back(newTriangle);

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
		for (int k = 0; k < 3; k++)
		{
			if (!addedTriangles[k]->setNeighbor(addedTriangles[(k + 1) % 3]))
				cout << "ERROR: setting a neighbor no-neighbor\n";
			if (!addedTriangles[k]->setNeighbor(addedTriangles[(k + 2) % 3]))
				cout << "ERROR: setting a neighbor no-neighbor\n";
		}

		// Delete replaced triangle
		_triangulation.erase(_containerTriangles[0]);
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
		vector<TrianglePtr> addedTriangles;
		for (size_t i = 0; i < _containerTriangles.size(); i++)
		{
			TrianglePtr container = _containerTriangles[i];
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
					_triangulation[newTriangle] = true;

					// Save a reference for later
					addedTriangles.push_back(newTriangle);

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
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
					continue;

				if (addedTriangles[i]->setNeighbor(addedTriangles[j]) && Config::getDebugLevel() >= MEDIUM)
					cout << "Neighbor added to " << addedTriangles[i]->getId() << " > " << addedTriangles[j]->getId() << "\n";
			}
		}

		// Delete replaced triangles
		_triangulation.erase(_containerTriangles[0]);
		_triangulation.erase(_containerTriangles[1]);
	}

	return output;
}

void legalizeTriangles(stack<pair<TrianglePtr, TrianglePtr>> &_triangles, const map<TrianglePtr, bool> &_triangulation)
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

void removeSuperTriangle(map<TrianglePtr, bool> &_triangulation, const TrianglePtr &_t0)
{
	map<TrianglePtr, bool> eraseMap;

	// Build a map with references to the triangles using each vertex
	vector<VertexPtr> vertex = _t0->getVertices();
	for (pair<TrianglePtr, bool> pair : _triangulation)
	{
		for (int k = 0; k < 3; k++)
		{
			if (pair.first->isVertex(vertex[k]))
				eraseMap[pair.first] = true;
		}
	}

	// Remove triangles
	for (pair<TrianglePtr, bool> pair : eraseMap)
		_triangulation.erase(pair.first);
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

	double width = (abs(maxX) > abs(minX) ? abs(maxX) : abs(minX)) * 2.1;
	double height = (abs(maxY) > abs(minY) ? abs(maxY) : abs(minY)) * 2.1;

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

		if (_nargs >= 9 && strcmp(_vargs[7], "-s") == 0)
		{
			cout << "Saving randomly generated input to " << _vargs[8] << "\n";
			Helper::writePoints(vertexList, _vargs[8]);
		}
	}
	else
		Helper::readInput(_vargs[1], vertexList);

	// Initial triangle for triangulation
	TrianglePtr t0 = Helper::calculateSurroundingTriangle(vertexList);
	map<TrianglePtr, bool> triangulation = map<TrianglePtr, bool>();
	triangulation[t0] = true;

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

		// Get the triangles surrounding the current point
		vector<TrianglePtr> containers;
		if (Config::getWalkingMethod() == JUMP_AND_WALK)
			containers = jumpAndWalk(triangulation, next);
		else
			containers = getContainerTriangles(triangulation, next);

		if (Config::getDebugLevel() >= MEDIUM)
			Helper::printSelectedTriangles(triangulation, containers, next, "selectedTriangle" + to_string(i) + ".png");

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

	// Print circumcircles
	if (Config::drawCircles())
		Helper::printCircumcircles(triangulation, vertexList, "circumcircles.png");

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	cout << "Triangulation done in " << duration << "[us] using method " << Config::getWalkingMethod() << "\n";

	return EXIT_SUCCESS;
}
