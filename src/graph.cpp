#include "graph.hpp"

Vertex::Vertex(Vertex::key k, std::string _name, double _x, double _y, double _z)
	: number(k), name(_name), x(_x), y(_y), z(_z)
{
}

int Graph::maxVertexCount = MAX_VERTEX_COUNT;

Graph::Graph()
{
//	vertices = new std::map<Vertex::key, Vertex>();
	//	memset(&mtx, sizeof(Vertex::key), MAX_VERTEX_COUNT*MAX_VERTEX_COUNT, 0);
	for (unsigned i = 0; i < maxVertexCount; ++i)
		for (unsigned j = 0; j < maxVertexCount; ++j)
			mtx[i][j] = false;
}

bool Graph::addVertex(Vertex v)
{
	if (vertices.find(v.number) != vertices.end())
		return false;

	vertices[v.number] = v;
	return true;
}

bool Graph::addEdge(Vertex::key v1, Vertex::key v2)
{
	if (v1 == v2)
		return false;

	if (mtx[v1][v2])
		return false;

	mtx[v1][v2] = mtx[v2][v1] = true;
}

