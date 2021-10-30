#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <math.h>

#include "Utilities.hpp"

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

Data<double>* parseData(string filename, int dim, int totalVectors) {
    // open file for reading
    ifstream input_file(filename);  
    Data<double> *arr = new Data<double>[totalVectors];
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

    return arr;
}

int getVectorDim(string str) {
    stringstream ss(str);
    string temp;
    int countDim = 0;
    while(getline(ss, temp, ' ')) {
        if(isspace(temp.at(0)) == 0)
            countDim++;
    }

    return countDim - 1; 
}

void Calc_LSH_needs(int *tableSize, int *dim, string filename)
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

    // close the file 
    input_file.close();
}

void normal_distribution_fun(double *n, float x, float y) {
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
    default_random_engine e (seed); 
  
    /* declaring normal distribution object 'distN' and initializing its mean and standard deviation fields. */
    /* Mean and standard deviation are distribution parameters of Normal distribution. Here, we have used mean=5, and standard deviation=2. You can take mean and standard deviation as per your choice */
    normal_distribution<double> distN(x, y);

    *n  = distN(e);
}

int modular_pow(int base, int exponent, int modulus)
{
    int result = 1;
    base = base % modulus;

    while (exponent > 0)
    {
        if(exponent & 1)
            result = (result * base) & modulus;
        exponent = exponent >> 1;
        base = (base*base) % modulus;
    }
    return result;
}

long long inn_product(int *h, int *r, int dim) {
    int product = 0;
    for(int i = 0; i < dim; i++) 
        product += h[i] * r[i];

    return product;
}   

unsigned long positive_modulo( unsigned long x, unsigned y) {
    return  ( ( x % y ) + y ) % y;
}



