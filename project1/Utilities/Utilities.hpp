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

/* Calculation hash table size and vector dimension, given the input file */
void Calc_LSH_needs(int *, int *,string);

/* Creating the data set given the input file */
Data<double>* parseData(string filename, int dim, int totalVectors); 
/* Computing and returning vector v of hash function */
// template <typename T>
// void normal_distribution_fun(float, float);
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
