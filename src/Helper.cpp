/**
 * Author: rodrigo
 * 2015
 */
#include "Helper.h"

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
