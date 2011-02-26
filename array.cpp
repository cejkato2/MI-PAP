#include "array.h"
#include <iostream>


Array::Array()
{
	capacity = 10;
	data = new int[capacity];
	size = 0;
}

Array::~Array()
{
	capacity = 10;
	size = 0;
	delete [] data;
}

int Array::getSize()
{
	return size;
}

int *Array::getData()
{
	return data;
}

void Array::addValue(int v)
{
	if (size == capacity + 1) {
		int *temp = new int[capacity * 2];
		for (int i = 0; i < size; ++i) {
			temp[i] = data[i];
		}
		capacity *= 2;
		delete [] data;
		data = temp;
	}
	data[size++] = v;
}

void Array::print()
{
	for (int i = 0; i < size; i++) {
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;
}


