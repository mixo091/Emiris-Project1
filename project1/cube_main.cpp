#include <iostream>

#include "./hypercube/hypercube.hpp"
#include "./Utilities/Utilities.hpp"
#include "./Data/Data.hpp"

int main(int argc, char **argv) {
    std::string input_f, query_f, output_f;
    // default values
    int k = 14, M = 10, probes = 2, N = 1, R = 10000;
    int w = 400;
    // vector dim
    int vectorDim = 0;
    // total vectors
    int totalVectors = 0;
    int queryLines = 0, qrVectorDim = 0;

    // parse arguments
    arg_handling(argc, argv, 
                &input_f, &query_f, &output_f,
                &k, &M, &probes, &N, &R);

    // get the dimensions
    calc_dimensions(&totalVectors, &vectorDim, &input_f);

    // dataset structure
    Data<double> dataset[totalVectors];
    parseData(input_f, vectorDim, dataset);
    // create dataset
    // Data<double> *dataset = parseData(input_f, vectorDim, totalVectors);

    // create hypercube 
    hypercube<double> cube = hypercube<double>(probes, M, w, k, vectorDim, totalVectors, dataset);

    // get query lines and vector dimensions
    calc_dimensions(&queryLines, &qrVectorDim, &query_f);

    Data<double> qr_data[queryLines];
    parseData(query_f, qrVectorDim, qr_data);

    // execute query search
    cube.ANN(qr_data, queryLines, dataset, totalVectors, N, output_f);

    return 0;
}