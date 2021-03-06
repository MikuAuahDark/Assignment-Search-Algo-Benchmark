#ifndef _NP_PATHCOST_H
#define _NP_PATHCOST_H

struct Path
{
	Edge *current;
	Path *prev;
};

struct PathCost
{
	Edge *current;
	PathCost *prev;
	int totalCost;
};

struct PathCostComparator
{
	bool operator()(const PathCost *a, const PathCost *b)
	{
		return a->totalCost > b->totalCost;
	}
};

struct PathDepth
{
	Edge *current;
	PathDepth *prev;
	size_t depth;
	bool active;
};

#endif
