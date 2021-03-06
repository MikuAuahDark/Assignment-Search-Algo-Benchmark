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
	static Problem *getStarSystemProblem();

	Vertex *vertexes;
	Vertex *start;
	Vertex *end;
	Edge *edgeFull;
	Edge *edgeDFS;

	size_t vertexesCount;
	size_t edgeFullCount;
	size_t edgeDFSCount;
};

#endif
