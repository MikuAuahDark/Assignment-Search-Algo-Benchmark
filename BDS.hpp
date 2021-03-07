#ifndef _NP_BDS_H
#define _NP_BDS_H

#include <queue>
#include <stack>

#include "TreeInitializer.hpp"

template<template<class> class alloc = std::allocator>
class BDS: public TreeInitializer<alloc>
{
private:
	template<typename K>
	using BDSQueue = std::queue<K, std::deque<K, alloc<K>>>;
	template<typename K>
	using BDSSet = typename TreeInitializer<alloc>::template TreeSet<K>;
	template<typename K>
	using BDSList = typename TreeInitializer<alloc>::template TreeList<K>;
	template<typename K>
	using BDSStack = std::stack<K, std::deque<K, alloc<K>>>;

public:
	TreeSearch::Result *find(Vertex *source, Vertex *destination) override
	{
		if (
			this->neighboor.find(source) == this->neighboor.end() ||
			std::find(this->verts.begin(), this->verts.end(), destination) == this->verts.end()
		)
			return nullptr;

		BDSList<Path> pathDFS;
		BDSList<Path> pathBFS;
		BDSQueue<Path*> queue;
		BDSStack<Path*> stack;
		BDSSet<Vertex*> visited;
		int expansion = 1;

		// Do first expansion (BFS/source)
		for (Edge *adj: this->neighboor[source])
		{
			pathBFS.emplace_back();
			Path &p = pathBFS.back();

			p.current = adj;
			p.prev = nullptr;
			queue.push(&p);
		}

		// Do first expansion (DFS/destination)
		for (Edge *adj: this->neighboor[destination])
		{
			pathDFS.emplace_back();
			Path &p = pathDFS.back();

			p.current = adj;
			p.prev = nullptr;
			stack.push(&p);
		}

		while (!stack.empty() || !queue.empty())
		{
			// Source part
			Path *ps = queue.empty() ? nullptr : queue.front();
			Vertex *nextBFS = ps ? ps->current->getB() : nullptr;
			Path *pd = stack.empty() ? nullptr : stack.top();
			Vertex *nextDFS = pd ? pd->current->getB() : nullptr;
			Path *intersect = nullptr;
			bool intersectBFS = false;

			if (nextBFS == nextDFS)
				intersect = pd;

			if (!intersect && pd)
			{
				// Check whetever there's intersect from BFS to DFS
				for (auto a = pathDFS.rbegin(); a != pathDFS.rend(); a++)
				{
					Path &p = *a;
					if (p.current->getB() == nextBFS)
					{
						intersect = &p;
						break;
					}
				}
			}

			if (!intersect && ps)
			{
				// Check whetever there's intersect from DFS to BFS
				for (auto a = pathBFS.rbegin(); a != pathBFS.rend(); a++)
				{
					Path &p = *a;
					if (p.current->getB() == nextDFS)
					{
						intersect = &p;
						intersectBFS = true;
						break;
					}
				}
			}

			// Does it intersect?
			if (intersect)
			{
				std::vector<Vertex*> paths;
				paths.push_back(nextBFS);
				Path *p1 = intersectBFS ? intersect : ps;
				Path *p2 = intersectBFS ? pd : intersect;

				// Backtrack BFS part
				while (p1)
				{
					paths.push_back(p1->current->getA());
					p1 = p1->prev;
				}

				std::reverse(paths.begin(), paths.end());

				// Backtrack DFS part
				while (p2)
				{
					paths.push_back(p2->current->getA());
					p2 = p2->prev;
				}

				// New result
				TreeSearch::Result *result = TreeSearch::newResult(paths.size());
				result->cost = result->expansions = expansion;

				std::copy(paths.begin(), paths.end(), result->vertexes);
				return result;
			}

			// Step BFS
			if (ps && visited.find(nextBFS) == visited.end())
			{
				visited.insert(nextBFS);

				if (this->neighboor.find(nextBFS) != this->neighboor.end())
				{
					BDSSet<Edge*> &adjacent = this->neighboor[nextBFS];

					for (Edge *adj: adjacent)
					{
						if (adj->getB() != ps->current->getA())
						{
							pathBFS.emplace_back();
							Path &newPath = pathBFS.back();

							newPath.current = adj;
							newPath.prev = ps;

							queue.push(&newPath);
						}
					}
				}
			}

			// Step DFS
			if (pd && this->neighboor.find(nextDFS) != this->neighboor.end())
			{
				BDSSet<Edge*> &adjacent = this->neighboor[nextDFS];

				for (Edge *adj: adjacent)
				{
					if (adj->getB() != pd->current->getA())
					{
						pathDFS.emplace_back();
						Path &newPath = pathDFS.back();

						newPath.current = adj;
						newPath.prev = pd;

						stack.push(&newPath);
					}
				}
			}
		}

		return nullptr;
	}
};

#endif
