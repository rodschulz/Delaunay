/**
 * Author: rodrigo
 * 2015
 */
#pragma once

#include "Vertex.h"

class Helper
{
public:
	/**
	 * Evaluates the orientation of the given points. If they are CCW, then
	 * returns a value > 0, if they are CW then the value returned is < 0
	 */
	static double getOrientation(const Vertex *_v1, const Vertex *_v2, const Vertex *_v3);

	/**
	 * Evaluates if the three given vertices are collinear
	 */
	static bool areCollinear(const Vertex *_v1, const Vertex *_v2, const Vertex *_v3);

private:
	Helper()
	{
	};

	~Helper()
	{
	};
};
