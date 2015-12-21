#ifndef __Graph_h
#define __Graph_h
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<iostream>
#include<map>
#include<iomanip>

using namespace std;
class Arc{//structure: string "from" -> [int weight] -> string "to"
public:
	string from;
	string to;
	int weight;
	Arc(string f, string t, int w) :from(f), to(t), weight(w) {}
};

class Graph{
private:
	vector<vector<int>> graph;//graph of DAG, row stands for originating node, col terminating node
	map<string, int> nodes;//map nodes to matrix coordinates, so that node name does not have to be contiguous
	int numOfNode;//number of nodes
public:
	int getN() { return numOfNode; }//get node number
	Graph();//read "graph.dat" and, map node name to coordinates, initialize the graph
	vector<vector<int>> getGraph() { return graph; }//get the matrix
	map<string, int> getNodes();//get the mapping
	vector<Arc> getArcs();//get the array of arcs for plotting
	int at(int row, int col);//accessing single edge
	string num2name(int num);//map coordinates to names
	int name2num(string name) { return nodes[name]; }//map name to coordinates
	void print();//print graph for testing
};

string Graph::num2name(int num) {
	string buffer;
	for_each(nodes.begin(), nodes.end(), [&](pair<string, int> x) {
		if (x.second == num)
			buffer = x.first;
	});
	return buffer;
}

Graph::Graph() {
	ifstream in("graph.dat");
	stringstream buffer;
	string buffer_string;
	string buffer_word;
	int counter = 1;
	//pre-read the file, find all nodes
	while (getline(in, buffer_string)) {//read one line from the file
		buffer << buffer_string;//convert to sstream type
		buffer >> buffer_word;//read in the starting node
		if (nodes.find(buffer_word) == nodes.end())//if it's new nodes, add it in to the map
			nodes[buffer_word] = counter++;
		while (buffer >> buffer_word) {//read in all terminating nodes
			if (nodes.find(buffer_word) == nodes.end())//if it's new, add it to the map
				nodes[buffer_word] = counter++;
			buffer >> buffer_word;
		}
		buffer_string.clear();
		buffer.clear();
	}
	numOfNode = nodes.size();
	graph.resize(nodes.size(), vector<int>(nodes.size(), -1));//initialize graph by the number of the nodes, -1 means no connection
	in.close();
	in.open("graph.dat");
	int buffer_int;
	string buffer_row;
	while (getline(in, buffer_string)) {//re read the file, add each edge to the graph
		buffer << buffer_string;
		buffer >> buffer_row;
		while (buffer >> buffer_word) {
			buffer >> buffer_int;
			graph[nodes[buffer_row] - 1][nodes[buffer_word] - 1] = buffer_int;
		}
		buffer_string.clear();
		buffer.clear();
	}

}

void Graph::print() {
	cout << setw(4) << " ";
	for_each(nodes.begin(), nodes.end(), [&](pair<string, int> row) {//print col number
		cout << setw(4) << row.first;
	});
	cout << endl;
	for_each(nodes.begin(), nodes.end(), [&](pair<string, int> row) {
		cout << setw(4) << row.first;//print row number
		for_each(nodes.begin(), nodes.end(), [&](pair<string, int> col) {
			cout << setw(4) << graph[row.second - 1][col.second - 1];
		});
		cout << endl;
	});
}

map<string, int> Graph::getNodes(){
	return nodes;
}

int Graph::at(int from, int to) {
	return graph[from][to];
}

vector<Arc> Graph::getArcs(){
	vector<Arc> arclist;
	for_each(nodes.begin(), nodes.end(), [&](pair<string, int> row) {
		for_each(nodes.begin(), nodes.end(), [&](pair<string, int> col) {
			if (graph[row.second - 1][col.second - 1] != -1)//each non -1 cell is an arc
				arclist.push_back(Arc(row.first, col.first, graph[row.second - 1][col.second - 1]));
		});
	});
	return arclist;
}



#endif
