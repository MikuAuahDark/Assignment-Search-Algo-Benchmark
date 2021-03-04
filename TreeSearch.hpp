#ifndef _NP_TREESEARCH_H
#define _NP_TREESEARCH_H

#include <memory>

#include "Edge.hpp"
#include "Vertex.hpp"

// TreeSearch requirements: vertexes and edges pointr must alive until
// object destroyed!
class TreeSearch
{
public:
	struct Result
	{
		Vertex **vertexes;
		size_t vertexCount;

		int cost;
		int expansions;
	};

	virtual ~TreeSearch();
	virtual bool initialize(Vertex *vertexes, size_t vertexCount, Edge *edges, size_t edgeCount) = 0;
	virtual Result *find(Vertex *source, Vertex *destination) = 0;

	static Result *newResult(size_t vertexCount);
};

#endif
