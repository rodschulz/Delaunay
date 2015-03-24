/**
 * Author: rodrigo
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include <exception>
#include <fstream>
#include "Helper.h"
#include "Vertex.h"
#include "Triangle.h"

using namespace std;

void printTriangulation(const vector<Triangle> &_triangulation)
{
	static int counter = 0;

	string str = "triangulacion";
	str += counter++;
	str += ".txt";

	ofstream stream;
	stream.open(str.c_str(), std::ofstream::out);

	for (Triangle t : _triangulation)
		stream << t;

	stream.close();
}

int getRandomInt(int max)
{
	return (rand() % (int) (max + 1));
}

vector<Vertex> generateVerticesList()
{
	vector<Vertex> list = vector<Vertex>();

	list.push_back(Vertex(10, 2));
	list.push_back(Vertex(9, 6));
	list.push_back(Vertex(4, 7));
	list.push_back(Vertex(6, 3));
	list.push_back(Vertex(3, 2));
	list.push_back(Vertex(12, 5));
	list.push_back(Vertex(14, 1));

	return list;
}

queue<Vertex *> generatePendingQueue(const vector<Vertex> &_vertexList)
{
	vector<Vertex *> aux = vector<Vertex *>();
	queue<Vertex *> pendingVertices = queue<Vertex *>();

	// add all vertices to the auxiliary queue
	for (size_t i = 0; i < _vertexList.size(); i++)
		aux.push_back((Vertex *) &_vertexList[i]);

	while (!aux.empty())
	{
		int index = getRandomInt(aux.size() - 1);
		cout << "index: " << index << " vertex:" << *aux[index] << "\n";

		pendingVertices.push(aux[index]);
		aux.erase(aux.begin() + index);
	}

	return pendingVertices;
}

vector<int> getContainerTriangles(const vector<Triangle> &_triangulation, const Vertex *_target)
{
	vector<int> containers = vector<int>();
	for (size_t i = 0; i < _triangulation.size(); i++)
	{
		if (_triangulation[i].contains(_target))
			containers.push_back(i);

		// stop if already 2 triangles were found
		if (containers.size() == 2)
			break;
	}

	return containers;
}

vector<Triangle *> addNewTriangles(const vector<int> &_containerTriangles, vector<Triangle> &_triangulation, const Vertex *_vertex)
{
	if (_containerTriangles.size() == 1)
	{
		/**
		 * This is the case when the point added is inside a triangle
		 */

		// Get data
		Triangle container = _triangulation[_containerTriangles[0]];
		vector<Vertex *> vertices = container.getVertices();

		// Add new triangles
		int firstAdded = _triangulation.size();
		for (int k = 0; k < 3; k++)
		{
			// Create new triangle
			_triangulation.push_back(Triangle(vertices[k], vertices[(k + 1) % 3], _vertex));

			// Set neighbor from the neighbor of the splitted triangle
			if (!_triangulation.back().setNeighbor(container.getNeighbor(k)))
				cout << "ERROR: setting a neighbor no-neighbor\n";
		}

		// Set pending neighbors for new triangles
		int j = firstAdded;
		_triangulation[j].setNeighbor(&_triangulation[j + 1]);
		_triangulation[j].setNeighbor(&_triangulation[j + 2]);

		_triangulation[j + 1].setNeighbor(&_triangulation[j - 1]);
		_triangulation[j + 1].setNeighbor(&_triangulation[j + 1]);

		_triangulation[j + 2].setNeighbor(&_triangulation[j - 1]);
		_triangulation[j + 2].setNeighbor(&_triangulation[j - 2]);

		// Update neighbors for already existing triangles
		for (size_t t = firstAdded; t < _triangulation.size(); t++)
		{
			_triangulation[t].getNeighbor(0)->setNeighbor(&_triangulation[t]);
			_triangulation[t].getNeighbor(1)->setNeighbor(&_triangulation[t]);
			_triangulation[t].getNeighbor(2)->setNeighbor(&_triangulation[t]);
		}

		// Delete replaced triangle
		_triangulation.erase(_triangulation.begin() + _containerTriangles[0]);
	}
	else
	{
		// This is the case when the point added is over an existing edge
		string str = "collinear points!!\n";
		throw exception();
	}

	printTriangulation(_triangulation);

	return vector<Triangle *>();
}

int main(void)
{
	std::cout << "Start!\n";

	// Generate lists with data
	vector<Vertex> vertexList = generateVerticesList();
	vector<Triangle> triangulation = vector<Triangle>();

	// Set initial triangulation surrounding all the points
	Vertex v1 = Vertex();
	Vertex v2 = Vertex(20, 0);
	Vertex v3 = Vertex(5, 12);
	Triangle t0 = Triangle(&v1, &v3, &v2);
	triangulation.push_back(t0);

	// Begin Delaunay's triangulation process
	queue<Vertex *> pending = generatePendingQueue(vertexList);
	while (!pending.empty())
	{
		Vertex *nextVertex = pending.front();

		// get the triangles surrounding the current point
		vector<int> containers = getContainerTriangles(triangulation, nextVertex);
		vector<Triangle *> newTriangles = addNewTriangles(containers, triangulation, nextVertex);
//		legalizeNewTriangles(newTriangles);

		pending.pop();
	}

	return EXIT_SUCCESS;
}
