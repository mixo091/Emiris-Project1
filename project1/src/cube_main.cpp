#include <iostream>

#include "./hypercube/hypercube.hpp"
#include "./Utilities/Utilities.hpp"
#include "./Data/Data.hpp"

int main(int argc, char **argv) {
    std::string input_file, qr_file, out_file;
    // default values
    int k = 14, M = 10, probes = 4, N = 2, R = 10000;
    int w = 400;
    // vector dim
    int vectorDim = 0;
    // total vectors
    int totalVectors = 0;
    int queryLines = 0, qrVectorDim = 0;

    // parse arguments
    cube_parse_args(argc, argv, 
                &input_file, &qr_file, &out_file,
                &k, &M, &probes, &N, &R);

    // get the dimensions
    calc_dimensions(&totalVectors, &vectorDim, &input_file);
    Data<double> dataset[totalVectors];
    parseData(input_file, vectorDim, dataset);

    // create hypercube 
    hypercube<double> cube = hypercube<double>(probes, M, w, k, vectorDim, totalVectors, dataset);

    // get query lines and vector dimensions
    calc_dimensions(&queryLines, &qrVectorDim, &qr_file);
    Data<double> *qr_data = new Data<double>[queryLines];
    parseData(qr_file, qrVectorDim, qr_data);

    bool stop;
    do {
        // execute query search
        cube.ANN(qr_data, queryLines, dataset, totalVectors, N, out_file, R);

        string choice;
        cout << "Would you like to continue (Y/N)?   ";
        do {
            cin >> choice;
        } while(choice != "Y" && choice != "N");

        // free existing query dataset and create new one if asked
        delete [] qr_data;

        stop = (choice == "Y");

        if(stop) {
            cout << "New query file:    "; cin >> qr_file;

            queryLines = 0; qrVectorDim = 0;
            calc_dimensions(&queryLines, &qrVectorDim, &qr_file);
            qr_data = new Data<double>[queryLines];
            parseData(qr_file, qrVectorDim, qr_data);
        }
    } while(stop);

    return 0;
}