#include "../include/BFS.hpp"
#include "../include/Dijkstra.hpp"
#include "../include/A_star.hpp"
#include "../include/draw.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;


//std::array<Raster::Location, 4> Raster::DIRS  {make_pair(1, 0), make_pair(0, -1),
//										 make_pair(-1, 0), make_pair(0, 1)};
/*
void printParent(int **parent, int height, int width) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			if (parent[i][j] == INT_MAX)
				cout << "M" << " ";
			else if (parent[i][j] >= width*height)
				cout << "@" << " ";
			else
				cout << parent[i][j] << " ";;
		cout << std::endl;
	}
}
*/
void printUsage(char* prog)
{
	cout << "usage: " << prog << " n, where n is a mode number. Modes: "
		<< "1 - raster loaded from stdin, 2 - raster auto-generated with given width, height, "
		<< "number of vertices and edges, 3 - benchmark mode." << endl;
}
/*
void runBFS(Raster* rptr, Raster::Location start, Raster::Location end)
{
	BFS bfs(rptr);
	bfs.shortestPath(start, end);
	vector<Raster::Location> shortest_path_1 = bfs.getPath();
	cout << "Shortest path BFS(inversed): " << endl
		<< "length: " << shortest_path_1.size() << endl;
	drawResult(rptr->getWidth(), rptr->getHeight(), rptr, shortest_path_1);
}*/

void runAlgorithms(Raster* rptr, bool benchmark, vector<int>* bench_info=nullptr)
{
	high_resolution_clock::time_point t1, t2;
	Raster::Location start, end;
	start = rptr->getStart();
	end = rptr->getEnd();
	int width = rptr->getWidth();
	int height = rptr->getHeight();
	BFS bfs(rptr);
	A_star a_star(rptr);
	Dijkstra dijkstra(rptr);

	cout << "Start point: " << start.first << ", " << start.second << endl;
	cout << "End point: " << end.first << ", " << end.second << endl;

	/* BFS */
	//bfs.shortestPath(start, end);
	//vector<Raster::Location> shortest_path_1 = bfs.getPath();
	//cout << "Shortest path BFS(inversed): " << endl
	//	<< "length: " << shortest_path_1.size() << endl;
	//drawResult(width, height, rptr, shortest_path_1);
	if (benchmark && bench_info!=nullptr)
	{
		t1 = high_resolution_clock::now();
		bfs.shortestPath(start, end);
		t2 = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(t2 - t1).count();
		bench_info->push_back(duration);
		cout << "BFS exec time: " << duration << endl;
	}
	else
		bfs.shortestPath(start, end);
	vector<Raster::Location> shortest_path_1 = bfs.getPath();
	cout << "Shortest path BFS(inversed): " << endl
		<< "length: " << shortest_path_1.size() << endl;
	drawResult(width, height, rptr, shortest_path_1);

	/* Dijkstra*/
	if (benchmark && bench_info != nullptr)
	{
		t1 = high_resolution_clock::now();
		dijkstra.shortestPath(start, end);
		t2 = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(t2 - t1).count();
		bench_info->push_back(duration);
		cout << "Dijkstra exec time: " << duration << endl;
	}
	else
		dijkstra.shortestPath(start, end);
	vector<Raster::Location> shortest_path_2 = dijkstra.getPath();
	cout << "Shortest path Dijkstra(inversed): " << endl
		<< "length: " << shortest_path_2.size() << endl;
	drawResult(width, height, rptr, shortest_path_2);

	/* A* */
	if (benchmark && bench_info != nullptr)
	{
		t1 = high_resolution_clock::now();
		a_star.shortestPath(start, end);
		t2 = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(t2 - t1).count();
		bench_info->push_back(duration);
		cout << "A* exec time: " << duration << endl;
	}
	else
		a_star.shortestPath(start, end);
	vector<Raster::Location> shortest_path_3 = a_star.getPath();
	cout << "Shortest path A*(inversed): " << endl
		<< "length: " << shortest_path_3.size() << endl;
	drawResult(width, height, rptr, shortest_path_3);
}

void fromStdinMode()
{
	int width, height;
	pair<int, int> start, end;
	Raster* rptr = nullptr;

	while(cin.good())
	{
		cin >> width >> height;
		rptr = new Raster(width, height);
		if (!(rptr->readGrid()) || !(rptr->readStartAndEnd()))
		{
			delete rptr;
			exit(1);
		}
		runAlgorithms(rptr,false);
		delete rptr;
		rptr = nullptr;
	}
	//make sure memory is deallocated when cin error occurs
	delete rptr;
}

void autoGeneratedMode(bool benchmark, vector<int>* bench_info=nullptr)
{
	int tests, width, height, vertices, edges;
	pair<int, int> start, end;
	Raster* rptr = nullptr;
	cout << "number of tests:" << endl;
	cin >> tests;
	while (tests > 0)
	{
		cout << "raster width:" << endl;
		cin >> width;
		cout << "raster height:" << endl;
		cin >> height;
		cout << "number of vertices:" << endl;
		cin >> vertices;
		cout << "number of edges:" << endl;
		cin >> edges;
		rptr = new Raster(width, height);
		rptr->generateGrid(vertices, edges);
		if (benchmark && bench_info != nullptr)
		{
			bench_info->push_back(vertices);
			bench_info->push_back(edges);
		}
		runAlgorithms(rptr, benchmark, bench_info);

		delete rptr;
		--tests;
	}
}

void benchmarkMode()
{
	vector<int> results, edges, vertices,
		bfs_times, dijkstra_times, a_star_times;
	vector<double> bfs_q, dijkstra_q, a_star_q;

	autoGeneratedMode(true, &results);

	char fill = ' ';
	int width = 10;
	int v, e, median, time_bfs, time_dijkstra, time_a_star, q,
		t_med, q_med, v_med, e_med, T;
	double T_med;

	// organize benchamrk results into buckets
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		vertices.push_back(*it++);
		edges.push_back(*it++);
		bfs_times.push_back(*it++);
		dijkstra_times.push_back(*it++);
		a_star_times.push_back(*it);
	}
	int no_test = vertices.size();

	// find median index
	if (no_test == 1)
		median = 0;
	else if (no_test % 2 == 0)
		median = (no_test / 2) - 1;
	else
		median = no_test / 2;

	v_med = vertices[median];
	e_med = edges[median];
	cout << "median: " << median
		<< " v_med: " << v_med
		<< " e_med: " << e_med << endl;

	/*
	q = (t(n) * T(n_median) / (T(n) * t(n_median))
	*/

	bfs_q.resize(vertices.size());
	dijkstra_q.resize(vertices.size());
	a_star_q.resize(vertices.size());

	// BFS - O(V+E)
	T_med = v_med + e_med;
	t_med = bfs_times[median];
	double constant = T_med / (double)t_med;

	bfs_q[median] = 1.0;
	for (int i = 0; i < no_test; ++i)
	{
		if (i == median) continue;
		T = vertices[i] + edges[i];
		q = ((double)bfs_times[i]) * constant;
		bfs_q[i] = q / T;
	}

	// Dijkstra - O(E*logV)
	T_med = (double)e_med * log((double)v_med);
	t_med = dijkstra_times[median];
	constant = T_med / (double)t_med;

	dijkstra_q[median] = 1.0;
	for (int i = 0; i < no_test; ++i)
	{
		if (i == median) continue;
		T = (double)edges[i] * log((double)vertices[i]);
		q = (double)dijkstra_times[i] * constant;
		bfs_q[i] = q / T;
	}

	// A* - O(E)
	T_med = e_med;
	t_med = a_star_times[median];
	constant = T_med / (double)t_med;

	a_star_q[median] = 1.0;
	for (int i = 0; i < no_test; ++i)
	{
		if (i == median) continue;
		T = edges[i];
		q = (double)a_star_times[i] * constant;
		a_star_q[i] = q / T;
	}

	char* alg_names[3] = { "BFS O(V+E)" , "Dijkstra O(E*logV)" , "A* O(E)" };

	for (int k = 0; k < 3; ++k)
	{
		cout << endl << alg_names[k] << endl;
		cout << left << setw(width) << setfill(fill) << "V";
		cout << left  << setw(width) << setfill(fill) << "E";
		cout << left  << setw(width) << setfill(fill) << "t(n)[ms]";
		cout << left  << setw(width) << setfill(fill) << "q(n)" << endl;
		for (int i = 0; i < no_test; ++i)
		{
			cout << std::left << setw(width) << setfill(fill) << vertices[i];
			cout << std::left << setw(width) << setfill(fill) << edges[i];
			if(k==0)
				cout << std::left << setw(width) << setfill(fill) << bfs_times[i];
			else if(k==1)
				cout << std::left << setw(width) << setfill(fill) << dijkstra_times[i];
			else
				cout << std::left << setw(width) << setfill(fill) << a_star_times[i];
			if(k==0)
				cout << std::left << std::fixed << setw(width) << setfill(fill) << bfs_q[i] << endl;
			else if(k==1)
				cout << std::left << std::fixed << setw(width) << setfill(fill) << dijkstra_q[i] << endl;
			else
				cout << std::left << std::fixed << setw(width) << setfill(fill) << a_star_q[i] << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printUsage(argv[0]);
		exit(1);
	}

	pair<int, int> start, end;
	Raster* rptr = nullptr;

	string mode = argv[1];

	if (mode == "1")
	{
		fromStdinMode();
		return 0;
	}
	else if (mode == "2")
	{
		autoGeneratedMode(false);
		return 0;
	}
	else if (mode == "3")
	{
		benchmarkMode();
		return 0;
	}
	else
	{
		printUsage(argv[0]);
		exit(0);
	}

	delete rptr;
	return 0;
}
