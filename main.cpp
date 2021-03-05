#include <cstdlib>

#include <iostream>
#include <vector>

#include "BFS.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"

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

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<Vertex> vertexes = {
		Vertex("Arad"),          // 0
		Vertex("Timisoara"),     // 1
		Vertex("Zerind"),        // 2
		Vertex("Oradea"),        // 3
		Vertex("Lugoj"),         // 4
		Vertex("Drobeta"),       // 5
		Vertex("Mehadia"),       // 6
		Vertex("Sibiu"),         // 7
		Vertex("Rimnicu Vilcea"),// 8
		Vertex("Craiova"),       // 9
		Vertex("Fagaras"),       // 10
		Vertex("Pitesti"),       // 11
		Vertex("Giurgiu"),       // 12
		Vertex("Bucharest"),     // 13
		Vertex("Neamt"),         // 14
		Vertex("Urziceni"),      // 15
		Vertex("Iasi"),          // 16
		Vertex("Vaslui"),        // 17
		Vertex("Hirsova"),       // 18
		Vertex("Eforie")         // 19
	};
	std::vector<Edge> edges = {
		// Arah deklarasi ke kanan
		// akan otomatis membuat sisi arah lainnya
		Edge(vertexes[0], vertexes[1], 118),
		Edge(vertexes[0], vertexes[2], 75),
		Edge(vertexes[0], vertexes[7], 140),
		Edge(vertexes[1], vertexes[4], 111),
		Edge(vertexes[2], vertexes[3], 71),
		Edge(vertexes[3], vertexes[7], 151),
		Edge(vertexes[4], vertexes[6], 70),
		Edge(vertexes[5], vertexes[9], 120),
		Edge(vertexes[6], vertexes[5], 75),
		Edge(vertexes[7], vertexes[8], 80),
		Edge(vertexes[7], vertexes[10], 99),
		Edge(vertexes[8], vertexes[9], 146),
		Edge(vertexes[8], vertexes[11], 97),
		Edge(vertexes[9], vertexes[11], 138),
		Edge(vertexes[10], vertexes[13], 211),
		Edge(vertexes[11], vertexes[13], 101),
		Edge(vertexes[13], vertexes[12], 90),
		Edge(vertexes[13], vertexes[15], 85),
		Edge(vertexes[15], vertexes[17], 142),
		Edge(vertexes[15], vertexes[18], 98),
		Edge(vertexes[16], vertexes[14], 87),
		Edge(vertexes[17], vertexes[16], 92),
		Edge(vertexes[18], vertexes[19], 86)
	};

	TreeSearch *bfs = new BFS<TrackingAllocator>();
	if (bfs->initialize(vertexes.data(), vertexes.size(), edges.data(), edges.size()))
	{
		std::cout << "Arad -> Bucharest: BFS" << std::endl;

		// Find from Arad to Bucharest
		TreeSearch::Result *res = bfs->find(&vertexes[0], &vertexes[13]);

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

			free(res);
		}
	}

	delete bfs;
	return 0;
}
