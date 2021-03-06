#ifndef _NP_PROBLEM_H
#define _NP_PROBLEM_H

#include <vector>

#include "Edge.hpp"
#include "Vertex.hpp"

struct Problem
{
	static Problem *getSimpleProblem();
	static Problem *getRomaniaProblem();
	static Problem *getNumberProblem();

	Vertex *vertexes, *start, *end;
	Edge *edgeFull, *edgeDFS;

	size_t vertexesCount, edgeFullCount, edgeDFSCount;
};

#endif
