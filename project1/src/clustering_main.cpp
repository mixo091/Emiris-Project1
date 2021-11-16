#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>
#include "./Clustering/ClusteringUtilities.hpp"
#include "./Clustering/Clustering.hpp"
#include "./Utilities/Utilities.hpp"
#include <algorithm>


#include <ctype.h>

using namespace std;


    //cluster -i ./test_files/input_small_id -c ./test_files/cluster.conf -o output -complete 0 -m LSH

int main(int argc, char **argv){

    //----------------------- input args --------------//
    cout<<"number of arguments :"<<argc<<endl;
    if(strcmp(argv[1], "-i") != 0) {    
        cerr << "Please give the input file." << endl;
        exit(-1);
    }
    string inputFile = argv[2];
    cout<<"Input File:"<<inputFile<<endl;

    if(strcmp(argv[3], "-c") != 0) {    
        cerr << "Please give the input file." << endl;
        exit(-1);
    } 
    string configFile = argv[4];
    cout<<"Config File"<<configFile<<endl;

    if(strcmp(argv[5], "-o") != 0) {    
        cerr << "Please give the input file." << endl;
        exit(-1);
    } 
    string outPutFile = argv[6];
    cout<<"OutputFile: "<<outPutFile<<endl;
    string method ;
    int complete; //0 or 1 
    if(argc == 11){
        if(strcmp(argv[7], "-complete") != 0) {    
            cerr << "Please give the input file." << endl;
            exit(-1);
        } 
        complete = atoi(argv[8]);

        if(strcmp(argv[9], "-m") != 0) {    
            cerr << "Please give the input file." << endl;
            exit(-1);
        } 
        method = argv[10];


    }else if(argc == 9 ){
        if(strcmp(argv[7], "-m") != 0) {    
            cerr << "Please give the input file." << endl;
            exit(-1);
        } 
        method = argv[7];
    }
    cout<<"Method:"<<method<<endl;
    bool printComplete;
    if( complete == 1){
        printComplete = true;
    }else if (complete ==0 ){
        printComplete = false;
    }

    //------------------- config file -----------//
    int number_of_clusters;
    int number_of_hash_tables;
    int number_of_vector_hash_functions; 
    int max_number_M_hypercube;
    int number_of_hypercube_dimensions;
    int number_of_probes;
    string input_file, out_file, qr_file;
    int totalVectors = 0; //Total points in space.
    int dimension = 0;    //Space Dimension.
    input_file = inputFile;

    ifstream config(configFile);  
    // we need to count the Data created
    int nVector = 0;
    if(config.is_open()) {
        string str;
        std::string line;
        while (getline(config, line)) 
        {
           cout<<line<<endl;
            if (line[0] == '#' || line.empty()) continue;

            auto delimiterPos = line.find(":");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);


            if (name == "number_of_clusters") number_of_clusters = atoi(value.c_str());
            else if (name == "number_of_hash_tables") number_of_hash_tables = atoi(value.c_str());
            else if (name == "number_of_vector_hash_functions") number_of_vector_hash_functions = atoi(value.c_str());
            else if (name == "number_of_vector_hash_functions") number_of_vector_hash_functions = atoi(value.c_str());
            else if (name == "max_number_M_hypercube") max_number_M_hypercube = atoi(value.c_str());
            else if (name == "number_of_hypercube_dimensions") number_of_hypercube_dimensions = atoi(value.c_str());
            else if (name == "number_of_probes") number_of_probes = atoi(value.c_str());
        }
    }else{
        cout<<"error opening config file..";
        return -1 ;
    }

    // std::ifstream is RAII, i.e. no need to call close
    //=== Calculate d-space info based on the input file . ===//
    CalculateInfo(&totalVectors, &dimension, input_file);
    //=== Get the input dataSet. ===//
    Data<double> *dataset = parseData(input_file, dimension, totalVectors);
    cout<<"Clusters_"<<number_of_clusters<<endl;
    //====  Intitialize Clustering Structure ====//
    Clustering<Data<double>> clustering =Clustering<Data<double>>(number_of_clusters,dataset,totalVectors);

    if(method == "LSH"){
        clustering.Lsh_Clustering(dimension, outPutFile, number_of_hash_tables,number_of_vector_hash_functions,printComplete);
    }
    if(method == "Classic"){
        clustering.Loyds_Clustering(outPutFile,printComplete);
    }
 
    return 0;
}