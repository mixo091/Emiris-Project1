#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Utilities.h"

using namespace std;
//Function to get the args for LSH.
int Handle_LSH_args ( 
    int argc, char **argv, 
    string *i_file, string *qr_file, string *o_file,
    int *k, int *L, int *N, int *R ) 
    {
    if(argc < 7) {
        cerr << "Wrong arguments. Please try again!" << endl;
        exit(0);
    }
    if(strcmp(argv[1], "-i") != 0) {    
        cerr << "Please give the input file." << endl;
        return -1;
    }
    *i_file = argv[2];
    if(strcmp(argv[3], "-q") != 0) {    
        cerr << "Please give the input file." << endl;
        return -1;
    }
    *qr_file = argv[4];
    for(int i = 5; i < argc; i++) {
        if(strcmp(argv[i], "-k") == 0) {
            *k = atoi(argv[i + 1]);
        } else if(strcmp(argv[i], "-L") == 0) {
            *L = atoi(argv[i+1]);
        } else if(strcmp(argv[i], "-N") == 0) {
            *N = atoi(argv[i+1]);
        } else if(strcmp(argv[i], "-R") == 0) {
            *R = atoi(argv[i+1]);
        } else if(strcmp(argv[i], "-o") == 0) {
            *o_file = argv[i+1];
        }
    }
    if(o_file->empty()) {
        cerr << "No output file given.";
        return -1;
    }
    return 1;
}

int getVectorDim(string str) {
    stringstream ss(str);
    string temp;
    int countDim = 0;
    while(getline(ss, temp, ' ')) {
        countDim++;
    }
    return countDim;
}

void Cal_LSH_needs(int *tableSize, int *dim,string filename)
{
    bool flag = true;
    ifstream input_file;   
    input_file.open(filename.c_str());
    if(input_file.is_open()) {
        string str;
        while(getline(input_file, str)) {
            if(flag) {
                flag = false;
                *dim = getVectorDim(str);
            }
            (*tableSize)++;    
        }
    } else {
        cerr << "Unable to open file" << endl;
        exit(-1);
    }
    cout << "hash table size = " << *tableSize << endl;
    // close the file 
    input_file.close();
}

Data* parseData(string filename, int dim, int totalVectors) {
    ifstream input_file;  
    Data *arr = new Data[totalVectors];
    // we need to count the Data created
    int nVector = 0;
    input_file.open(filename.c_str());
    if(input_file.is_open()) {
        string str;
        while(getline(input_file, str)) {
            // we need a counter for vector dimensions
            int i = 0;
            istringstream ss(str);
            // we need to seperate coordinates and store them in a vector container
            // store id
            string id;
            getline(ss, id, ' ');  
            arr[nVector].setId(stoi(id));
            // cout << "id = " << id << "  ";
            string x_ij;
            while(getline(ss, x_ij, ' ')) {
                // cout << "xij = " << x_ij << "   ";
                if(i++ < dim) {
                    // let's set our data
                    arr[nVector].setVector(stod(x_ij));
                }
            }
            nVector++;
        }
    } else {
        cerr << "Unable to open file" << endl;
        exit(-1);
    }
    // close the file 
    input_file.close();
    return arr;
}



