/**
 * Author: rodrigo
 * 2015
 */

#include <stdlib.h>
#include "Triangle.h"
#include "Helper.h"
#include <iostream>

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

	int i = getNeighborIndex(_other.get());
	int j = _other->getNeighborIndex(this);

	// Since j is the pointer to this triangle, then store a pointer
	TrianglePtr thisPonter = _other->neighbors[j];

	vector<pair<TrianglePtr, Edge>> neighborsThis;
	for (int k = 1; k < 3; k++)
		neighborsThis.push_back(make_pair(neighbors[(i + k) % 3], getEdge((i + k) % 3)));

	cout << "neighborThis: " << neighborsThis[0].second << " - id: " << neighborsThis[0].first->id << "\n";
	cout << "neighborThis: " << neighborsThis[1].second << " - id: " << neighborsThis[1].first->id << "\n";

	vector<pair<TrianglePtr, Edge>> neighborsOther;
	for (int k = 1; k < 3; k++)
		neighborsOther.push_back(make_pair(_other->neighbors[(j + k) % 3], _other->getEdge((j + k) % 3)));

	cout << "neighborsOther: " << neighborsOther[0].second << " - id: " << neighborsOther[0].first->id << "\n";
	cout << "neighborsOther: " << neighborsOther[1].second << " - id: " << neighborsOther[1].first->id << "\n";

	// Update vertices
	vertices[i] = _other->getOppositeVertex(this);
	_other->vertices[j] = getOppositeVertex(_other.get());

	cout << "this " << *this << "\n";
	cout << "other " << *_other << "\n";

	/*for (int k = 1; k < 3; k++)
	 {
	 if (neighbors[(i + k) % 3] != NULL)
	 neighborsThis.push_back(neighbors[(i + k) % 3]);
	 }

	 vector<TrianglePtr> neighborsOther;
	 for (int k = 1; k < 3; k++)
	 {
	 if (_other->neighbors[(j + k) % 3] != NULL)
	 neighborsOther.push_back(_other->neighbors[(j + k) % 3]);
	 }*/

	// TODO: put in the output vector only the 2 neighbors that must be checked
	// Update neighbors in THIS triangle
	setNeighbor(_other);
	for (size_t k = 0; k < neighborsThis.size(); k++)
	{
		cout << "checking " << neighborsThis[k].second << "\n";
		if (hasEdge(neighborsThis[k].second))
		{
			if (neighborsThis[k].first != NULL)
			{
				cout << "setting neighbor " << *neighborsThis[k].first << "\n";

				setNeighbor(neighborsThis[k].first);
				neighborsThis[k].first->setNeighbor(thisPonter);
				output.push_back(make_pair(thisPonter, neighborsThis[k].first));
			}
			break;
		}
	}
	for (size_t k = 0; k < neighborsOther.size(); k++)
	{
		cout << "checking " << neighborsOther[k].second << "\n";

		if (hasEdge(neighborsOther[k].second))
		{
			if (neighborsOther[k].first != NULL)
			{
				cout << "setting neighbor " << *neighborsOther[k].first << "\n";

				setNeighbor(neighborsOther[k].first);
				neighborsOther[k].first->setNeighbor(thisPonter);
				output.push_back(make_pair(thisPonter, neighborsOther[k].first));
			}
			break;
		}
	}

	// Update neighbors in OTHER triangle
	_other->setNeighbor(thisPonter);
	for (size_t k = 0; k < neighborsThis.size(); k++)
	{
		cout << "checking " << neighborsThis[k].second << "\n";

		if (_other->hasEdge(neighborsThis[k].second))
		{
			if (neighborsThis[k].first != NULL)
			{
				cout << "setting neighbor " << *neighborsThis[k].first << "\n";

				_other->setNeighbor(neighborsThis[k].first);
				neighborsThis[k].first->setNeighbor(_other);
				output.push_back(make_pair(_other, neighborsThis[k].first));
			}
			break;
		}
	}
	for (size_t k = 0; k < neighborsOther.size(); k++)
	{
		cout << "checking " << neighborsOther[k].second << "\n";

		if (_other->hasEdge(neighborsOther[k].second))
		{
			if (neighborsOther[k].first != NULL)
			{
				cout << "setting neighbor " << *neighborsOther[k].first << "\n";

				_other->setNeighbor(neighborsOther[k].first);
				neighborsOther[k].first->setNeighbor(_other);
				output.push_back(make_pair(_other, neighborsOther[k].first));
			}
			break;
		}
	}

	/*i = 0;
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

Edge Triangle::getEdge(const int _n) const
{
	return Edge(vertices[_n % 3], vertices[(_n + 1) % 3]);
}

std::ostream &operator<<(std::ostream &_stream, const Triangle &_triangle)
{
	_stream << "id: " << _triangle.id << " vertices: " << *_triangle.vertices[0] << " - " << *_triangle.vertices[1] << " - " << *_triangle.vertices[2];
	return _stream;
}

int Triangle::getNeighborIndex(const Triangle *_neighbor) const
{
	for (int i = 0; i < 3; i++)
		if (neighbors[i].get() == _neighbor)
			return i;

	return -1;
}

bool Triangle::hasEdge(const Edge &_edge) const
{
	for (int i = 0; i < 3; i++)
	{
		if (_edge.equals(getEdge(i)))
			return true;
	}

	return false;
}
