#include <cstdlib>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "BFS.hpp"
#include "DFS.hpp"
#include "IDDLS.hpp"
#include "UCS.hpp"
#include "BDS.hpp"

#include "Edge.hpp"
#include "Vertex.hpp"

#include "Problem.hpp"

static size_t trackedMemory = 0;

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
	if (name == "Romania" && algo == "DFS") return; // infinite loop, can't be tested

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
	std::cout << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<std::pair<std::string, Problem*>> problems = {
		{"Simple", Problem::getSimpleProblem()},
		{"Romania", Problem::getRomaniaProblem()},
		{"Number", Problem::getNumberProblem()},
		{"StarSystem", Problem::getStarSystemProblem()}
	};

	// Test
	for (std::pair<std::string, Problem*> &p: problems)
	{
		Problem *pr = p.second;
		std::string &name = p.first;

		TreeSearch *tree = new BFS<TrackingAllocator>();
		startTest("BFS", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;

		tree = new DFS<TrackingAllocator>();
		//startTest("DFS", name, pr->vertexes, pr->vertexesCount, pr->edgeDFS, pr->edgeDFSCount, tree, pr->start, pr->end);
		startTest("DFS", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;

		tree = new DLS<TrackingAllocator>(5);
		startTest("DLS5", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;
		
		tree = new DLS<TrackingAllocator>(10);
		startTest("DLS10", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;
		
		tree = new DLS<TrackingAllocator>(20);
		startTest("DLS20", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;
		
		tree = new DLS<TrackingAllocator>(40);
		startTest("DLS40", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;

		tree = new DLS<TrackingAllocator>(60);
		startTest("DLS60", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;

		tree = new UCS<TrackingAllocator>();
		startTest("UCS", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;

		tree = new IDDLS<TrackingAllocator>(100);
		startTest("IDDLS", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;
		
		/*
		tree = new BDS<TrackingAllocator>();
		startTest("BDS", name, pr->vertexes, pr->vertexesCount, pr->edgeFull, pr->edgeFullCount, tree, pr->start, pr->end);
		delete tree;
		*/
	}

	if (trackedMemory != 0)
		std::cerr << "TrackingAllocator: err possible memory leak allocated " << trackedMemory << std::endl;

	return 0;
}
