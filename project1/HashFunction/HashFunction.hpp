#include <iostream>
#include <string.h>
#include <chrono>
#include <random>

#include "../Utilities/Utilities.hpp"

using namespace std;

template <typename T>
class HashFunction
{
private:
    /* vector array*/
    double *v;
    /* needed for hash value */
    int w;
    double t;
    int k;
    int dim;
    int M;
    /* array  of r values for g calculation */
    int *r;
    /* array of h values */
    int *h;
public:
    HashFunction(int _w, int _k, int _dim) : w(_w), k(_k), dim(_dim) 
    {
        unsigned seed = chrono::steady_clock::now().time_since_epoch().count(); 
        default_random_engine e (seed); 
  
        /* declaring normal distribution object 'distN' and initializing its mean and standard deviation fields. */
        /* Mean and standard deviation are distribution parameters of Normal distribution. Here, we have used mean=5, and standard deviation=2. You can take mean and standard deviation as per your choice */
        uniform_int_distribution<int> distN(-20, 20);

        r = new int[k];
        for(int i = 0; i < k; i++) 
            r[i] = distN(e);

        // set t
        normal_distribution_fun(&t, 0.0, (double) w);

        // set to NULL
        h = NULL;

        // calculate M , needed for modular operation
        int M = pow(2, 32 / this->k);
    }

    ~HashFunction() {
        delete [] r;
    }

    int hfunction(vector<T> p);
    int hashValue(vector<T> p, int tableSize);

};

