#ifndef LOADER_H

void getFilename(char **filename, int argc, char **argv);
void loadValues(char *filename, Array &array);
//TODO tomas - dodelam jeste loadValues do Array2D
//TODO zmenit pouziti Array2D z ukazatele na referenci?
//void loadValues(char *filename, Array2D);

#endif
