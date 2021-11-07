#include <vector>
#include <map>
#include <ctime>

#include "../Data/Data.hpp"
#include "../HashTable/HashTable.hpp"
#include "../HashFunction/HashFunction.hpp"

#define BUCKET_DIVIDER 16

using namespace std;

template <typename T>
class Lsh
{
protected:
    int numberOfHashTables;
    int ht_Size;
    int vecDimension;
    int numberOfHashFunctions;
    int w;
    
   HashTable<Data<T> *> **hash_tables; 
public:
    // this constructor is called for LSH
    Lsh(int L, int totalVectors, int _dim, int _k, int _w, Data<T> *data)
    :   numberOfHashTables(L), 
        ht_Size(totalVectors / BUCKET_DIVIDER), 
        vecDimension(_dim), 
        numberOfHashFunctions(_k), 
        w(_w)
    {   
        hash_tables = new HashTable<Data<T> *>*[numberOfHashTables];
        for(int i = 0; i < numberOfHashTables; i++) {
            hash_tables[i] = new HashTable<Data<T>*>(ht_Size, w, numberOfHashFunctions, vecDimension);
        }
        // let's insert some data from the dataset in hash table
        // with the use of hash function
        for(int i = 0; i < totalVectors; i++) {
            for(int j = 0; j < numberOfHashTables; j++) 
                hash_tables[j]->insert(&data[i]);

        }
    }
    // this constructor is called for Hypercube
    Lsh(int L, int _size, int _dim, int _k, int _w)
    :   numberOfHashTables(L), 
        ht_Size(_size), 
        vecDimension(_dim), 
        numberOfHashFunctions(_k), 
        w(_w)
    {   
        // cout << "Constructing Lsh...\n";

        // create hash tables
        hash_tables = new HashTable<Data<T> *>*[numberOfHashTables];
        hash_tables[0] = new HashTable<Data<T>*>(ht_Size, w, numberOfHashFunctions, vecDimension);        
    }

    void PrintHTs(){
        for(int j = 0; j < numberOfHashTables; j++){
            cout <<"__ HASH TABLE ["<<j<<"] __"<<endl;
            hash_tables[j]->PrintHT();
        }
    }

    void ANN(Data<T> *qr_data, int qr_lines, Data<T> *in_data, int in_dataSize, int N, const string &out_file) {
        // open output file
        ofstream output(out_file);
        if(!output.is_open()) {
            cout << "Error with output file\n";
            exit(-1);
        }
        // create a map of <id, eu_dist> where id is int and eu_dist is double
        map<double, int> my_map;

        for(int i = 0; i < qr_lines; i++) {
        
            double time_LSH = 0.0; // timer for LSH-NN
            double time_BF = 0.0; // timer for BF-NN
            // get every query point
            // and hash it in every hash table
            for(int j = 0; j < numberOfHashTables; j++) {
                hash_tables[j]->find_NN(&qr_data[i], my_map, &time_LSH);
            }

            int k = 0;
            int item_id = 0; 
            double eu_dist = 0.0;
            // cout << "mymap contains:\n";
            for (auto it = my_map.cbegin(); it != my_map.cend(); ++it) {
                // cout << "{" << (*it).first << ": " << (*it).second << "}\n";
                k++;
                if(k == N) {
                    eu_dist = (*it).first;
                    item_id  = (*it).second;

                    break;
                }
            }
            // need to calculate the time taken for BF
            const clock_t begin_time = clock();
            clock_t end_time;

            // vector used for brute force
            vector<double> brute_force_v;

            // Brute force method for NN
            BruteForceNN(qr_data[i].getVector(), in_data, in_dataSize, &brute_force_v);
            // sort brute force vector
            sort(brute_force_v.begin(), brute_force_v.end());

            end_time = clock();

            time_BF = double(end_time - begin_time) / CLOCKS_PER_SEC;   

            // keep the N-th distance of NN of the sorted vector
            double bf_nearestNeighbor = brute_force_v.at(N);

            // done with query-i
            output << "\nQuery: " << qr_data[i].getId() \
                   << "\nNearest neighbor-" << N << ": " << item_id \ 
                   << "\ndistanceLSH: " << eu_dist \ 
                   << "\ndistanceTrue: " << bf_nearestNeighbor \
                   << "\ntLSH: " << time_LSH \
                   << "\ntTrue:" << time_BF;

            my_map.clear();
            brute_force_v.clear();
        }
        // close output file
        output.close();
    }

    virtual ~Lsh() {
        // cout << "Destructing Lsh... " << numberOfHashTables << endl;
        for (int i = 0; i < numberOfHashTables; i++) {
            delete hash_tables[i];
        }

        delete [] hash_tables;
    }
};