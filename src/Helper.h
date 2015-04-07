/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include "Vertex.h"
#include "Triangle.h"
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

#define COMPARISON_EPSILON	1E-10

class Helper
{
public:
	~Helper();

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

	/**
	 * Generator used to shuffle input data
	 */
	static int shuffleGenerator(const int _i);

	/**
	 * Prints the given triangulation
	 */
	static void printTriangulation(const vector<TrianglePtr> &_triangulation, const string &_outputName);
	/**
	 * Prints the triangulation along with the selected triangles
	 */
	static void printSelectedTriangles(const vector<TrianglePtr> &_triangulation, const vector<TrianglePtr> &_selected, const VertexPtr &_vertex, const string &_outputName);
	/**
	 * Prints triangulation and vertices
	 */
	static void printAll(const vector<TrianglePtr> &_triangulation, const vector<VertexPtr> &_vertices, const string &_outputName);
	/**
	 * Prints a series of images displaying the neighbors associated to each triangle
	 */
	static void printNeighbors(const vector<TrianglePtr> &_triangulation, const string &_outputName, const string &_extension);
	/**
	 * Prints the given triangle along with given vector of vertices
	 */
	static void printTriangle(const TrianglePtr &_triangle, const vector<VertexPtr> &_vertices, const string &_outputName);

private:
	Helper();
	Helper(const Helper &_other);
	void operator=(const Helper &_other);
};
