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

		BDSList<PathDepth> paths;
		//BDSQueue<
	}

protected:
	static PathDepth &findPathDepth(BDSList<PathDepth> &list)
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
