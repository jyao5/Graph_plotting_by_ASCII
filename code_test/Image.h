#ifndef __Image_h
#define __Image_h

#include"Graph.h"

class Image {
private:
	vector<vector<char>> bitmap;//store the ascii art
	int row;//dimension of the bitmap
	int col;
	map<string, int> nodes;//for mapping names to contiguous integers
	map<string, pair<int,int>> nodePos;//store location of one node in the bitmap
public:
	Image(map<string, int>&);//initialize the bitmap and place the nodes by the mapping information from Graph object
	void draw(vector<Arc>&);//draw the edges according the the arclist from Graph object
	void single_stroke(Arc);//subroutine of draw()
	void probe(int, int, int, int);//subrouting of single_stroke, for probing possible route for a stroke
	void print();
};

Image::Image(map<string, int>& obj_node) {
	row = (obj_node.size() + 1) * 8 + 1;//two nodes are 7 lines away vertically
	col = 71;//width of the bit map is 71 characters considering most console windows are not full
	bitmap.resize(row, vector<char>(71, ' '));
	auto iter = obj_node.begin();
	for (int i = 4; i < row; i += 8) {
		nodePos[(*iter).first] = make_pair(i, 36);//node name is place in the middle, of at most 2 chars long
		bitmap[i][34] = '[';//wrapped by square brackets, leave two cells in between for node name placement
		bitmap[i][37] = ']';
		if ((*iter).first.size() == 2) {//if name is of two char, fill them up 
			bitmap[i][36] = (*iter).first[1];
			bitmap[i][35] = (*iter).first[0];
		}
		else if ((*iter).first.size() == 1) {//if has only one, file the right cell
			bitmap[i][36] = (*iter).first[0];
			bitmap[i][35] = ' ';
		}
		else
			cout << "can't draw node!" << endl;
		iter++;
		if (iter == obj_node.end()) break;
	}
}

void Image::print() {
	for (int i = 0; i < 80; i++)
		cout << '-';//delimit the edge the drawing
	cout << endl;

	for_each(bitmap.begin(), bitmap.end(), [](vector<char> x) {
		for_each(x.begin(), x.end(), [](char y) {
			cout << y;
		});
		cout << endl;
	});

	for (int i = 0; i < 80; i++)
		cout << '-';//delimit the edge the drawing
	cout << endl;
}

void Image::single_stroke(Arc arc) {
	int from_r = nodePos[arc.from].first;//row number of the starting node in the ascii art
	int to_r = nodePos[arc.to].first;	//row ending node
	int from_c = nodePos[arc.from].second;//column starting
	int to_c = nodePos[arc.to].second;	//columm ending

	//draw incoming arrow to the ending node if non drawn yet
	if (from_r < to_r) {//if starting node is above the ending node
		if (bitmap[to_r - 1][to_c] == ' ') {//if no arrow made to the node 
			bitmap[to_r - 1][to_c] = 'V';//draw arrow pointing down to the right cell of the two-celled name
			bitmap[to_r - 2][to_c] = '|';
			bitmap[to_r - 3][to_c] = '+';//allong with it's 90 degree bend
			
		}
		to_r -= 3;
	}
	else if (from_r > to_r) {//starting below ending node
		if (bitmap[to_r + 1][to_c] == ' ') {//if no arrow made to the node 
			bitmap[to_r + 1][to_c] = '^';//draw arrow pointing up to the node
			bitmap[to_r + 2][to_c] = '|';
			bitmap[to_r + 3][to_c] = '+';
			
		}
		to_r += 3;
	}

	//draw outgoing horizontal line from the starting point if non drawn yet
	if (from_r > to_r) {//if starting node is below the ending, the line goes left(then up)
		if (bitmap[from_r][from_c - 3] == ' ') {
			bitmap[from_r][from_c - 3] = '-';
			bitmap[from_r][from_c - 4] = '-';//leave one more '-' for convenience of placing weight
		}
		from_c -= 5;//place the cursor at the first space to the left of the line

	}
	else if (from_r < to_r) {//if starting node is beyond the ending, the line goes right(then down)
		if (bitmap[from_r][from_c + 2] == ' ') {
			bitmap[from_r][from_c + 2] = '-';
			bitmap[from_r][from_c + 3] = '-';
		}
		from_c += 4;//since name takes two cells, so this is not symetric
	}

	//probe and draw vertical line, then connected horizontally to the 90 bend of the ending node
	probe(from_r, from_c, to_r, arc.weight);
	

}
void Image::probe(int Row, int Col, int to_row, int Weight) {
	int i;
	//probing
	if (Row < to_row) {//ending point is below, so probe rightward and draw downward
		do {//probing
			if (bitmap[Row][Col] == ' ')//if the column to be probed has no line connected
				bitmap[Row][Col] = '-';
			else if(bitmap[Row][Col] == '|')//if the column to be probed has vertical line
				bitmap[Row][Col] = '=';//cross over it

			for (i = Row + 1; i <= to_row; ++i) {
				if (bitmap[i][Col] == '|')//probe the column of of this position to the destination row
											//if a vertical line exist, break
					break;
			}

			if (i == to_row + 1) {//if no vertical line encoutered during the probing
				bitmap[Row][Col] = '+'; //starting from here, draw a line down, so '-' becomes a bend
				bool flag = false;//used for weight placing
				for (i = Row + 1; i < to_row; ++i) {
					if (bitmap[i][Col] == ' ') {//if no horizontal lines encountered
						if (flag == false) {//if no weight has been placed yet
							if (bitmap[i][Col - 1] == ' ' && bitmap[i][Col + 1] == ' ') {
								for (int j = 0; j < 3; ++j) {
									bitmap[i][Col + 1 - j] = Weight % 10 + '0';//all weight is force to be three chars long
									Weight /= 10;
								}
							}
							flag = true;//only write weight once
						}
						else
							bitmap[i][Col] = '|';//place vertical line
					}
					else if (bitmap[i][Col] == '-')//if meet a horizontal line
						bitmap[i][Col] = '=';//cross over it(or more precisely, be crossed over by it)
				}//at this point, vertical line is drawn
				bitmap[i][Col] = '+';//now i == to_row, so, place a bend at the ending horizontal line
				while (bitmap[i][Col - 1] != '+') {//draw leftward till encounter the first bend(made by other arc pointing to this node)
					if (bitmap[i][Col - 1] == '|')
						bitmap[i][--Col] = '=';
					else
						bitmap[i][--Col] = '-';
					if (Col <= 0) break;
				}
				break;
			}
			else {//if encounter a vertical line during the probing
				for (int k = 0; k < 3; ++k) {//draw three line rightward
					Col++;
					if (bitmap[Row][Col] == ' ')
						bitmap[Row][Col] = '-';
				}
				Col++;//and place the cursor a cell left of the tip
			}
		} while (Col <= col);//cursor can't exceed the right bound
	}

	else {//ending point is above, so probe leftward and upward, same structure as the one above
		do {
			if (bitmap[Row][Col] == ' ')
				bitmap[Row][Col] = '-';
			else if (bitmap[Row][Col] == '|')
				bitmap[Row][Col] = '=';

			for (i = Row - 1; i >= to_row; --i) {
				if (bitmap[i][Col] == '|')
					break;
			}

			if (i == to_row - 1) {
				bitmap[Row][Col] = '+';
				bool flag = false;
				for (i = Row - 1; i > to_row; --i) {
					if (bitmap[i][Col] == ' ') {
						if (flag == false) {
							if (bitmap[i][Col - 1] == ' ' && bitmap[i][Col + 1] == ' ') {
								for (int j = 0; j < 3; ++j) {
									bitmap[i][Col + 1 - j] = Weight % 10 + '0';
									Weight /= 10;
								}
							}
							flag = true;
						}
						else
							bitmap[i][Col] = '|';
					}
					else if (bitmap[i][Col] == '-')
						bitmap[i][Col] = '=';
				}
				bitmap[i][Col] = '+';
				while (bitmap[i][Col + 1] != '+') {
					if (bitmap[i][Col + 1] == '|')
						bitmap[i][++Col] = '=';
					else
						bitmap[i][++Col] = '-';
				}
				break;
			}
			else {
				for (int k = 0; k < 3; ++k) {
					Col--;
					if (bitmap[Row][Col] == ' ')
						bitmap[Row][Col] = '-';

				}
				Col--;
			}
		} while (Col >= 0);//can't exceed the left bound
	}

}
void Image::draw(vector<Arc>& x){
	for_each(x.begin(), x.end(), [&](Arc y) {
		single_stroke(y);
	});
}

#endif