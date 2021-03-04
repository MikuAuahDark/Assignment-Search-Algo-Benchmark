#ifndef _NP_EDGE_H
#define _NP_EDGE_H

#include "Vertex.hpp"

struct Edge
{
	Edge(Vertex &a, Vertex &b, int cost = 1);

	Vertex *getA() const noexcept;
	Vertex *getB() const noexcept;
	int getCost() const noexcept;
	Edge reverse() const noexcept;
	bool contains(const Vertex &v) const noexcept;

	bool operator==(const Edge &e) const noexcept;

private:
	Vertex *a, *b;
	int cost;
};

#endif
