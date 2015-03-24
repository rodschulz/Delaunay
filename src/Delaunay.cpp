/**
 * Author: rodrigo
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include "Vertex.h"
#include "Triangle.h"

using namespace std;

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
	vector<Triangle *> newTriangles = vector<Triangle *>();
	for (size_t i = 0; i < _containerTriangles.size(); i++)
	{
//		vector<Triangle> triangles = _triangulation[i].split(_vertex);
	}

	return newTriangles;
}

int main(void)
{
	std::cout << "Start!\n";

	// generate lists with data
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
