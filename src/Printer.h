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
	static Mat generateBaseImage();
	static void printVertices(Mat &_image, const vector<VertexPtr> &_vertices);
	static void printTriangulation(Mat &_image, const vector<TrianglePtr> &_triangulation);
	static void printSelectedTriangles(Mat &_image, const vector<TrianglePtr> &_selected);
	static void saveImage(const string &_outputName, const Mat &_image);

private:
	Printer();
	~Printer();

	static Point convert(const float _x, const float _y);
	static Point convert(const Vertex &_vertex);
	static void drawTriangle(Mat &_image, const TrianglePtr &_triangle, const Scalar &_color);
	static void drawPoint(Mat &_image, const VertexPtr &_vertex);
};
