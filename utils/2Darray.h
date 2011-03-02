#ifndef ARRAY_2D_H
#include <vector>
 
class Array2D{
public:

typedef std::vector<std::vector<int> > t_array2D; //type definition

Array2D();
Array2D(int sizeX,int sizeY);
~Array2D();
int getValueAt(int i,int j);
void setValueAt(int val,int i,int j);
void print();
void addRow(int numRows=1); //pridani radku
void addColumn(int numColumns=1);
//Array2D* getArray2D();

private:
t_array2D* a;

void initArray(int sizeX,int sizeY);

int sizeX;
int sizeY;
};

#endif
