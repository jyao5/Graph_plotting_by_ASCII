#include"Graph.h"
#include"Image.h"
#include"Path.h"
#include<stdlib.h>
int main() {
	Graph x;
	Image y(x.getNodes());
	y.draw(x.getArcs());
	y.print();
	string signal;
	do {
		cout << "enter:	1)node name for its minimum spanning tree or " << endl;
		cout << "	2) \"graph\" for original graph" << endl;
		cout << "	3)quit" << endl;
		cin >> signal;
		if (signal == "quit") break;
		
		else if (signal == "graph") {
			system("cls");
			y.print();
			continue;
		}
		else if (signal.size() >= 2) {
			cout << "invalid input" << endl;
			continue;
		}
		system("cls");
		computePath(x, x.name2num(signal) - 1);
	} while (signal != "quit");
	return 0;
}