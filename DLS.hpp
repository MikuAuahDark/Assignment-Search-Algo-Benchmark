#ifndef _NP_DLS_H
#define _NP_DLS_H

#include <stack>

#include "TreeInitializer.hpp"
#include "PathCost.hpp"

template<template<class> class alloc = std::allocator>
class DLS: public TreeInitializer<alloc>
{
private:
	template<typename K>
	using DLSSet = typename TreeInitializer<alloc>::template TreeSet<K>;
	template<typename K>
	using DLSList = typename TreeInitializer<alloc>::template TreeList<K>;
	template<typename K>
	using DLSStack = std::stack<K, std::deque<K, alloc<K>>>;

public:
	DLS(size_t depth): depth(depth) {}
	DLS *setDepth(size_t d) { depth = d; return this; }

	TreeSearch::Result *find(Vertex *source, Vertex *destination) override
	{
		if (
			this->neighboor.find(source) == this->neighboor.end() ||
			std::find(this->verts.begin(), this->verts.end(), destination) == this->verts.end()
		)
			return nullptr;
		
		allDiscovered = true;

		DLSList<PathDepth> paths;
		DLSStack<PathDepth*> stack;
		DLSSet<Vertex*> visited;
		int expansion = 1;

		// Do first expansion
		for (Edge *adj: this->neighboor[source])
		{
			PathDepth &p = findPathDepth(paths);

			p.current = adj;
			p.prev = nullptr;
			p.depth = 0;
			p.active = true;
			stack.push(&p);
		}

		while (!stack.empty())
		{
			PathDepth *p = stack.top();
			Vertex *next = p->current->getB();

			expansion++;
			stack.pop();

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

			// If the depth is less than the total cost, go ahead
			if (p->depth < depth)
			{
				if (this->neighboor.find(next) != this->neighboor.end())
				{
					DLSSet<Edge*> &adjacent = this->neighboor[next];

					for (Edge *adj: adjacent)
					{
						if (adj->getB() != p->current->getA())
						{
							PathDepth &newPath = findPathDepth(paths);

							newPath.current = adj;
							newPath.prev = p;
							newPath.depth = p->depth + 1;
							newPath.active = true;

							stack.push(&newPath);
						}
					}

					if (adjacent.size() == 0)
						p->active = false;
				}
				else
					p->active = false;
			}
			else
			{
				p->active = false;
				allDiscovered = false;
			}
		}

		return nullptr;
	}
protected:
	size_t depth;
	bool allDiscovered;

	static PathDepth &findPathDepth(DLSList<PathDepth> &list)
	{
		for (PathDepth &d: list)
		{
			if (!d.active)
				return d;
		}

		list.emplace_back();
		return list.back();
	}
};

#endif
