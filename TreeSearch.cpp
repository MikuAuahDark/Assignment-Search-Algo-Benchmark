#include <memory>

#include "TreeSearch.hpp"

TreeSearch::~TreeSearch()
{}

TreeSearch::Result *TreeSearch::newResult(size_t vertexCount)
{
	void *temp = malloc(sizeof(TreeSearch::Result) + sizeof(void*) * vertexCount);
	Result *ret = (Result *) temp;

	if (ret)
	{
		ret->vertexes = (Vertex **) (ret + 1);
		ret->vertexCount = vertexCount;
	}

	return ret;
}
