#ifndef _NP_DFS_H
#define _NP_DFS_H

#include <stack>

#include "DLS.hpp"
#include "PathCost.hpp"

// Depth First Search
template<template<class> class alloc = std::allocator>
class DFS: public DLS<alloc>
{
public:
	DFS(): DLS<alloc>(~size_t(0)) {}
};

#endif
