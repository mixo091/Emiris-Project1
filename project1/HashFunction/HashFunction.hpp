#pragma once

#include <iostream>
#include <string.h>
#include <chrono>
#include <random>

#include "../Utilities/Utilities.hpp"

using namespace std;

class HashFunction
{
private:
    /* vector array*/
    double *v;
    /* needed for hash value */
    int w;
    double t;
    int num_of_hfun;
    int dim;
    int M;
    /* array  of r values for g calculation */
    int *r;
    /* array of h values */
    int *h;
public:
    HashFunction(int _w, int _k, int _dim) : w(_w), num_of_hfun(_k), dim(_dim) 
    {
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
        default_random_engine e (seed); 
  
        /* declaring normal distribution object 'distN' and initializing its mean and standard deviation fields. */
        /* Mean and standard deviation are distribution parameters of Normal distribution. Here, we have used mean=5, and standard deviation=2. You can take mean and standard deviation as per your choice */
        uniform_int_distribution<int> distN(-20, 20);

        r = new int[num_of_hfun];
        h = new int[num_of_hfun];
        for(int i = 0; i < num_of_hfun; i++) 
            r[i] = distN(e);

        // set t
        normal_distribution_fun(&t, 0.0, (double) w);

        // calculate M , needed for modular operation
        int M = pow(2, 32 / num_of_hfun);
    }

    ~HashFunction() {
        delete [] r; delete [] h;
    }

    int hfunction(vector<double> p) {
        long double product = 0;
        vector<double> v;

        // create a vector by normal distribution 
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
        default_random_engine e (seed);
        normal_distribution<double> distN(0.0, 1.0);

        for(int i = 0; i < dim; i++)
            v.push_back( distN(e) );

        for(int i = 0; i < dim; i++)
            product += v[i] * p[i];
        
        return (int) floor( ( this->t + (double) product) / this->w);

    }

    unsigned int hashValue(vector<double> p, int tableSize) {
        // this will be our Sum of h * r
        int sum = 0;

        for(int i = 0; i < num_of_hfun; i++) {
            // compute h(p)
            this->h[i] = hfunction(p);

            // we need to avoid overflow, so we use the formula from the slides
            // Recall (ab) mod m = ((a mod m)(b mod m)) mod m
            int res = modular_pow(h[i], r[i], M);

            // cout << "h_i = " << h[i] << endl;
            // cout << "r_i = " << r[i] << endl;
            // cout << "res = " << res << endl;
            
            if(res < 0) {
                cerr << "Modular error" << endl;
                exit(-1);
            }

            sum += res;
        }

        return sum % tableSize;
    }

    // int hfunction(vector<T> p);
    // unsigned int hashValue(vector<T> p, int tableSize);

};