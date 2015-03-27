/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include "Vertex.h"
#include "Triangle.h"
#include <string>
#include <vector>

using namespace std;

#define COMPARISON_EPSILON	1E-10

class Helper
{
public:
	/**
	 * Evaluates the orientation of the given points. If they are CCW, then
	 * returns a value > 0, if they are CW then the value returned is < 0
	 */
	static double getOrientation(const Vertex *_v1, const Vertex *_v2, const Vertex *_v3);

	/**
	 * Evaluates if the three given vertices are collinear
	 */
	static bool areCollinear(const Vertex *_v1, const Vertex *_v2, const Vertex *_v3);

	/**
	 * Reads an input file from the given location
	 */
	static vector<VertexPtr> readInput(const string &_location);

	/**
	 * Calculates a triangle surrounding the whole group of points, so it can be used
	 * as the initial triangulation
	 */
	static TrianglePtr calculateSurroundingTriangle(const vector<VertexPtr> &_vertexList);

private:
	Helper()
	{
	}
	;

	~Helper()
	{
	}
	;
};
