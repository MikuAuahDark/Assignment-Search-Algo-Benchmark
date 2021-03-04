#ifndef _NP_BFS_H
#define _NP_BFS_H

#include <algorithm>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

#include "TreeSearch.hpp"

// Breadth First Search
template<template<class> class alloc = std::allocator>
class BFS: public TreeSearch
{
private:
	struct Path
	{
		Edge *current;
		Path *prev;
	};

	template<typename T>
	using BFSSet = std::set<T, std::less<T>, alloc<T>>;
	template<typename K, typename V>
	using BFSMap = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, alloc<std::pair<const K, V>>>;
	template<typename K>
	using BFSQueue = std::queue<K, std::deque<K, alloc<K>>>;
	template<typename K>
	using BFSVector = std::vector<K, alloc<K>>;

public:
	BFS() {}

	bool initialize(Vertex *vertexes, size_t vertexCount, Edge *es, size_t edgeCount) override
	{
		// Copy vertexes
		verts.resize(vertexCount);
		for (size_t i = 0; i < vertexCount; i++)
			verts[i] = vertexes + i;

		// Copy edges
		edges.resize(edgeCount);
		for (size_t i = 0; i < edgeCount; i++)
			edges[i] = es + i;

		// Create bidirectional graph to simulate undirected graph
		for (size_t i = 0; i < edgeCount; i++)
		{
			Edge &e = es[i];
			Edge r = e.reverse();

			// Too bad we have to do linear search
			bool found = false;
			for (size_t j = 0; j < edgeCount && !found; j++)
			{
				if (es[i] == r)
					found = true;
			}

			if (!found)
				// Allocate and insert
				tempEdges.emplace_back(r);
		}

		// Reallocate temporary edges
		tempEdges.shrink_to_fit();

		// Add new edges
		edges.reserve(edges.size() + tempEdges.size());
		for (size_t i = 0; i < tempEdges.size(); i++)
			edges.push_back(&tempEdges[i]);

		// Reallocate edges
		edges.shrink_to_fit();

		// Create connection between vertex
		for (size_t i = 0; i < edges.size(); i++)
		{
			Edge *e = edges[i];
			Vertex *v = e->getA();

			// Is vertex really defined?
			if (std::find(verts.begin(), verts.end(), v) == verts.end())
				return false;

			if (neighboor.find(v) == neighboor.end())
				neighboor.emplace(v, BFSSet<Edge*>());
			
			neighboor[v].insert(e);
		}

		return true;
	}

	TreeSearch::Result *find(Vertex *source, Vertex *destination) override
	{
		if (
			neighboor.find(source) == neighboor.end() ||
			std::find(verts.begin(), verts.end(), destination) == verts.end()
		)
			return nullptr;
		
		BFSVector<Path> paths;
		BFSQueue<Path*> queue;
		BFSSet<Vertex*> visited;
		int expansion = 1;

		visited.insert(source);

		// Do first expansion
		for (Edge *adj: neighboor[source])
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
				TreeSearch::Result *result = newResult(paths.size());
				std::copy(paths.begin(), paths.end(), result->vertexes);

				return result;
			}

			// If there are no recursion, go ahead
			if (visited.find(next) == visited.end())
			{
				visited.insert(next);

				if (neighboor.find(next) != neighboor.end())
				{
					BFSSet<Edge*> &adjacent = neighboor[next];

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

private:
	BFSVector<Vertex*> verts;
	BFSVector<Edge*> edges;
	BFSVector<Edge> tempEdges;
	BFSMap<Vertex*, BFSSet<Edge*>> neighboor;
};

#endif
