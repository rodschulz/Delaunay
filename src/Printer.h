/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include <vector>
#include <string>
#include "Triangle.h"

#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;
using namespace cv;

class Printer
{
public:
	static void printTriangulationAndVertices(const vector<TrianglePtr> &_triangulation, const vector <VertexPtr> &_vertices, const string _outputName);
	static void printTriangulation(const vector<TrianglePtr> &_triangulation, const string _outputName);

private:
	Printer();
	~Printer();

	static Point convert(const float _x, const float _y);
	static Point convert(const Vertex &_vertex);
	static Mat generateBaseImage();
	static void drawTriangle(Mat &_image, const TrianglePtr &_triangle);
	static void drawPoint(Mat &_image, const VertexPtr &_vertex);
};
