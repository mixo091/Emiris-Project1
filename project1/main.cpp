#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>
#include "./HashTable/HashTable.hpp"
#include "./Utilities/Utilities.hpp"
#include "./LSH/lsh.hpp"

using namespace std;

//$./lsh –i <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν
//<number of nearest> -R <radius>


int main(int argc, char **argv){
    cout<<"Lsh algorithm implementation ..."<<endl;
    string input_file, out_file, qr_file;
    int k = 6, L = 5, N = 5, R = 10000, w = 1000;
    int totalVectors = 0; //Total points in space.
    int dimension = 0;    //Space Dimension.
    int queryLines = 0, qrVectorDim;
    //=== 1)Handle the args for The LSH. ===// 
    int err_no = Handle_LSH_args(argc, argv, &input_file,&qr_file, &out_file, &k, &L, &N, &R);
    if(err_no <= 0 ) {
        cerr << "Something wrong with arguments!";
        exit(err_no);
    } 
    //=== 2) Get data from the input File. ===//
    Calc_LSH_needs(&totalVectors, &dimension, input_file);
    Data<double> *dataset = parseData(input_file, dimension, totalVectors);
            //printSet(dataset,totalVectors); //Lets print the DataSet to test it.
    //-------------------------------------------------//
    //=== 3) Initialize our LSH structure - Insert Data. ===//
    Lsh<double> lsh = Lsh<double>(L, totalVectors, dimension, k, w, dataset);
    //=== 4) Ckeck if evrything is expressed right through LSH. ===//
            //lsh.PrintHTs();
    //-------------------------------------------------//
    //=== 5) Read from query file - Store query data . ===//
    Calc_LSH_needs(&queryLines, &qrVectorDim, qr_file);
    cout<<qrVectorDim<<"  "<<dimension<<endl;
    assert(qrVectorDim == dimension);
    Data<double> *qr_data = parseData(qr_file, qrVectorDim, queryLines);
            printSet(qr_data,queryLines); //print Set to ckeck it.
    //-------------------------------------------------//
    //=== 6) Execute Nearest Neighbor Search. ===//
    cout<<"N = "<<N<<endl;
    lsh.ANN(qr_data, queryLines, dataset, totalVectors, N, out_file);
    //------------------------------------------------//
    return 0 ;
}




