/**
 * Author: rodrigo
 * 2015
 */
#include "Printer.h"

#define OUTPUT_FOLDER		"./output/"
#define WIDTH			640
#define HEIGHT			480
#define CONVERSION_RATE		8 // this is X:1 meaning conversion pixels:unit
#define HORIZONTAL_OFFSET	(WIDTH / 2)
#define VERTICAL_OFSET		(HEIGHT / 2)
#define toPixel(x)		((x) * CONVERSION_RATE)

// Color used to draw triangles
static Scalar backgroundColor = Scalar(180, 180, 180);
static Scalar axesColor = Scalar(0, 0, 0);
static Scalar triangleColor = Scalar(200, 0, 0);
static Scalar selectedTriangleColor = Scalar(0, 200, 0);
static Scalar vertexColor = Scalar(0, 0, 255);

Printer::Printer()
{
}

Printer::~Printer()
{
}

Point Printer::convert(const float _x, const float _y)
{
	int x = toPixel(_x) + HORIZONTAL_OFFSET;
	int y = VERTICAL_OFSET - toPixel(_y);
	return Point(x, y);
}

Point Printer::convert(const Vertex &_vertex)
{
	return convert(_vertex.getX(), _vertex.getY());
}

void Printer::drawTriangle(Mat &_image, const TrianglePtr &_triangle, const Scalar &_color)
{
	Point p1 = convert(*_triangle->getVertex(0));
	Point p2 = convert(*_triangle->getVertex(1));
	Point p3 = convert(*_triangle->getVertex(2));

	line(_image, p1, p2, _color);
	line(_image, p2, p3, _color);
	line(_image, p3, p1, _color);
}

void Printer::drawPoint(Mat &_image, const VertexPtr &_vertex)
{
	Point p = convert(*_vertex);
	circle(_image, p, 1, vertexColor);
}

void Printer::printVertices(Mat &_image, const vector<VertexPtr> &_vertices)
{
	for (VertexPtr v : _vertices)
		drawPoint(_image, v);
}

void Printer::printTriangulation(Mat &_image, const vector<TrianglePtr> &_triangulation)
{
	for (TrianglePtr t : _triangulation)
		drawTriangle(_image, t, triangleColor);
}

void Printer::printSelectedTriangles(Mat &_image, const vector<TrianglePtr> &_selected)
{
	for (TrianglePtr t : _selected)
		drawTriangle(_image, t, selectedTriangleColor);
}

void Printer::saveImage(const string &_outputName, const Mat &_image)
{
	imwrite(OUTPUT_FOLDER + _outputName, _image);
}

Mat Printer::generateBaseImage()
{
	Mat image(HEIGHT, WIDTH, CV_8UC3, backgroundColor);

	line(image, Point(0, VERTICAL_OFSET), Point(WIDTH, VERTICAL_OFSET), axesColor);
	line(image, Point(HORIZONTAL_OFFSET, 0), Point(HORIZONTAL_OFFSET, HEIGHT), axesColor);

	return image;
}
