/**
 * Author: rodrigo
 * 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Vertex.h"
#include "Triangle.h"

using namespace std;

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

int main(void)
{
	std::cout << "Beginning Delaunay triangulation\n";

	// generate lists with data
	vector<Vertex> vertexList = generateVerticesList();
	vector<Triangle> triangleList = vector<Triangle>();

	// Set initial triangulation surrounding all the points

	return EXIT_SUCCESS;
}
