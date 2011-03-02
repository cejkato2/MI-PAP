#include "2Darray.h"
#include <iostream>

void Array2D::initArray(int sizeX, int sizeY)
{
	a = new std::vector<std::vector<int> >(sizeX, std::vector<int>(sizeY, 0));
}

Array2D::Array2D()
{
	initArray(10, 10);
	sizeX = sizeY = 10;
}

Array2D::Array2D(int sizeX, int sizeY)
{
	initArray(sizeX, sizeY);
	this->sizeX = sizeX;
	this->sizeY = sizeY;
}

Array2D::~Array2D()
{
	delete a;
}

int Array2D::getValueAt(int i, int j)
{
	int tmp;

	tmp = (*a)[i][j];
	return tmp;
}

void Array2D::setValueAt(int val, int i, int j)
{
	(*a)[i][j] = val;
}

void Array2D::print()
{
	int sizeX, sizeY;

	sizeX = (*a).size();
	for (int i = 0; i < sizeX; i++) {
		sizeY = (*a)[i].size();
		for (int j = 0; j < sizeY; j++) {
			std::cout << (*a)[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Array2D::addRow(int numRows)
{
	int newSize = this->sizeY + numRows;
	int columns = this->sizeX;

	(*a).resize(newSize, std::vector<int>(columns, 0));
	this->sizeY = newSize;
}

void Array2D::addColumn(int numColumns)
{
	int numOfRows = this->sizeY;
	int newCount = this->sizeX + numColumns;

	for (int i = 0; i < numOfRows; i++) {
		(*a)[i].resize(newCount);
	}
	this->sizeX = newCount;
}
