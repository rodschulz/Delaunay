/**
 * Author: rodrigo
 * 2015
 */
#include "Helper.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <limits>
#include "Printer.h"

using namespace std;

Helper::Helper()
{
}

Helper::~Helper()
{
}

double Helper::getOrientation(const Vertex *_v1, const Vertex *_v2, const Vertex *_v3)
{
	// This is the calculation of a determinant to evaluate the
	// orientation of the given points

	double ax = _v1->getX();
	double ay = _v1->getY();

	double bx = _v2->getX();
	double by = _v2->getY();

	double cx = _v3->getX();
	double cy = _v3->getY();

	return ax * (by - cy) - ay * (bx - cx) + (bx * cy - by * cx);
}

bool Helper::areCollinear(const Vertex *_v1, const Vertex *_v2, const Vertex *_v3)
{
	return abs(getOrientation(_v1, _v2, _v3)) < COMPARISON_EPSILON;
}

void Helper::readInput(const string &_location, vector<VertexPtr> &_destination)
{
	string line;
	ifstream inputFile;
	inputFile.open(_location.c_str(), fstream::in);
	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			// Parse string line
			vector<string> tokens;
			istringstream iss(line);
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

			// Generate new vertex
			VertexPtr newVertex(new Vertex(std::stof(tokens[0]), std::stof(tokens[1])));
			cout << "Vertex read: " << *newVertex << "\n";

			// Add to vector
			_destination.push_back(newVertex);
		}
		inputFile.close();
	}
	else
		cout << "Unable to open input: " << _location;
}

TrianglePtr Helper::calculateSurroundingTriangle(const vector<VertexPtr> &_vertexList)
{
	double minX = numeric_limits<double>::infinity();
	double maxX = -numeric_limits<double>::infinity();
	double minY = numeric_limits<double>::infinity();
	double maxY = -numeric_limits<double>::infinity();

	for (VertexPtr v : _vertexList)
	{
		minX = minX > v.get()->getX() ? v.get()->getX() : minX;
		maxX = maxX < v.get()->getX() ? v.get()->getX() : maxX;

		minY = minY > v.get()->getY() ? v.get()->getY() : minY;
		maxY = maxY < v.get()->getY() ? v.get()->getY() : maxY;
	}

	double width = maxX - minX;
	double height = maxY - minY;

	minX -= ceil(width / 4);
	maxX += ceil(width / 4);
	minY -= ceil(height / 4);
	maxY += ceil(height / 4);

	// Update width and height with the new values of minimums and maximums
	width = maxX - minX;
	height = maxY - minY;

	double upperVertexX = minX + (width / 2.0);
	double upperVertexY = maxY + (max(width, height) / 2.0);

	double leftVertexY = minY;
	double leftVertexX = (leftVertexY - upperVertexY) * ((minX - upperVertexX) / (maxY - upperVertexY)) + upperVertexX;

	double rightVertexY = minY;
	double rightVertexX = (rightVertexY - upperVertexY) * ((maxX - upperVertexX) / (maxY - upperVertexY)) + upperVertexX;

	VertexPtr upper(new Vertex(upperVertexX, upperVertexY));
	VertexPtr left(new Vertex(leftVertexX, leftVertexY));
	VertexPtr right(new Vertex(rightVertexX, rightVertexY));

	TrianglePtr triangle(new Triangle(left, right, upper));
	return triangle;
}

int Helper::shuffleGenerator(const int _i)
{
	srand(time(NULL));
	return std::rand() % _i;
}

void Helper::printTriangulation(const vector<TrianglePtr> &_triangulation, const string &_outputName)
{
	Mat image = Printer::generateBaseImage();

	Printer::printTriangulation(image, _triangulation);

	Printer::saveImage(_outputName, image);
}

void Helper::printSelectedTriangles(const vector<TrianglePtr> &_triangulation, const vector<TrianglePtr> &_selected, const VertexPtr &_vertex, const string &_outputName)
{
	Mat image = Printer::generateBaseImage();

	Printer::printTriangulation(image, _triangulation);
	Printer::printSelectedTriangles(image, _selected);
	Printer::printVertices(image, vector<VertexPtr>(1, _vertex));

	Printer::saveImage(_outputName, image);
}

void Helper::printAll(const vector<TrianglePtr> &_triangulation, const vector<VertexPtr> &_vertices, const string &_outputName)
{
	Mat image = Printer::generateBaseImage();

	Printer::printTriangulation(image, _triangulation);
	Printer::printVertices(image, _vertices);

	Printer::saveImage(_outputName, image);
}

void Helper::printNeighbors(const vector<TrianglePtr> &_triangulation, const string &_outputName, const string &_extension)
{
	for (size_t i = 0; i < _triangulation.size(); i++)
	{
		Mat image = Printer::generateBaseImage();
		Printer::printTriangulation(image, _triangulation);
		Printer::printNeighbors(image, _triangulation[i]);
		Printer::saveImage(_outputName + "_" + to_string(_triangulation[i]->getId()) + _extension, image);
	}
}

void Helper::printTriangle(const TrianglePtr &_triangle, const vector<VertexPtr> &_vertices, const string &_outputName)
{
	Mat image = Printer::generateBaseImage();

	Printer::printTriangulation(image, vector<TrianglePtr>(1, _triangle));
	Printer::printVertices(image, _vertices);

	Printer::saveImage(_outputName, image);
}
