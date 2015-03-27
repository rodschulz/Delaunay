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

vector<TrianglePtr> getContainerTriangles(const vector<TrianglePtr> &_triangulation, const VertexPtr &_target)
{
	vector<TrianglePtr> containers = vector<TrianglePtr>();
	for (size_t i = 0; i < _triangulation.size(); i++)
	{
		if (_triangulation[i]->contains(_target))
			containers.push_back(_triangulation[i]);

		// stop if already 2 triangles were found
		if (containers.size() == 2)
			break;
	}

	return containers;
}

vector<Triangle *> addNewTriangles(const vector<int> &_containerTriangles, vector<Triangle> &_triangulation, const Vertex *_vertex)
{
	/**
	 if (_containerTriangles.size() == 1)
	 {
	 **
	 * This is the case when the point added is inside a triangle
	 *

	 // Get data
	 Triangle container = _triangulation[_containerTriangles[0]];
	 vector<VertexPtr> vertices = container.getVertices();

	 // Add new triangles
	 int firstAdded = _triangulation.size();
	 for (int k = 0; k < 3; k++)
	 {
	 // Create new triangle
	 _triangulation.push_back(Triangle(vertices[k], vertices[(k + 1) % 3], _vertex));

	 // If neighbor exists, then set neighbor from the neighbor of the splitted triangle
	 if (container.getNeighbor(k) != NULL)
	 {
	 if (!_triangulation.back().setNeighbor(container.getNeighbor(k)))
	 cout << "ERROR: setting a neighbor no-neighbor\n";
	 }
	 }

	 // Set pending neighbors for new triangles
	 int j = firstAdded;
	 _triangulation[j].setNeighbor(&_triangulation[j + 1]);
	 _triangulation[j].setNeighbor(&_triangulation[j + 2]);

	 _triangulation[j + 1].setNeighbor(&_triangulation[j]);
	 _triangulation[j + 1].setNeighbor(&_triangulation[j + 2]);

	 _triangulation[j + 2].setNeighbor(&_triangulation[j]);
	 _triangulation[j + 2].setNeighbor(&_triangulation[j + 1]);

	 // Update neighbors for already existing triangles
	 for (size_t t = firstAdded; t < _triangulation.size(); t++)
	 {
	 if (_triangulation[t].getNeighbor(0) != NULL)
	 _triangulation[t].getNeighbor(0)->setNeighbor(&_triangulation[t]);

	 if (_triangulation[t].getNeighbor(1) != NULL)
	 _triangulation[t].getNeighbor(1)->setNeighbor(&_triangulation[t]);

	 if (_triangulation[t].getNeighbor(2) != NULL)
	 _triangulation[t].getNeighbor(2)->setNeighbor(&_triangulation[t]);
	 }

	 // Delete replaced triangle
	 _triangulation.erase(_triangulation.begin() + _containerTriangles[0]);

	 // ACA HAY QUE CAMBIAR TODO ESTO POR USO DE MEMORIA DINAMICA YA QUE AL ELIMINAR O AGREGAR UN TRIANGULO
	 // DEL VECTOR LAS DIRECCIONES DE MEMORIA DE LOS ELEMENTOS PODRIAN CAMBIAR!!!
	 }
	 else
	 {
	 // This is the case when the point added is over an existing edge
	 string str = "collinear points!!\n";
	 //		throw exception();
	 }

	 printTriangulation(_triangulation);
	 */

	return vector<Triangle *>();
}

void legalizeTriangles(vector<Triangle *> &_triangles)
{
	for (size_t i = 0; i < _triangles.size(); i++)
	{
	}
}

int main(void)
{
	std::cout << "Start!\n";

	// Generate lists with data
	vector<VertexPtr> vertexList = Helper::readInput("/media/rodrigo/Documents/UChile/Fall2015/CC72Y/Delaunay/input/input1");
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

		// get the triangles surrounding the current point
		vector<TrianglePtr> containers = getContainerTriangles(triangulation, next);
		Helper::printSelectedTriangles(triangulation, containers, next, "selected_" + to_string(i) + ".png");

//		vector<Triangle *> newTriangles = addNewTriangles(containers, triangulation, nextVertex);
//		legalizeTriangles(newTriangles);
	}

	return EXIT_SUCCESS;
}
