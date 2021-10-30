#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>

#include "./Utilities/Utilities.hpp"
#include "./LSH/lsh.hpp"
#include "./HashTable/HashTable.hpp"

using namespace std;
int main(int argc, char **argv){

    string input_file, out_file, qr_file;
    int k = 4, L = 5, N = 1, R = 10000, w = 4;
    int totalVectors = 0; //Total points in space
    int dimension = 0;
    int queryLines = 0, qrVectorDim;
    std::cout <<"Project starts..."<<endl;

    // Handling the args for The LSH. 
    int err_no = Handle_LSH_args(argc, argv, &input_file,&qr_file, &out_file, &k, &L, &N, &R);
    if(err_no <= 0 ) {
        cerr << "Something wrong with arguments!";
        exit(err_no);
    } 

    // Get the dimension of a vector and the total amount of data
    Calc_LSH_needs(&totalVectors, &dimension, input_file);

    // Get the data(points) given
    Data<double> *dataset = parseData(input_file, dimension, totalVectors);
    
    // Initialise our lsh structure
    Lsh<double> lsh = Lsh<double>(L, totalVectors, dimension, k, w, dataset);

    // Read from query file
    Calc_LSH_needs(&queryLines, &qrVectorDim, qr_file);
    assert(qrVectorDim == dimension);
    
    // Store query data
    Data<double> *qr_data = parseData(qr_file, qrVectorDim, queryLines);
    
    // Execute query search
    lsh.ANN(qr_data, queryLines, dataset, totalVectors, N, out_file);

    return 0 ;
}




