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
    int k = 5, L = 7, N = 1, R = 10000, w = 4000;
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
    calc_dimensions(&totalVectors, &dimension, &input_file);

    Data<double> dataset[totalVectors];
    parseData(input_file, dimension, dataset);

    // Initialise our lsh structure
    Lsh<double> lsh = Lsh<double>(L, totalVectors, dimension, k, w, dataset);

    // Read from query file
    calc_dimensions(&queryLines, &qrVectorDim, &qr_file);
    assert(qrVectorDim == dimension);
    
    Data<double> qr_data[queryLines];
    parseData(qr_file, qrVectorDim, qr_data);
    
    // Execute query search
    lsh.ANN(qr_data, queryLines, dataset, totalVectors, N, out_file);

    return 0 ;
}




