#ifndef _NP_UCS_H
#define _NP_UCS_H

#include <queue>

#include "BFS.hpp"
#include "PathCost.hpp"

// Uniform Cost Search
template<template<class> class alloc = std::allocator>
class UCS: public TreeInitializer<alloc>
{
private:
	template<typename K>
	using UCSVector = typename TreeInitializer<alloc>::template TreeVector<K>;
	template<typename K, typename C = std::less<K>>
	using UCSQueue = std::priority_queue<K, UCSVector<K>, C>;
	template<typename K>
	using UCSList = typename TreeInitializer<alloc>::template TreeList<K>;
	template<typename K>
	using UCSSet = typename TreeInitializer<alloc>::template TreeSet<K>;

public:
	TreeSearch::Result *find(Vertex *source, Vertex *destination) override
	{
		if (
			this->neighboor.find(source) == this->neighboor.end() ||
			std::find(this->verts.begin(), this->verts.end(), destination) == this->verts.end()
		)
			return nullptr;
		
		UCSList<PathCost> paths;
		UCSQueue<PathCost*, PathCostComparator> queue{PathCostComparator()};
		UCSSet<Vertex*> visited;
		int expansion = 1;

		visited.insert(source);

		// Do first expansion
		for (Edge *adj: this->neighboor[source])
		{
			paths.emplace_back();
			PathCost &p = paths.back();

			p.current = adj;
			p.prev = nullptr;
			p.totalCost = adj->getCost();
			queue.push(&p);
		}

		while (!queue.empty())
		{
			PathCost *p = queue.top();
			Vertex *next = p->current->getB();

			expansion++;
			queue.pop();

			// Is it our destination?
			if (next == destination)
			{
				// Can't use custom allocator, we don't want this as part
				// of the allocation
				std::vector<Vertex*> paths;
				paths.push_back(next);

				int totalCost = p->totalCost;

				// Backtrack
				while (p)
				{
					paths.push_back(p->current->getA());
					p = p->prev;
				}

				std::reverse(paths.begin(), paths.end());

				// New result
				TreeSearch::Result *result = TreeSearch::newResult(paths.size());
				result->cost = totalCost;
				result->expansions = expansion;

				std::copy(paths.begin(), paths.end(), result->vertexes);
				return result;
			}

			// If there are no recursion, go ahead
			if (visited.find(next) == visited.end())
			{
				visited.insert(next);

				if (this->neighboor.find(next) != this->neighboor.end())
				{
					UCSSet<Edge*> &adjacent = this->neighboor[next];

					for (Edge *adj: adjacent)
					{
						if (adj->getB() != p->current->getA())
						{
							paths.emplace_back();
							PathCost &newPath = paths.back();

							newPath.current = adj;
							newPath.prev = p;
							newPath.totalCost = adj->getCost() + p->totalCost;

							queue.push(&newPath);
						}
					}
				}
			}
		}

		return nullptr;
	}
};

#endif
