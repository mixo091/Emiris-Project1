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
    int k = 5, L = 5, N = 2, R = 10000, w = 400;
    int totalVectors = 0; //Total points in space
    int dimension = 0;
    int queryLines = 0, qrVectorDim;

    // Handling the args for The LSH. 
    int err_no = lsh_parse_args(argc, argv, &input_file,&qr_file, &out_file, &k, &L, &N, &R);
    if(err_no <= 0 ) {
        cerr << "Something wrong with the arguments!";
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
    Data<double> *qr_data = new Data<double>[queryLines];
    parseData(qr_file, qrVectorDim, qr_data);

    bool stop;
    do {
        // Execute query search
        lsh.ANN(qr_data, queryLines, dataset, totalVectors, N, out_file, R);

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

    return 0 ;
}




