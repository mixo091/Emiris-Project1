#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>

#include "./Utilities/Utilities.hpp"
#include "./LSH/lsh.hpp"
//#include "./VectorList/VectorList.h"

using namespace std;
int main(int argc, char **argv){

    string input_file, out_file, qr_file;
    int k = 4, L = 5, N = 1, R = 10000, w = 4;
    int totalVectors = 0; //Total points in space
    int dimension = 0;
    std::cout <<"Project starts..."<<endl;
    /*Space is a list of vectors that represents 
    the points at the Space with dimension <Dimension> */ 
    //Handling the args for The LSH. 
    int err_no = Handle_LSH_args(argc, argv, &input_file,&qr_file, &out_file, &k, &L, &N, &R);
    if(err_no <= 0 ) {
        cerr << "Something wrong with arguments!";
        exit(err_no);
    } 

    // Get the dimension of a vector and the total amount of data
    Calc_LSH_needs(&totalVectors, &dimension, input_file);

    // cout << "dimension = " << dimension << "vectors are " << totalVectors << endl;

    //Get the data(points) given
    Data<double> *dataset = parseData(input_file, dimension, totalVectors);
    for(int i = 0; i < totalVectors; i++)
        dataset[i].printVector();
    
    // Lsh<double> lsh = Lsh<double>(L, totalVectors, dimension, k, w, dataset);

    return 0 ;
}




