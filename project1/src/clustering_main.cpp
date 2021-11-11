#include <iostream>
#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>
#include "./Clustering/ClusteringUtilities.hpp"
#include "./Clustering/Clustering.hpp"
using namespace std;

int main(int argc, char **argv){
    string input_file, out_file, qr_file;
    int totalVectors = 0; //Total points in space.
    int dimension = 0;    //Space Dimension.
    input_file = "./test_files/input_xs";
    int ClustersNum = 6;

    //=== Calculate d-space info based on the input file . ===//
    CalculateInfo(&totalVectors, &dimension, input_file);
    //=== Get the input dataSet. ===//
    Data<double> *dataset = parseData(input_file, dimension, totalVectors);

    //====  Intitialize Clustering Structure ====//
    Clustering<Data<double>> clustering =Clustering<Data<double>>(ClustersNum,dataset,totalVectors);
    //clustering.PrintClusters();
    clustering.Loyds_Clustering();
    return 0;
}