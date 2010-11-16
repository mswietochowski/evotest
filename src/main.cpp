#include <iostream>

#include "graph.hpp"

#include <cmath>

double evaluate(Graph *g);
double euclidean_distance(Vertex &v1, Vertex &v2);
void printgraph(Graph *g);

int main(int argc, char **argv)
{
	Graph *g = new Graph();
	g->addVertex(Vertex(1, "v1"));
	g->addVertex(Vertex(2, "v2"));
	g->addVertex(Vertex(3, "v3"));
	g->addVertex(Vertex(4, "v4"));

	g->addEdge(1, 2);
	g->addEdge(1, 3);
	g->addEdge(3, 4);

	std::cout << "Hello!" << std::endl;
	std::cout << "=== ITERATION init" << std::endl;
	printgraph(g);
	std::cout << "asdsda " << g->vertices.size() << " sad " << std::endl;
	return 0;
}

double evaluate(Graph *g)
{
	double result = 0.0;

	for (Graph::vertices_iterator i = g->vertices.begin(); i != g->vertices.end(); ++i)
	{
		for (Graph::vertices_iterator j = i; j != g->vertices.end(); ++j)
		{
			double dist = euclidean_distance(i->second, j->second);
			std::cout << "Dist(" << i->second.number << "," << j->second.number << "): " << dist << std::endl;
			if (g->mtx[i->second.number][j->second.number])
				result += fabs(3.0 - dist);
			else
				result += fmax(0.0, 3.0 - dist);
		}
	}
	return result;
}

double euclidean_distance(Vertex &v1, Vertex &v2)
{
	return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

void printgraph(Graph *g)
{
	for (Graph::vertices_iterator i = g->vertices.begin(); i != g->vertices.end(); ++i)
	{
		Vertex &v = i->second;
		std::cout << v.number << "\t" << v.name << "\t" << v.x << "\t" << v.y << "\t" << v.z << std::endl;
	}
	std::cout << "Evaluation: " << evaluate(g) << std::endl;
}

