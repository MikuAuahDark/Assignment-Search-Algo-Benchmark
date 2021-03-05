#include "TreeSearch.hpp"

template<template<class> class alloc = std::allocator>
class TreeInitializer: public TreeSearch
{
protected:
	template<typename T>
	using TreeSet = std::set<T, std::less<T>, alloc<T>>;
	template<typename K, typename V>
	using TreeMap = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, alloc<std::pair<const K, V>>>;
	template<typename K>
	using TreeVector = std::vector<K, alloc<K>>;
	template<typename K>
	using TreeList = std::list<K, alloc<K>>;

public:
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
				neighboor.emplace(v, TreeSet<Edge*>());
			
			neighboor[v].insert(e);
		}

		return true;
	}

protected:
	TreeVector<Vertex*> verts;
	TreeVector<Edge*> edges;
	TreeVector<Edge> tempEdges;
	TreeMap<Vertex*, TreeSet<Edge*>> neighboor;
};
