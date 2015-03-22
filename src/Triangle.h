/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include <vector>
#include "Vertex.h"

using namespace std;

class Triangle
{
public:
	Triangle(const Vertex *_vertex1, const Vertex *_vertex2, const Vertex *_vertex3);
	~Triangle();

private:
	vector<Vertex *> vertices;
};

