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

void Printer::drawTriangle(Mat &_image, const TrianglePtr &_triangle)
{
	Point p1 = convert(*_triangle->getVertex(0));
	Point p2 = convert(*_triangle->getVertex(1));
	Point p3 = convert(*_triangle->getVertex(2));

	line(_image, p1, p2, triangleColor);
	line(_image, p2, p3, triangleColor);
	line(_image, p3, p1, triangleColor);
}

void Printer::drawPoint(Mat &_image, const VertexPtr &_vertex)
{
	Point p = convert(*_vertex);
	circle(_image, p, 1, vertexColor);
}

void Printer::printTriangulationAndVertices(const vector<TrianglePtr> &_triangulation, const vector<
                VertexPtr> &_vertices, const string _outputName)
{
	Mat image = generateBaseImage();

	// Draw triangles
	for (TrianglePtr t : _triangulation)
		drawTriangle(image, t);

	// Draw points
	for (VertexPtr v : _vertices)
		drawPoint(image, v);

	imwrite(OUTPUT_FOLDER + _outputName, image);
}

void Printer::printTriangulation(const vector<TrianglePtr> &_triangulation, const string _outputName)
{
	Mat image = generateBaseImage();

//putText(image, "Hello world", cvPoint(320, 200), CV_FONT_HERSHEY_SIMPLEX, 1, cvScalar(255, 0, 0));

	for (TrianglePtr t : _triangulation)
		drawTriangle(image, t);

	imwrite(OUTPUT_FOLDER + _outputName, image);
}

Mat Printer::generateBaseImage()
{
	Mat image(HEIGHT, WIDTH, CV_8UC3, backgroundColor);

	line(image, Point(0, VERTICAL_OFSET), Point(WIDTH, VERTICAL_OFSET), axesColor);
	line(image, Point(HORIZONTAL_OFFSET, 0), Point(HORIZONTAL_OFFSET, HEIGHT), axesColor);

	return image;
}
