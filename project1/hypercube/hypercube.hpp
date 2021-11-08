#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <random>
#include <chrono>

#include "../HashTable/HashTable.hpp"
#include "../Utilities/Utilities.hpp"
#include "../Data/Data.hpp"
#include "../LSH/lsh.hpp"

using namespace std::chrono;

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
        int bucket_num = 0;       

        for(int i = 0; i < totalVectors; i++) {
            // calculate index for every vertex in dataset
            bucket_num = create_hash(dataset[i]);
            // cout << "bucket_num = " << bucket_num << "-" << dataset[i].getId() << endl;

            // insert in hash table
            this->hash_tables[0]->insertHyperCube(&dataset[i], bucket_num);
        } 

        print_fmap();
    }

    ~hypercube() {
        // cout << "Destructing hypercube...\n";
        f_map.clear();
    }

    int create_hash(Data<K> dataset) {
        /* Create hash value for the vertex given */

        int result = 0;
        int h = 0;

        for(int i = 0; i < this->numberOfHashFunctions; i++) {
            // compute h1...hk
            h = this->hash_tables[0]->getHashFunction()->hfunction(dataset.getVector());
            
            int zero_or_one = coinToss();
            f_map.insert(std::make_pair(h, zero_or_one));

            if(zero_or_one == 1) result = result | zero_or_one;

            // avoid overflow
            if(i < this->numberOfHashFunctions - 1 )
                result *= 2;
        }
        return result;
    }

    int cube_hasing(Data<K> dataset) {
        /* Find bucket of hypercube for the vertex given */

        int result = 0;
        // temp values of h1....hk
        int h = 0;

        for(int i = 0; i < this->numberOfHashFunctions; i++) {
            // compute h1...hk
            h = this->hash_tables[0]->getHashFunction()->hfunction(dataset.getVector());

            // cout << "h = " << h << endl;

            // check if h value maps to 0 ? 1
            int zero_or_one = f_map.at(h);

            if(zero_or_one == 1) result = result | zero_or_one;

            // avoid overflow
            if(i < this->numberOfHashFunctions - 1 )
                result *= 2;
        }
        return result;
    }

    std::vector<int> get_neigbors_by_distance(int query_vertex, int diff, int maxDim) {
        /* Returns an array of distances diff, of query vertex with every
        *  possible vertex 
        */
        std::vector<int> possible_neighbors;
        for(int i = 0; i < pow(2, maxDim); i++) {
            if(hammingDistance(i, query_vertex) == diff) {
                possible_neighbors.push_back(i);
            }
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(possible_neighbors.begin(), possible_neighbors.end(), g);

        int count_probes = 0;
        // from every neigbor choose randomly
        std::vector<int> final_neighbors;

        for(int j = final_neighbors.size(); 
            j < probes && count_probes < possible_neighbors.size(); 
            j++, count_probes++)
         {
            final_neighbors.push_back(possible_neighbors[count_probes]);
        }

        return final_neighbors;
    }

    void ANN(Data<K> *qr_data, int qr_lines, Data<K> *in_data, int in_dataSize, int N, const string &out_file) {
        /* Aproximate NN. For every query create his neighbours with the help of hamming distance.
        * Hamming distance is calculated for every dimension from i=1,...,k. 
        * We check the bucket of hashed query at first, and then we move into the neighbours vector */
        ofstream output(out_file);
        if(!output.is_open()) {
            cout << "Error with output file\n";
            exit(-1);
        }

        for(int i = 0; i < qr_lines; i++) {

            std::vector<int> neighbors;
            // maps for the output
            map<double, int> my_map;
            map<double, int> result_map;
    
            unsigned int bucket_num = cube_hasing(qr_data[i]);
           
            auto start = high_resolution_clock::now();
            
            int maxDim = this->numberOfHashFunctions;
            for(int j = 1; j < maxDim; j++) {
                neighbors = get_neigbors_by_distance(bucket_num, j, maxDim);

                this->hash_tables[0]->find_NN(&qr_data[i], my_map, N, neighbors, bucket_num, &M);

                for (auto it = my_map.cbegin(); it != my_map.cend(); ++it) {
                    result_map.insert(pair<double, int>((*it).first, (*it).second));        
                }
                my_map.clear();

                // need to stop searching, used instead of flag
                if(M == -1) break;
            }
            auto stop = high_resolution_clock::now();
            auto durationCube = duration_cast<microseconds>(stop - start);   

            auto startBF = high_resolution_clock::now();
            // vector used for brute force
            vector<double> brute_force_v;

            //Brute force method for NN
            BruteForceNN(qr_data[i].getVector(), in_data, in_dataSize, &brute_force_v);

            // sort brute force vector
            sort(brute_force_v.begin(), brute_force_v.end());
  
            auto stopBF = high_resolution_clock::now();
            auto durationBF = duration_cast<microseconds>(stopBF - startBF);

            int n_ostos = 0;
            output << "\nQuery: " << qr_data[i].getId();
            for (auto it = result_map.cbegin(); it != result_map.cend(); ++it) {
                if(++n_ostos == N) {

                    output<< "\nNearest neighbor-" << N << ": " << (*it).second \ 
                    << "\ndistanceCube: " << (*it).first \ 
                    << "\ndistanceTrue: " << brute_force_v.at(N-1) \
                    << "\ntCube: " << durationCube.count() \
                    << "\ntTrue:" << durationBF.count();
                  
                    break;
                }
            }
            result_map.clear();
            brute_force_v.clear();
        }
        output.close();
    }

    void print_fmap() {
        for(auto &it : f_map){
            cout<< it.first << " : " << it.second << endl;
        }
    }

};



