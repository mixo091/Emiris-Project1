#pragma once

#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>
#include <ctime>

#include "../Data/Data.hpp"

using namespace std;

/* This function handles arguments given by the user in the need of LSH */
int Handle_LSH_args ( int, char **, 
    string *, string *, string *,
    int *, int *, int *, int *);

/* Argument handling for hypercube and lsh */
void arg_handling(int, char **, string *, string *,string *, int *, int *, int *, int *, int *);

/* Calculation hash table size and vector dimension, given the input file */
void calc_dimensions(int *, int *, string *);

/* Creating the data set given the input file */
template <typename K>
void parseData(string filename, int dim, Data<K> *arr) {
    // open file for reading
    ifstream input_file(filename);  
    // Data<double> *arr = new Data<double>[totalVectors];
    // we need to count the Data created
    int nVector = 0;
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

            string x_ij;
            while(getline(ss, x_ij, ' ')) {
                // not a whitespace char
                if(isspace(x_ij.at(0)) == 0)
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

    // return arr;
}

/* Computing and returning vector v of hash function */
template <typename T>
void normal_distribution_fun(vector<T> , float, float);
void normal_distribution_fun(double *, float, float);

/* Easy to understand */
template <typename K>
double euclidean_dist(const K &v1, const K &v2) 
{
    double dist = 0;

    for(unsigned int i = 0; i < v1.size(); i++) 
        dist += pow(v1[i] - v2[i],  2);
    
    return sqrt(dist);
}
/* Modular exponetiation algorithm is used to avoid overflow
 Used in LSH to calculate  Recall (ab) mod m = ((a mod m)(b mod m)) mod m*/
int modular_pow(int, int, int);
unsigned long positive_modulo( unsigned long, unsigned);

/* This is a brute force method for NN, storing in a vector
* the euclidean distance of a query and every point of the input file
stored in dataset */
template <typename T>
void BruteForceNN(vector<double> qr_v,Data<T> *dataset, int data_size, vector<double> *brute_force_v) {
    // for every vector in dataset, calculate euclidean distance 
    for(int i = 0; i < data_size; i++) {
        double eu_dist = euclidean_dist(qr_v, dataset[i].getVector());

        // push eu_dist to our brute_force_v
        brute_force_v->push_back(eu_dist);
    }
}

/* Simple coin toss function return 0 or 1 for hypercube implementation */
int coinToss();

/* Calculate hamming distance of two verices for hypercube */
int hammingDistance(int, int);
