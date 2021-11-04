#include <vector>
#include <map>
#include <ctime>
#include <iostream>
#include "../Data/Data.hpp"
#include "../HashTable/HashTable.hpp"
#include "../HashFunction/HashFunction.hpp"
#include <chrono>



#define BUCKET_DIVIDER 16
using namespace std;
using namespace std::chrono;
//Lsh Structure implementation.
template <typename T>
class Lsh
{
private:
    int numberOfHashTables;
    int ht_Size;
    int vecDimension;
    int numberOfHashFunctions;
    int w; 
    HashTable<Data<T>*> **hash_tables; 
public:
    //Constructor.
    Lsh(int L, int totalVectors, int dim, int k, int w, Data<T> *data)
    : numberOfHashTables(L), ht_Size(totalVectors / BUCKET_DIVIDER), vecDimension(dim), numberOfHashFunctions(k), w(w)
    {   
        hash_tables = new HashTable<Data<T> *>*[numberOfHashTables];
        for(int i = 0; i < numberOfHashTables; i++) {
            hash_tables[i] = new HashTable<Data<T>*>(ht_Size, w, numberOfHashFunctions, vecDimension);
        }
        // ------- Insert Data Points to the LSH ------- //
        for(int i = 0; i < totalVectors; i++) {
            for(int j = 0; j < numberOfHashTables; j++) 
                hash_tables[j]->insert(&data[i], data[i].id);
        }
    }
    //Print the HashTables.
    void PrintHTs(){
        for(int j = 0; j < numberOfHashTables; j++){
            cout<<"Table size "<<ht_Size<<endl;
            cout <<"__ HASH TABLE ["<<j<<"] __"<<endl;
            hash_tables[j]->PrintHT();
        }
    }

    //Calculate Approximate Nearest Neighbor.
    void ANN(Data<T> *qr_data, int qr_lines, Data<T> *in_data, int in_dataSize, int N, const string &out_file) {
        //---Open output file---//
        ofstream output(out_file);
        if(!output.is_open()) {
            cout << "Error with output file\n";
            exit(-1);
        }
        clock_t time_req;
        clock_t begin_time_lsh;
        clock_t end_time_lsh;
        clock_t time_lsh ;
        map<double, int> Result_map;
        map<double, int> my_map;
        for(int i = 0; i < 3; i++) {
            double time_LSH = 0.0; // timer for LSH-NN
            clock_t time_BF = 0.0; // timer for BF-NN
            begin_time_lsh = clock();
            auto start = high_resolution_clock::now();
            //Get every query point and hash it in every hash table.
            for(int j = 0; j < numberOfHashTables; j++) {
                hash_tables[j]->find_NN(&qr_data[i], my_map, &time_LSH,N);
                for (auto it = my_map.cbegin(); it != my_map.cend(); ++it) {
                    //Ckeck if this vector already exists in the Result_map.
                   // cout<<"Res:"<<(*it).second<<endl<<endl;
                    Result_map.insert(pair<double, int>((*it).first, (*it).second));
                        
                    
                }
                my_map.clear();
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
  
// To get the value of duration use the count()
// member function on the duration object
            cout <<"lsh"<< duration.count() << endl;

            //need to calculate the time taken for BF
           // const clock_t begin_time = clock();
            

            auto startBF = high_resolution_clock::now();
            // vector used for brute force
            vector<double> brute_force_v;
            //Brute force method for NN
            BruteForceNN(qr_data[i].getVector(), in_data, in_dataSize, &brute_force_v);
            // sort brute force vector
            sort(brute_force_v.begin(), brute_force_v.end());
            //time_BF = float(clock() - begin_time) / CLOCKS_PER_SEC;   
            // keep the N-th distance of NN of the sorted vector
            double bf_nearestNeighbor = brute_force_v.at(N);
            auto stopBF = high_resolution_clock::now();
            auto durationBF = duration_cast<microseconds>(stopBF - startBF);
            cout <<"BF"<< durationBF.count() << endl;
            int n_ostos = 0 ;
            output << "\nQuery: " << qr_data[i].getId();
            
            for (auto it = Result_map.cbegin(); it != Result_map.cend(); ++it) {
                 
                output<< "\nNearest neighbor-" << n_ostos << ": " << (*it).second \ 
                 << "\ndistanceLSH: " << (*it).first \ 
                 << "\ndistanceTrue: " << brute_force_v.at(n_ostos) \
                 << "\ntLSH: " << time_lsh \
                 << "\ntTrue:" << time_BF;
                  
                  n_ostos++;
                if(n_ostos == N){
                    break;
                }
            }
             Result_map.clear();
             brute_force_v.clear();
        }
        // close output file
        output.close();
    }




    ~Lsh() {
        // for (int i = 0; i < numberOfHashTables; i++) 
        //     delete[] hash_tables[i];
        // delete[] hash_tables;
            }
};