#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "../Data/Data.h"
using namespace std;

int Handle_LSH_args ( 
    int argc, char **argv, 
    string *i_file, string *qr_file, string *o_file,
    int *k, int *L, int *N, int *R ) ;
void Calc_LSH_needs(int *tableSize, int *dim,string filename);
Data* parseData(string filename, int dim, int totalVectors);