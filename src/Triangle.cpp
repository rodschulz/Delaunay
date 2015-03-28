/**
 * Author: rodrigo
 * 2015
 */

#include <stdlib.h>
#include "Triangle.h"
#include "Helper.h"

Triangle::Triangle(VertexPtr &_vertex1, VertexPtr &_vertex2, VertexPtr _vertex3)
{
	vertices = vector<VertexPtr>();
	neighbors = vector<TrianglePtr>();

	id = getNextId();
	addInCCW(_vertex1, _vertex2, _vertex3);
	neighbors = vector<TrianglePtr>(3, NULL);
}

Triangle::Triangle(const Triangle &_other)
{
	id = _other.id;
	vertices = _other.vertices;
	neighbors = _other.neighbors;
}

Triangle::~Triangle()
{
}

unsigned long Triangle::getNextId() const
{
	static unsigned long nextId = 0;
	return nextId++;
}

void Triangle::addInCCW(const VertexPtr &_vertex1, const VertexPtr &_vertex2, const VertexPtr &_vertex3)
{
	if (Helper::getOrientation(_vertex1.get(), _vertex2.get(), _vertex3.get()) > 0)
	{
		vertices.push_back(_vertex1);
		vertices.push_back(_vertex2);
		vertices.push_back(_vertex3);
	}
	else
	{
		vertices.push_back(_vertex1);
		vertices.push_back(_vertex3);
		vertices.push_back(_vertex2);
	}
}

bool Triangle::contains(const VertexPtr &_vertex) const
{
	return (Helper::getOrientation(vertices[0].get(), vertices[1].get(), _vertex.get()) >= 0) && (Helper::getOrientation(vertices[1].get(), vertices[2].get(), _vertex.get()) >= 0) && (Helper::getOrientation(vertices[2].get(), vertices[0].get(), _vertex.get()) >= 0);
}

bool Triangle::inEdge(const VertexPtr &_vertex) const
{
	for (int i = 0; i < 3; i++)
	{
		if (!Helper::areCollinear(vertices[i].get(), vertices[(i + 1) % 3].get(), _vertex.get()))
			return true;
	}

	return false;
}

bool Triangle::setNeighbor(const TrianglePtr &_t)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((vertices[i] == _t->vertices[(j + 1) % 3]) && (vertices[(i + 1) % 3] == _t->vertices[j]))
			{
				neighbors[i] = _t;
				return true;
			}
		}
	}

	return false;
}

bool Triangle::isNeighbor(const TrianglePtr &_t) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((vertices[i] == _t->vertices[(j + 1) % 3]) && (vertices[(i + 1) % 3] == _t->vertices[j]))
				return true;
		}
	}

	return false;
}

pair<double, double> Triangle::getCenter() const
{
	double centerX = (vertices[0]->getX() + vertices[1]->getX() + vertices[2]->getX()) / 3;
	double centerY = (vertices[0]->getY() + vertices[1]->getY() + vertices[2]->getY()) / 3;

	return make_pair(centerX, centerY);
}

bool Triangle::isVertex(const VertexPtr &_vertex) const
{
	return (vertices[0] == _vertex) || (vertices[1] == _vertex) || (vertices[2] == _vertex);
}

bool Triangle::isInCircle(const VertexPtr &_vertex) const
{
	double a = vertices[0]->getX() - _vertex->getX();
	double b = vertices[0]->getY() - _vertex->getY();

	double c = vertices[1]->getX() - _vertex->getX();
	double d = vertices[1]->getY() - _vertex->getY();

	double e = vertices[2]->getX() - _vertex->getX();
	double f = vertices[2]->getY() - _vertex->getY();

	double result = (a * d * (e * e + f * f) + b * e * (c * c + d * d) + c * f * (a * a + b * b)) - (d * e * (a * a + b * b) + b * c * (e * e + f * f) + a * f * (c * c + d * d));

	return result > 0;
}

vector<pair<TrianglePtr, TrianglePtr>> Triangle::flipSide(TrianglePtr &_other)
{
	vector<pair<TrianglePtr, TrianglePtr>> output = vector<pair<TrianglePtr, TrianglePtr>>();

	// Get the side where the other currently is and update vertex of this triangle
	int i;
	for (i = 0; i < 3; i++)
		if (neighbors[i] == _other)
			break;
	vertices[i] = _other->getOppositeVertex(this);

	// Get the side where this triangle is for the other and update vertex of the neighbor
	int j;
	for (j = 0; j < 3; j++)
		if (_other->neighbors[j].get() == this)
			break;
	_other->vertices[j] = getOppositeVertex(_other.get());

	TrianglePtr thisPonter = _other->neighbors[j];

	vector<TrianglePtr> neighborsThis;
	neighborsThis.push_back(neighbors[(i + 2) % 3]);
	neighborsThis.push_back(neighbors[(i + 1) % 3]);

	vector<TrianglePtr> neighborsOther;
	neighborsOther.push_back(_other->neighbors[(i + 2) % 3]);
	neighborsOther.push_back(_other->neighbors[(i + 1) % 3]);

	// Update neighbors in THIS triangle
	setNeighbor(_other);

	i = 0;
	if (!setNeighbor(neighborsThis[i]))
		setNeighbor(neighborsThis[++i]);
	neighborsThis[i]->setNeighbor(thisPonter);
	output.push_back(make_pair(thisPonter, neighborsThis[i]));

	i = 0;
	if (!setNeighbor(neighborsOther[i]))
		setNeighbor(neighborsOther[++i]);
	neighborsOther[i]->setNeighbor(thisPonter);
	output.push_back(make_pair(thisPonter, neighborsOther[i]));

	// Update neighbors in OTHER triangle
	_other->setNeighbor(thisPonter);

	j = 0;
	if (!_other->setNeighbor(neighborsThis[j]))
		_other->setNeighbor(neighborsThis[++j]);
	neighborsThis[j]->setNeighbor(_other);
	output.push_back(make_pair(_other, neighborsThis[j]));

	j = 0;
	if (!_other->setNeighbor(neighborsOther[j]))
		_other->setNeighbor(neighborsOther[++j]);
	neighborsOther[j]->setNeighbor(_other);
	output.push_back(make_pair(_other, neighborsOther[j]));

	/**
	 // Update neighbors NOTE: (i + 2) % 3 == (i - 1) % 3
	 TrianglePtr aux = neighbors[(i + 3) % 3];
	 neighbors[(i + 3) % 3] = _other;
	 neighbors[i] = _other->neighbors[(j + 3) % 3];

	 _other->neighbors[(j + 3) % 3] = _other->neighbors[j];
	 _other->neighbors[j] = aux;
	 */


	/**
	// Push to check this triangle using the side that used to be the neighbor
	output.push_back(make_pair(thisPonter, neighbors[i]));
	// Push to check the neighbor using the side that used to be j + 1
	output.push_back(make_pair(_other, _other->neighbors[(j + 1) % 3]));
	*/

	return output;
}

VertexPtr Triangle::getOppositeVertex(const Triangle *_neighbor) const
{
	for (int i = 0; i < 3; i++)
		if (_neighbor == neighbors[i].get())
			return vertices[(i + 2) % 3];

	return VertexPtr(NULL);
}

std::ostream &operator<<(std::ostream &_stream, const Triangle &_triangle)
{
	_stream << *_triangle.vertices[0] << "\n" << *_triangle.vertices[1] << "\n" << *_triangle.vertices[2] << "\n" << *_triangle.vertices[0] << "\n\n";
	return _stream;
}
