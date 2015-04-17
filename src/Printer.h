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
	// Generates a base image
	static Mat generateBaseImage();
	// Prints the given vertices in the given image
	static void printVertices(Mat &_image, const vector<VertexPtr> &_vertices);
	// Prints the given triangles in the given image
	static void printTriangulation(Mat &_image, const vector<TrianglePtr> &_triangulation);
	static void printTriangulation(Mat &_image, const map<TrianglePtr, bool> &_triangulation);
	// Prints the given triangles in the given image
	static void printSelectedTriangles(Mat &_image, const vector<TrianglePtr> &_selected);
	// Prints the neighbors relations of the given triangle
	static void printNeighbors(Mat &_image, const TrianglePtr &_triangle);
	// Saves the given image to disk
	static void saveImage(const string &_outputName, const Mat &_image);
	// Returns an instance of the current singleton printer class
	static Printer *getInstance();
	// Calculates the conversion rate according to the given dimensions
	void calculateConversionRate(const double _width, const double _height);

	// Draws the given triangle in the given image
	static void drawTriangle(Mat &_image, const TrianglePtr &_triangle, const Scalar &_color);
	// Draws the given point in the given image
	static void drawPoint(Mat &_image, const VertexPtr &_vertex, const Scalar &_color);
	// Draws the given edge
	static void drawEdge(Mat &_image, const Edge &_edge, const Scalar &_color);
	// Draws the given circle
	static void drawCircle(Mat &_image, const double _Xc, const double _Yc, const double _r, const Scalar &_color);

private:
	Printer();
	~Printer();

	double conversionRate;
	int step;

	// Converts the given value to its pixel equivalent
	int toPixel(const double _value) const;
	// Converts a point from xy coordinates to pixels
	static Point convert(const double _x, const double _y);
	// Converts a vertex from xy coordinates to pixels
	static Point convert(const Vertex &_vertex);
};
