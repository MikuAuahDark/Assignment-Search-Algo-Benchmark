#ifndef _NP_BFS_H
#define _NP_BFS_H

#include <algorithm>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

#include "TreeInitializer.hpp"
#include "PathCost.hpp"

// Breadth First Search
template<template<class> class alloc = std::allocator>
class BFS: public TreeInitializer<alloc>
{
private:
	template<typename K>
	using BFSQueue = std::queue<K, std::deque<K, alloc<K>>>;
	template<typename K>
	using BFSSet = typename TreeInitializer<alloc>::template TreeSet<K>;
	template<typename K>
	using BFSList = typename TreeInitializer<alloc>::template TreeList<K>;

public:
	TreeSearch::Result *find(Vertex *source, Vertex *destination) override
	{
		if (
			this->neighboor.find(source) == this->neighboor.end() ||
			std::find(this->verts.begin(), this->verts.end(), destination) == this->verts.end()
		)
			return nullptr;
		
		BFSList<Path> paths;
		BFSQueue<Path*> queue;
		BFSSet<Vertex*> visited;
		int expansion = 1;

		visited.insert(source);

		// Do first expansion
		for (Edge *adj: this->neighboor[source])
		{
			paths.emplace_back();
			Path &p = paths.back();

			p.current = adj;
			p.prev = nullptr;
			queue.push(&p);
		}

		while (!queue.empty())
		{
			Path *p = queue.front();
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

				// Backtrack
				while (p)
				{
					paths.push_back(p->current->getA());
					p = p->prev;
				}

				std::reverse(paths.begin(), paths.end());

				// New result
				TreeSearch::Result *result = TreeSearch::newResult(paths.size());
				result->cost = result->expansions = expansion;

				std::copy(paths.begin(), paths.end(), result->vertexes);
				return result;
			}

			// If there are no recursion, go ahead
			if (visited.find(next) == visited.end())
			{
				visited.insert(next);

				if (this->neighboor.find(next) != this->neighboor.end())
				{
					BFSSet<Edge*> &adjacent = this->neighboor[next];

					for (Edge *adj: adjacent)
					{
						if (adj->getB() != p->current->getA())
						{
							paths.emplace_back();
							Path &newPath = paths.back();

							newPath.current = adj;
							newPath.prev = p;

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
