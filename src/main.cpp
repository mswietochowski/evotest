#include <iostream>

#include "graph.hpp"

#include <cmath>
#include <ctime>
#include <memory>
#include <algorithm>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

typedef std::auto_ptr<std::vector<Graph> > ptr;

double evaluate(const Graph *g);
double best(const ptr &population);
const Graph* bestg(const ptr &population);
double euclidean_distance(const Vertex &v1, const Vertex &v2);
void printgraph(const Graph *g);
void printgraph(const ptr &population);

ptr reproduce(ptr & population);
ptr genetic(ptr reproduced);
ptr succession(ptr population, ptr offsprings);

bool strictWeakOrderingOfGraphs(const Graph first, const Graph second);

const gsl_rng_type * T;
gsl_rng * r;

int main(int argc, char **argv)
{
    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    gsl_rng_set(r, time(NULL));

    Graph *g = new Graph();
	g->addVertex(Vertex(1, "v1", 0, 0, 0, 1.0, 1.0, 1.0));
	g->addVertex(Vertex(2, "v2", 0, 0, 0, 1.0, 0.2, 0.2));
	g->addVertex(Vertex(3, "v3", 0, 0, 0, 0.2, 1.0, 0.2));
	g->addVertex(Vertex(4, "v4", 0, 0, 0, 0.2, 0.2, 1.0));

	g->addEdge(1, 2);
	g->addEdge(1, 3);
	g->addEdge(3, 4);

	ptr population(new std::vector<Graph>(4, *g));
	/*
	std::cout << "Population" << std::endl;
	printgraph(population);

	ptr reproduced = reproduce(population);
	std::cout << "Reproduced" << std::endl;
	printgraph(reproduced);

	ptr offsprings = genetic(reproduced);
	std::cout << "Offsprings" << std::endl;
	printgraph(offsprings);

	population = succession(population, offsprings);
	std::cout << "After one iteration" << std::endl;
	printgraph(population);
	*/

	int iterationcount = 0;
	double bestv = best(population);
	double bestvold = bestv * 2;

	std::cout << "best " << bestv << "; bestvold " << bestvold << std::endl;
	while (fabs(bestvold - bestv)/bestvold > 0.00000001)
	{
		++iterationcount;
		ptr offsprings = genetic(reproduce(population));
		population = succession(population, offsprings);
		bestvold = bestv;
		bestv = best(population);
	}

	std::cout << "Possible result found after " << iterationcount << "iterations: " << std::endl;
	printgraph(population);

	const Graph *bestgraph = bestg(population);
	for (Graph::vertices_const_iterator i = bestgraph->vertices.begin(); i != bestgraph->vertices.end(); ++i)
	{
		const Vertex &v = i->second;
		std::cout << "\"" << v.name << "\"";
		std::cout << "\t@" << v.x << " " << v.y << " " << v.z;
		std::cout << "\tcolor" << v.r << " " << v.g << " " << v.b;
		std::cout << "\tfrozen light size 1.0" << std::endl;
	}

    gsl_rng_free (r);
	return 0;
}

double evaluate(const Graph *g)
{
	double result = 0.0;
	double exclusiveRadius = 1.0;

	for (Graph::vertices_const_iterator i = g->vertices.begin(); i != g->vertices.end(); ++i)
	{
		for (Graph::vertices_const_iterator j = i; j != g->vertices.end(); ++j)
		{
			double dist = euclidean_distance(i->second, j->second);
			//std::cout << "Dist(" << i->second.number << "," << j->second.number << "): " << dist << std::endl;

			// If vertices connected - they should be at specified distance
			if (g->mtx[i->second.number][j->second.number])
				result += fabs(3.0 - dist);
			// Otherwise, they should be at another distance
			else
				result += fabs(6.0 - dist);

			// Extra penalty for collisions
			if (dist < 2 * exclusiveRadius)
				result += 5 * (2 * exclusiveRadius - dist);
		}
	}
	return result;
}

double euclidean_distance(const Vertex &v1, const Vertex &v2)
{
	return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

void printgraph(const Graph *g)
{
	for (Graph::vertices_const_iterator i = g->vertices.begin(); i != g->vertices.end(); ++i)
	{
		const Vertex &v = i->second;
		std::cout << v.number << "\t" << v.name << "\t" << v.x << "\t" << v.y << "\t" << v.z << std::endl;
	}
	std::cout << "Evaluation: " << evaluate(g) << std::endl;
}

void printgraph(const ptr &population)
{
	std::cout << "== Printing population" << std::endl;
	int counter = 1;
	for (std::vector<Graph>::const_iterator i = population->begin(); i != population->end(); ++i)
	{
		std::cout << "Graph number " << counter << std::endl;
		printgraph(&(*i));
		++counter;
	}
}

ptr reproduce(ptr & population)
{
	return ptr(new std::vector<Graph>(*population));
}

ptr genetic(ptr reproduced)
{
	for (std::vector<Graph>::iterator i = reproduced->begin(); i != reproduced->end(); ++i)
	{
		for (Graph::vertices_iterator j = i->vertices.begin(); j != i->vertices.end(); ++j)
		{
			Vertex &v = j->second;
			v.x += gsl_ran_gaussian(r, 1.0);
			v.y += gsl_ran_gaussian(r, 1.0);
			v.z += gsl_ran_gaussian(r, 1.0);
		}
	}
	return reproduced;
}

ptr succession(ptr population, ptr offsprings)
{
	ptr newpopulation(new std::vector<Graph>());
	std::sort(population->begin(), population->end(), strictWeakOrderingOfGraphs);
	std::sort(offsprings->begin(), offsprings->end(), strictWeakOrderingOfGraphs);

	std::vector<Graph>::const_iterator i = population->begin(), j = offsprings->begin();

	while (i != population->end() && j != offsprings->end() && newpopulation->size() < population->size())
	{
		double v1 = evaluate(&(*i));
		double v2 = evaluate(&(*j));
		if (v1 <= v2)
		{
			newpopulation->push_back(*i);
			++i;
		}
		else
		{
			newpopulation->push_back(*j);
			++j;
		}
	}

	while (i != population->end() && newpopulation->size() < population->size())
		newpopulation->push_back(*(i++));
	while (j != offsprings->end() && newpopulation->size() < population->size())
		newpopulation->push_back(*(j++));

	return newpopulation;
}

bool strictWeakOrderingOfGraphs(const Graph first, const Graph second)
{
	double v1 = evaluate(&first);
	double v2 = evaluate(&second);
	return v1 < v2;
}

double best(const ptr &population)
{
	double best = evaluate(&(population->back()));
	for (std::vector<Graph>::const_iterator i = population->begin(); i != population->end(); ++i)
	{
		best = fmin(best, evaluate(&(*i)));
	}
	return best;
}

const Graph* bestg(const ptr &population)
{
	const Graph *result = &(population->back());
	double best = evaluate(result);
	double newb;
	for (std::vector<Graph>::const_iterator i = population->begin(); i != population->end(); ++i)
	{
		newb = evaluate(&(*i));
		if (newb < best)
		{
			best = newb;
			result = &(*i);
		}
	}
	return result;
}
