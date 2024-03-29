/**
 * Finding shortest path between points on square grid.
 *
 * @file AlgorithmBase.cpp
 * @brief Definitions of base classes for path-finding algorithms.
 * @author Wiktor Franus
 */

#include "../include/AlgorithmBase.hpp"
#include <iostream>

AlgorithmBase::AlgorithmBase(Raster* raster_) : raster(raster_)
{
	parent = new Location*[raster->getHeight()];
	for (int i = 0; i < raster->getHeight(); ++i)
	{
		parent[i] = new Location[raster->getWidth()];
		std::fill(parent[i], parent[i] + raster->getWidth(), nullPair);
	}
}

AlgorithmBase::~AlgorithmBase()
{
	for (int i = 0; i < raster->getHeight(); ++i)
		delete[] parent[i];
	delete[] parent;
}

vector<AlgorithmBase::Location> AlgorithmBase::getPath()
{
	vector<Location> path;
	Location current = raster->getEnd();
	while (current != raster->getStart()) {
		path.push_back(current);
		if (current.first == nullPair.first || current.second == nullPair.second)
		{
			std::cout << "AlgorithmBase::getPath() error: wrong parent detected!" << std::endl;
			return vector<Location>();
		}
		current = parent[current.first][current.second];
	}
	//path.push_back(raster->getStart()); // optional, not preffered when drawing image
	return path;

}

void AlgorithmBase::clear()
{
	for (int i = 0; i < raster->getHeight(); ++i)
		std::fill(parent[i], parent[i] + raster->getWidth(), nullPair);
}


AlgorithmWithPriorityQueue::AlgorithmWithPriorityQueue(Raster * raster_) : AlgorithmBase(raster_)
{
	distance = new int*[raster->getHeight()];
	for (int i = 0; i < raster->getHeight(); ++i)
	{
		distance[i] = new int[raster->getWidth()];
		std::fill(distance[i], distance[i] + raster->getWidth(), INT_MAX);
	}
}

AlgorithmWithPriorityQueue::~AlgorithmWithPriorityQueue()
{
	for (int i = 0; i < raster->getHeight(); ++i)
		delete[] distance[i];
	delete[] distance;
}

void AlgorithmWithPriorityQueue::clear()
{
	for (int i = 0; i < raster->getHeight(); ++i)
		std::fill(distance[i], distance[i] + raster->getWidth(), INT_MAX);
	AlgorithmBase::clear();
}
