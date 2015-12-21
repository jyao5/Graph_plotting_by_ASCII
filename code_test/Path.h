#ifndef __PATH_H
#define __PATH_H
#include"Graph.h"
#include"Image.h"
#define Infinity 999999
#define Finished -1
#define Inf_plus_1 1000000
void computePath(Graph& obj_graph, int node) {//compute and draw least path from the "node"
	vector<Arc> arclist;
	vector<vector<int>> g = obj_graph.getGraph();
	int dimension = obj_graph.getN();
	vector<int> lastNode(dimension, -1);//-1 means no previous node

	//convert -1 to Infinity
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			if (g[i][j] == -1) {
				g[i][j] = Infinity;
			}
		}
	}

	//set direct connection to the node
	for (int i = 0; i < dimension; ++i)
		if (g[node][i] != Infinity)
			lastNode[i] = node;

	int min = Inf_plus_1;//min is greater than all the weight in the list
	int min_pos = -1;//no previous node yet
	while (1) {
		min = Inf_plus_1;
		min_pos = -1;
		for (int i = 0; i < dimension; i++) {
			if (g[node][i] != Finished  && g[node][i] < min) {//in unfinished nodes, if current arc's weight is lesser
				min = g[node][i];//record weight
				min_pos = i;//and position
			}
		}

		if (min_pos == -1) break;//if couldn't find such node, break

		for (int i = 0; i < dimension; i++) {
			if (g[node][min_pos] + g[min_pos][i] < g[node][i]) {//if from "node" thru the min_pos node to i gives lesser total weight than "direct" arc to i
				g[node][i] = g[node][min_pos] + g[min_pos][i];//update "direct" arc weight
				lastNode[i] = min_pos;//update i's previous node as min_pos
			}
		}
		g[node][min_pos] = Finished;//mark the node as finished
	}
	for (int i = 0; i < lastNode.size(); ++i) {
		if(lastNode[i] != -1)//if i has previous node, make an arc and push it into the list
			arclist.push_back(Arc(obj_graph.num2name(lastNode[i] + 1), obj_graph.num2name(i + 1), obj_graph.at(lastNode[i], i)));
	}
	Image y(obj_graph.getNodes());//use the original Graph object to draw the nodes
	y.draw(arclist);//use the new arclist to draw edges
	y.print();

}

#endif
