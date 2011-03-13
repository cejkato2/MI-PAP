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
	capacity = 0;
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
	if ((size == (capacity + 1)) || (size == capacity)) {
                if (capacity == 0) {
                  capacity = 10;
                } else {
                  capacity *= 2;
                }
		int *temp = new int[capacity];
		for (int i = 0; i < size; ++i) {
			temp[i] = data[i];
		}
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

bool Array::isEmpty()
{
	return (size == 0);
}


