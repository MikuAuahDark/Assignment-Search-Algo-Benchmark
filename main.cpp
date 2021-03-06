#include <cstdlib>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "BFS.hpp"
#include "DFS.hpp"
#include "DLS.hpp"
#include "UCS.hpp"

#include "Edge.hpp"
#include "Vertex.hpp"

#include "Problem.hpp"

static size_t trackedMemory = 0;
//static std::atomic<size_t> trackedMemory {0};

template<typename T>
struct TrackingAllocator
{
	using value_type = T;

	constexpr TrackingAllocator() noexcept {}
    constexpr TrackingAllocator(const TrackingAllocator&) noexcept = default;
    template <class U>
    constexpr TrackingAllocator(const TrackingAllocator<U>&) noexcept {}

	T *allocate(size_t count, void *hint = 0)
	{
		size_t bytes = count * sizeof(T);
		void *result = malloc(bytes);

		if (result)
		{
			trackedMemory += bytes;
			std::cerr << "TrackingAllocator: add " << bytes << std::endl;
		}

		return (T *) result;
	}

	void deallocate(T *ptr, size_t count)
	{
		if (ptr)
		{
			size_t bytes = count * sizeof(T);
			trackedMemory -= (ptr != nullptr) * count * sizeof(T);

			free(ptr);
			std::cerr << "TrackingAllocator: sub " << bytes << std::endl;
		}
	}

	template <class U>
	bool operator==(const TrackingAllocator<U> &a) {return true;}
	template <class U>
	bool operator!=(const TrackingAllocator<U> &a) {return false;}
};

void startTest(
	const std::string &algo,
	const std::string &name,
	Vertex *vertexes,
	size_t vertexSize,
	Edge *edges,
	size_t edgeSize,
	TreeSearch *t,
	Vertex *a, Vertex *b
)
{
	std::cerr << "TrackingAllocator: str " << name << " " << algo << std::endl;
	std::cerr << "TrackingAllocator: reg initialize" << std::endl;

	auto startTime = std::chrono::high_resolution_clock::now();
	bool result = t->initialize(vertexes, vertexSize, edges, edgeSize);
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - startTime;

	std::cerr << "TrackingAllocator: edr initialize " << size_t(duration.count() * 1000000.0) << std::endl;

	if (result)
	{
		std::cout << name << " problem with " << algo << ": Points from " << (std::string) (*a) << " -> " << (std::string) (*b) << std::endl;
		std::cerr << "TrackingAllocator: reg find" << std::endl;

		startTime = std::chrono::high_resolution_clock::now();
		TreeSearch::Result *res = t->find(a, b);
		duration = std::chrono::high_resolution_clock::now() - startTime;

		std::cerr << "TrackingAllocator: edr find " << size_t(duration.count() * 1000000.0) << std::endl;

		if (res == nullptr)
			std::cout << "Not found" << std::endl;
		else
		{
			for (size_t i = 0; i < res->vertexCount; i++)
			{
				std::cout << (std::string) (*res->vertexes[i]);

				if (i != res->vertexCount - 1)
					std::cout << " --> ";
				else
					std::cout << std::endl;
			}

			std::cout << "Cost: " << res->cost << std::endl << "Expansion: " << res->expansions << std::endl;
			free(res);
		}
	}
	else
		std::cout << "Unable to initialize" << std::endl;

	std::cerr << "TrackingAllocator: stp " << name << " " << algo << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	Problem *romania = Problem::getRomaniaProblem();
	Problem *simple = Problem::getSimpleProblem();

	// Test
	TreeSearch *tree = new BFS<TrackingAllocator>();
	startTest("BFS", "Romania", romania->vertexes, romania->vertexesCount, romania->edgeFull, romania->edgeFullCount, tree, &romania->vertexes[0], &romania->vertexes[13]);
	delete tree;

	tree = new UCS<TrackingAllocator>();
	startTest("UCS", "Romania", romania->vertexes, romania->vertexesCount, romania->edgeFull, romania->edgeFullCount, tree, &romania->vertexes[0], &romania->vertexes[13]);
	delete tree;

	if (trackedMemory != 0)
		std::cerr << "TrackingAllocator: err possible memory leak allocated " << trackedMemory << std::endl;

	return 0;
}
