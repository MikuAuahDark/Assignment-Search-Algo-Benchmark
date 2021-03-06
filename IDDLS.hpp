#ifndef _NP_IDDLS_H
#define _NP_IDDLS_H

template<template<class> class alloc = std::allocator>
class IDDLS: public DLS<alloc>
{
public:
	IDDLS(size_t max): DLS<alloc>(0), maxDepth(max) {}

	TreeSearch::Result *find(Vertex *source, Vertex *destination) override
	{
		if (
			this->neighboor.find(source) == this->neighboor.end() ||
			std::find(this->verts.begin(), this->verts.end(), destination) == this->verts.end()
		)
			return nullptr;

		TreeSearch::Result *r = nullptr;
		for (this->depth = 0; this->depth <= maxDepth && !r; this->depth++)
			r = this->DLS<alloc>::find(source, destination);

		return r;
	}

private:
	size_t maxDepth;
};

#endif
