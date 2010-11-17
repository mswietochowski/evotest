#include <map>
#include <vector>
#include <list>
#include <string>

#define MAX_VERTEX_COUNT 10

struct Vertex
{
	typedef int key;
	key number;
	std::string name;
	double x, y, z;
	double r, g, b;
	Vertex() { };
	Vertex(Vertex::key k, std::string _name, double _x = 0.0, double _y = 0.0, double _z = 0.0, double r = 255, double g = 255, double b = 255);
};

struct Graph
{
	Graph();
	bool addVertex(Vertex v);
	bool addEdge(Vertex::key v1, Vertex::key v2);

	std::map<Vertex::key, Vertex> vertices;
	typedef std::map<Vertex::key, Vertex>::iterator vertices_iterator;
	typedef std::map<Vertex::key, Vertex>::const_iterator vertices_const_iterator;
	bool mtx[MAX_VERTEX_COUNT][MAX_VERTEX_COUNT];

	static int maxVertexCount;
};

