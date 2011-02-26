#ifndef ARRAY_H
class Array {
  int capacity;
  int size;
  int *data;
public:
  Array();
  ~Array();
  int getSize();
  int *getData();
  void addValue(int v);
  void print();
};
#endif
