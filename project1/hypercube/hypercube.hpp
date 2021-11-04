#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>

#include "../HashTable/HashTable.hpp"
#include "../Utilities/Utilities.hpp"
#include "../Data/Data.hpp"
#include "../LSH/lsh.hpp"

template <typename K>
class hypercube : public Lsh<K>
{
private:
    // number of neighbour buckets to check
    int probes; 
    // number of total vectors to check
    int M; 
    // p-> f(h(p)) : 0 ? 1
    std::unordered_map<int, int> f_map;
public:
    hypercube(int _probes, int _M, int _w, int _k, int _dim, int totalVectors, Data<K> *dataset) 
    :   probes(_probes), 
        M(_M), 
        Lsh<K>(1, pow(2, _k), _dim, _k, _w) // call constructor of Base class Lsh
                                        // where 1 is the number of Hash Tables and
                                        // pow(2, _k) is the size of the table
    {
        cout << "Constructing hypercube...\n";

        // this is our index 
        int bucket_num;       

        for(int i = 0; i < totalVectors; i++) {
            bucket_num = 0;

            // k hash_functions
            for(int j = 0; j < this->numberOfHashFunctions; j++) {
                
                // compute h1.....hk
                int h = this->hash_tables[0]->getHashFunction()->hfunction(dataset[i].getVector());

                // map h value to 0 ? 1
                int zero_or_one = coinToss();
                f_map.insert(std::make_pair(h, zero_or_one));

                // avoid bucket overflow and create our index
                if(j != this->numberOfHashFunctions - 1)
                    bucket_num += zero_or_one * pow(2, j);
            }

            // insert in hash table
            this->hash_tables[0]->insertHyperCube(&dataset[i], bucket_num);
        } 

        // this->hash_tables[0]->PrintHT();
    }

    ~hypercube() {
        cout << "Destructing hypercube...\n";
        f_map.clear();
    }

    void ANN(Data<K> *qr_data, int qr_lines, Data<K> *in_data, int in_dataSize, int N, const string &out_file) {
        ofstream output(out_file);
        if(!output.is_open()) {
            cout << "Error with output file\n";
            exit(-1);
        }

        // for every query
        for(int i = 0; i < qr_lines; i++) {

            double time_hcube = 0.0; // timer for Hypercube
            double time_BF = 0.0; // timer for brute force search

            unsigned int bucket_num = 0;

            for(int j = 0; j < this->numberOfHashFunctions; j++) {
                int h = this->hash_tables[0]->getHashFunction()->hfunction(qr_data[i].getVector());

                // check if h value maps to 0 ? 1
                int zero_or_one = f_map.at(h);

                cout << "h = " << h << " with value " << zero_or_one << endl;
                    
                if(j != this->numberOfHashFunctions - 1)
                    bucket_num += zero_or_one * pow(2, j);
            }
            cout << "bucket_num = " << bucket_num << endl;
            // check the bucket of hash table
            
        }
    }

    void print_fmap() {
        for(auto &it : f_map){
            cout<< it.first << " : " << it.second << endl;
}
    }

};



