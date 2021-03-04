#include "Edge.hpp"

Edge::Edge(Vertex &a, Vertex &b, int cost)
: a(&a)
, b(&b)
, cost(cost)
{}

Vertex *Edge::getA() const noexcept
{
	return a;
}

Vertex *Edge::getB() const noexcept
{
	return b;
}

int Edge::getCost() const noexcept
{
	return cost;
}

Edge Edge::reverse() const noexcept
{
	return Edge(*b, *a, cost);
}

bool Edge::contains(const Vertex &v) const noexcept
{
	return *a == v || *b == v;
}

bool Edge::operator==(const Edge &e) const noexcept
{
	return *a == *e.a && *b == *e.b;
}
