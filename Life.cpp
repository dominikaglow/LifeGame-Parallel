/*
 * Life.cpp
 */

#include "Life.h"
#include "Alloc.h"
#include <iostream>
using namespace std;

Life::Life()
{
}

Life::~Life()
{
}

void Life::setRules(Rules *rules)
{
	this->rules = rules;
}

void Life::setSize(int size)
{
	try
	{
		this->size = size;
		this->size_1 = size - 1;
		this->size_1_squared = size_1 * size_1;
		// cout << "cells"<<endl;
		this->cells = tableAlloc(size);
		// cout << "cellsnext"<<endl;
		this->cellsNext = tableAlloc(size);
		// cout << "polution"<<endl;
		this->pollution = tableAlloc(size);
		// cout << "polutionnext"<<endl;
		this->pollutionNext = tableAlloc(size);
		// cout << "clearcells"<<endl;
		clearTable(cells, size);
		// cout << "clearcellsnext"<<endl;
		clearTable(cellsNext, size);
		// cout << "clearpolution"<<endl;
		clearTable(pollution, size);
		// cout << "clearpolutionnext"<<endl;
		clearTable(pollutionNext, size);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
		
}

int Life::getSize(){
	return size;
}

void Life::bringToLife(int row, int col)
{
	cells[row][col] = 1;
}

// nie zrownoleglac
int Life::liveNeighbours(int row, int col)
{
	return cells[row - 1][col] + cells[row + 1][col] + cells[row][col - 1] + cells[row][col + 1] +
		   cells[row - 1][col - 1] + cells[row - 1][col + 1] + cells[row + 1][col - 1] + cells[row + 1][col + 1];
}

int Life::getPollution(int row, int col)
{
	return pollution[row][col];
}

int Life::getCellState(int row, int col)
{
	return cells[row][col];
}

int **Life::cellsTable()
{
	return cells;
}

int **Life::pollutionTable()
{
	return pollution;
}

void Life::swapTables()
{
	int **tmp;

	tmp = cells;
	cells = cellsNext;
	cellsNext = tmp;

	tmp = pollution;
	pollution = pollutionNext;
	pollutionNext = tmp;
}

int Life::sumTable( int **table ) {
	int sum = 0;
	for ( int row = 1; row < size_1; row++ )
		for( int col = 1; col < size_1; col++ )
			sum += table[ row ][ col ];
	return sum;
}

void Life::beforeFirstStep() {
}

void Life::afterLastStep() {
}
