#include "Grid.h"

Grid::Grid(int r, int c)
{
	rows = r; cols = c;
	grid = new int[r*c];
}

void Grid::resetGrid()
{
	for(int i=0;i<rows*cols;i++) grid[i]=0;
}

Grid::~Grid()
{
	delete [] grid;
}
