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

using namespace std;

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

vector<VertexPtr> Helper::readInput(const string &_location)
{
	vector<VertexPtr> input = vector<VertexPtr>();

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
			input.push_back(newVertex);
		}
		inputFile.close();
	}
	else
		cout << "Unable to open input: " << _location;

	return input;
}

TrianglePtr Helper::calculateSurroundingTriangle(const vector<VertexPtr> &_vertexList)
{
	float minX = numeric_limits<float>::infinity();
	float maxX = -numeric_limits<float>::infinity();
	float minY = numeric_limits<float>::infinity();
	float maxY = -numeric_limits<float>::infinity();

	for (VertexPtr v : _vertexList)
	{
		minX = minX > v.get()->getX() ? v.get()->getX() : minX;
		maxX = maxX < v.get()->getX() ? v.get()->getX() : maxX;

		minY = minY > v.get()->getY() ? v.get()->getY() : minY;
		maxY = maxY < v.get()->getY() ? v.get()->getY() : maxY;
	}

	float width = maxX - minX;
	float height = maxY - minY;

	minX -= ceil(width / 4);
	maxX += ceil(width / 4);
	minY -= ceil(height / 4);
	maxY += ceil(height / 4);

	// Update width and height with the new values of minimums and maximums
	width = maxX - minX;
	height = maxY - minY;

	float upperVertexX = minX + (width / 2.0);
	float upperVertexY = maxY + (max(width, height) / 2.0);

	float leftVertexY = minY;
	float leftVertexX = (leftVertexY - upperVertexY) * ((minX - upperVertexX) / (maxY - upperVertexY)) + upperVertexX;

	float rightVertexY = minY;
	float rightVertexX = (rightVertexY - upperVertexY) * ((maxX - upperVertexX) / (maxY - upperVertexY)) + upperVertexX;

	VertexPtr upper(new Vertex(upperVertexX, upperVertexY));
	VertexPtr left(new Vertex(leftVertexX, leftVertexY));
	VertexPtr right(new Vertex(rightVertexX, rightVertexY));

	TrianglePtr triangle(new Triangle(left, right, upper));
	return triangle;
}
