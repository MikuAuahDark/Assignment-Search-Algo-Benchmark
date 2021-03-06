#include "Problem.hpp"

////////////////////
// Simple Problem //
////////////////////

static Vertex simpleVertexes[] = {
	"S", // 0
	"L", // 1
	"A", // 2
	"C", // 3
	"K", // 4
	"B", // 5
	"D", // 6
	"H", // 7
	"G", // 8
	"E", // 9
	"I", // 10
	"F", // 11
	"J"	 // 12
};
constexpr size_t simpleVertexSize = sizeof(simpleVertexes) / sizeof(Vertex);

static Edge simpleEdgeFull[] = {
	Edge(simpleVertexes[0] , simpleVertexes[2] , 2),
	Edge(simpleVertexes[0] , simpleVertexes[3] , 1),
	Edge(simpleVertexes[0] , simpleVertexes[4] , 2),
	Edge(simpleVertexes[4] , simpleVertexes[1] , 1),
	Edge(simpleVertexes[2] , simpleVertexes[5] , 2),
	Edge(simpleVertexes[3] , simpleVertexes[6] , 1),
	Edge(simpleVertexes[5] , simpleVertexes[7] , 2),
	Edge(simpleVertexes[5] , simpleVertexes[8] , 3),
	Edge(simpleVertexes[6] , simpleVertexes[9] , 1),
	Edge(simpleVertexes[7] , simpleVertexes[10], 2),
	Edge(simpleVertexes[8] , simpleVertexes[10], 5),
	Edge(simpleVertexes[8] , simpleVertexes[11], 1),
	Edge(simpleVertexes[9] , simpleVertexes[11], 1),
	Edge(simpleVertexes[10], simpleVertexes[12], 1)
};
constexpr size_t simpleEdgeSize = sizeof(simpleEdgeFull) / sizeof(Edge);

static Edge simpleEdgeDFS[] = {
	Edge(simpleVertexes[0] , simpleVertexes[2] , 2),
	Edge(simpleVertexes[0] , simpleVertexes[3] , 1),
	Edge(simpleVertexes[0] , simpleVertexes[4] , 2),
	Edge(simpleVertexes[4] , simpleVertexes[1] , 1),
	Edge(simpleVertexes[2] , simpleVertexes[5] , 2),
	Edge(simpleVertexes[3] , simpleVertexes[6] , 1),
	Edge(simpleVertexes[5] , simpleVertexes[7] , 2),
	//Edge(simpleVertexes[5] , simpleVertexes[8] , 3),
	Edge(simpleVertexes[6] , simpleVertexes[9] , 1),
	Edge(simpleVertexes[7] , simpleVertexes[10], 2),
	Edge(simpleVertexes[8] , simpleVertexes[10], 5),
	//Edge(simpleVertexes[8] , simpleVertexes[11], 1),
	Edge(simpleVertexes[9] , simpleVertexes[11], 1),
	Edge(simpleVertexes[10], simpleVertexes[12], 1)
};
constexpr size_t simpleEdgeDFSSize = sizeof(simpleEdgeDFS) / sizeof(Edge);

/////////////////////
// Romania Problem //
/////////////////////

static Vertex romaniaVertexes[] = {
	"Arad",          // 0
	"Timisoara",     // 1
	"Zerind",        // 2
	"Oradea",        // 3
	"Lugoj",         // 4
	"Drobeta",       // 5
	"Mehadia",       // 6
	"Sibiu",         // 7
	"Rimnicu Vilcea",// 8
	"Craiova",       // 9
	"Fagaras",       // 10
	"Pitesti",       // 11
	"Giurgiu",       // 12
	"Bucharest",     // 13
	"Neamt",         // 14
	"Urziceni",      // 15
	"Iasi",          // 16
	"Vaslui",        // 17
	"Hirsova",       // 18
	"Eforie"         // 19
};
constexpr size_t romaniaVertexSize = sizeof(romaniaVertexes) / sizeof(Vertex);

static Edge romaniaEdgeFull[] = {
	// Arah deklarasi ke kanan
	// akan otomatis membuat sisi arah lainnya
	Edge(romaniaVertexes[0] , romaniaVertexes[1] , 118),
	Edge(romaniaVertexes[0] , romaniaVertexes[2] , 75 ),
	Edge(romaniaVertexes[0] , romaniaVertexes[7] , 140),
	Edge(romaniaVertexes[1] , romaniaVertexes[4] , 111),
	Edge(romaniaVertexes[2] , romaniaVertexes[3] , 71 ),
	Edge(romaniaVertexes[3] , romaniaVertexes[7] , 151),
	Edge(romaniaVertexes[4] , romaniaVertexes[6] , 70 ),
	Edge(romaniaVertexes[5] , romaniaVertexes[9] , 120),
	Edge(romaniaVertexes[6] , romaniaVertexes[5] , 75 ),
	Edge(romaniaVertexes[7] , romaniaVertexes[8] , 80 ),
	Edge(romaniaVertexes[7] , romaniaVertexes[10], 99 ),
	Edge(romaniaVertexes[8] , romaniaVertexes[9] , 146),
	Edge(romaniaVertexes[8] , romaniaVertexes[11], 97 ),
	Edge(romaniaVertexes[9] , romaniaVertexes[11], 138),
	Edge(romaniaVertexes[10], romaniaVertexes[13], 211),
	Edge(romaniaVertexes[11], romaniaVertexes[13], 101),
	Edge(romaniaVertexes[13], romaniaVertexes[12], 90 ),
	Edge(romaniaVertexes[13], romaniaVertexes[15], 85 ),
	Edge(romaniaVertexes[15], romaniaVertexes[17], 142),
	Edge(romaniaVertexes[15], romaniaVertexes[18], 98 ),
	Edge(romaniaVertexes[16], romaniaVertexes[14], 87 ),
	Edge(romaniaVertexes[17], romaniaVertexes[16], 92 ),
	Edge(romaniaVertexes[18], romaniaVertexes[19], 86 )
};
constexpr size_t romaniaEdgeFullSize = sizeof(romaniaEdgeFull) / sizeof(Edge);

static Edge romaniaEdgeDFS[] = {
	// Arah deklarasi ke kanan
	// akan otomatis membuat sisi arah lainnya
	Edge(romaniaVertexes[0] , romaniaVertexes[1] , 118),
	Edge(romaniaVertexes[0] , romaniaVertexes[2] , 75 ),
	Edge(romaniaVertexes[0] , romaniaVertexes[7] , 140),
	Edge(romaniaVertexes[1] , romaniaVertexes[4] , 111),
	Edge(romaniaVertexes[2] , romaniaVertexes[3] , 71 ),
	//Edge(romaniaVertexes[3] , romaniaVertexes[7] , 151),
	Edge(romaniaVertexes[4] , romaniaVertexes[6] , 70 ),
	Edge(romaniaVertexes[5] , romaniaVertexes[9] , 120),
	Edge(romaniaVertexes[6] , romaniaVertexes[5] , 75 ),
	//Edge(romaniaVertexes[7] , romaniaVertexes[8] , 80 ),
	Edge(romaniaVertexes[7] , romaniaVertexes[10], 99 ),
	//Edge(romaniaVertexes[8] , romaniaVertexes[9] , 146),
	//Edge(romaniaVertexes[8] , romaniaVertexes[11], 97 ),
	Edge(romaniaVertexes[9] , romaniaVertexes[11], 138),
	//Edge(romaniaVertexes[10], romaniaVertexes[13], 211),
	Edge(romaniaVertexes[11], romaniaVertexes[13], 101),
	Edge(romaniaVertexes[13], romaniaVertexes[12], 90 ),
	Edge(romaniaVertexes[13], romaniaVertexes[15], 85 ),
	Edge(romaniaVertexes[15], romaniaVertexes[17], 142),
	Edge(romaniaVertexes[15], romaniaVertexes[18], 98 ),
	Edge(romaniaVertexes[16], romaniaVertexes[14], 87 ),
	Edge(romaniaVertexes[17], romaniaVertexes[16], 92 ),
	Edge(romaniaVertexes[18], romaniaVertexes[19], 86 )
};
constexpr size_t romaniaEdgeDFSSize = sizeof(romaniaEdgeDFS) / sizeof(Edge);

////////////////////
// Number Problem //
////////////////////
#include "case3.hpp"

/////////////////////////
// Star System Problem //
/////////////////////////
#include "test_graph_stars.hpp"

/////////////////////////
// Problem Declaration //
/////////////////////////

Problem *Problem::getSimpleProblem()
{
	static Problem simple = {
		simpleVertexes,
		&simpleVertexes[0], // S
		&simpleVertexes[8], // G
		simpleEdgeFull,
		simpleEdgeDFS,
		simpleVertexSize,
		simpleEdgeSize,
		simpleEdgeDFSSize,
	};
	return &simple;
}

Problem *Problem::getRomaniaProblem()
{
	static Problem romania = {
		romaniaVertexes,
		&romaniaVertexes[0], // Arad
		&romaniaVertexes[13], // Bucharest
		romaniaEdgeFull,
		romaniaEdgeDFS,
		romaniaVertexSize,
		romaniaEdgeFullSize,
		romaniaEdgeDFSSize
	};
	return &romania;
}

Problem *Problem::getNumberProblem()
{
	static Problem number = {
		numberVertexes,
		&numberVertexes[30], // 30
		&numberVertexes[47], // 47
		numberEdges,
		numberEdges,
		numberVertexSize,
		numberEdgeSize,
		numberEdgeSize
	};
	return &number;
}

Problem *Problem::getStarSystemProblem()
{
	static Problem star = {
		starVertexes,
		&starVertexes[0], // Sol
		&starVertexes[371], // Delta Eridani
		starEdgeFull,
		starEdgeFull,
		starVertexSize,
		starEdgeSize,
		starEdgeSize
	};
	return &star;
}
