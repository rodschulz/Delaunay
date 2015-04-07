/**
 * Author: rodrigo
 * 2015
 */
#include "Printer.h"

#define OUTPUT_FOLDER		"./output/"
#define WIDTH			1280
#define HEIGHT			960
#define CONVERSION_RATE		8 // this is X:1 meaning conversion pixels:unit
#define STEP			5
#define HORIZONTAL_OFFSET	(WIDTH / 2)
#define VERTICAL_OFFSET		(HEIGHT / 2)
#define toPixel(x)		((x) * CONVERSION_RATE)

// Color used to draw triangles
static Scalar LIGHT_GRAY = Scalar(190, 190, 190);
static Scalar BLACK = Scalar(0, 0, 0);
static Scalar BLUE = Scalar(200, 0, 0);
static Scalar GREEN = Scalar(0, 200, 0);
static Scalar RED = Scalar(0, 0, 255);
static Scalar LIGHT_BLUE = Scalar(200, 200, 0);

Printer::Printer()
{
}

Printer::~Printer()
{
}

Point Printer::convert(const double _x, const double _y)
{
	int x = toPixel(_x) + HORIZONTAL_OFFSET;
	int y = VERTICAL_OFFSET - toPixel(_y);
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

	pair<double, double> center = _triangle->getCenter();
	putText(_image, to_string(_triangle->getId()), convert(center.first, center.second), CV_FONT_HERSHEY_SIMPLEX, 0.3, _color);
}

void Printer::drawPoint(Mat &_image, const VertexPtr &_vertex)
{
	Point p = convert(*_vertex);
	circle(_image, p, 2, RED, -1);
}

void Printer::printVertices(Mat &_image, const vector<VertexPtr> &_vertices)
{
	for (VertexPtr v : _vertices)
		drawPoint(_image, v);
}

void Printer::printTriangulation(Mat &_image, const vector<TrianglePtr> &_triangulation)
{
	for (TrianglePtr t : _triangulation)
		drawTriangle(_image, t, BLUE);
}

void Printer::printSelectedTriangles(Mat &_image, const vector<TrianglePtr> &_selected)
{
	for (TrianglePtr t : _selected)
		drawTriangle(_image, t, GREEN);
}

void Printer::printNeighbors(Mat &_image, const TrianglePtr &_triangle)
{
	vector<Scalar> colors(0);
	colors.push_back(RED);
	colors.push_back(GREEN);
	colors.push_back(LIGHT_BLUE);

	pair<double, double> aux = _triangle->getCenter();
	Point center = convert(aux.first, aux.second);
	vector<VertexPtr> vertices = _triangle->getVertices();

	for (int k = 0; k < 3; k++)
	{
		if (_triangle->getNeighbor(k) != NULL)
		{
			Point p1 = convert(*vertices[k]);
			Point p2 = convert(*vertices[(k + 1) % 3]);
			line(_image, p1, p2, colors[k]);

			pair<double, double> neighborCenter = _triangle->getNeighbor(k)->getCenter();
			line(_image, center, convert(neighborCenter.first, neighborCenter.second), colors[k]);
		}
	}
}

void Printer::saveImage(const string &_outputName, const Mat &_image)
{
	imwrite(OUTPUT_FOLDER + _outputName, _image);
}

Mat Printer::generateBaseImage()
{
	Mat image(HEIGHT, WIDTH, CV_8UC3, LIGHT_GRAY);

	line(image, Point(0, VERTICAL_OFFSET), Point(WIDTH, VERTICAL_OFFSET), BLACK);
	line(image, Point(HORIZONTAL_OFFSET, 0), Point(HORIZONTAL_OFFSET, HEIGHT), BLACK);

	int step = STEP;
	int tickSize = toPixel(0.3);
	int position = step;
	int tickOffsetH = 5;
	int tickOffsetV = 10;

	// X ticks
	int pos = toPixel(position);
	while (pos <= WIDTH)
	{
		line(image, Point(pos + HORIZONTAL_OFFSET, VERTICAL_OFFSET - tickSize), Point(pos + HORIZONTAL_OFFSET, VERTICAL_OFFSET + tickSize), BLACK);
		putText(image, to_string(position), Point(pos + HORIZONTAL_OFFSET - tickOffsetH, VERTICAL_OFFSET + tickSize + tickOffsetV), CV_FONT_HERSHEY_SIMPLEX, 0.3, BLACK);

		line(image, Point(HORIZONTAL_OFFSET - pos, VERTICAL_OFFSET - tickSize), Point(HORIZONTAL_OFFSET - pos, VERTICAL_OFFSET + tickSize), BLACK);
		putText(image, to_string(-position), Point(HORIZONTAL_OFFSET - pos - tickOffsetH - 5, VERTICAL_OFFSET + tickSize + tickOffsetV), CV_FONT_HERSHEY_SIMPLEX, 0.3, BLACK);

		position += step;
		pos = toPixel(position);
	}

	position = step;
	pos = toPixel(position);
	tickOffsetH = 15;
	tickOffsetV = 3;

	// Y ticks
	while (pos <= HEIGHT)
	{
		line(image, Point(HORIZONTAL_OFFSET - tickSize, VERTICAL_OFFSET - pos), Point(HORIZONTAL_OFFSET + tickSize, VERTICAL_OFFSET - pos), BLACK);
		putText(image, to_string(position), Point(HORIZONTAL_OFFSET - tickSize - tickOffsetH, VERTICAL_OFFSET - pos + tickOffsetV), CV_FONT_HERSHEY_SIMPLEX, 0.3, BLACK);

		line(image, Point(HORIZONTAL_OFFSET - tickSize, VERTICAL_OFFSET + pos), Point(HORIZONTAL_OFFSET + tickSize, VERTICAL_OFFSET + pos), BLACK);
		putText(image, to_string(-position), Point(HORIZONTAL_OFFSET - tickSize - tickOffsetH - 10, VERTICAL_OFFSET + pos + tickOffsetV), CV_FONT_HERSHEY_SIMPLEX, 0.3, BLACK);

		position += step;
		pos = toPixel(position);
	}

	return image;
}
