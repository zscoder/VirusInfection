#include "Random.h"

using namespace std;

#ifndef GRID_H
#define GRID_H //header guards
class Grid //stores large 2D grids
{
	private:
	int *grid; //flatten it into 1D grid
	int rows; 
	int cols;
	static const int INF = int(1e9);
	
	public:
	Grid(int rows, int cols);
	int getRows() const {return rows;}
	int getCols() const {return cols;}
	bool isValid(int x, int y) const {return (x>=0&&x<rows&&y>=0&&y<cols);} //check if the coordinates are in range
	int at(int x, int y) const {return (isValid(x,y)?grid[getCols()*x+y]:-INF);} //get the number at position (x,y)
	void update(int x, int y, int v){grid[getCols()*x+y]=v;}
	void add(int x, int y, int v){grid[getCols()*x+y]+=v;}
	void resetGrid();
	~Grid();
};
#endif
