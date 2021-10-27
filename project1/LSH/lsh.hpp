#include <vector>

#include "../Data/Data.hpp"
#include "../HashTable/HashTable.hpp"
#include "../HashFunction/HashFunction.hpp"

using namespace std;

template <typename T>
class Lsh
{
private:
    int numberOfHashTables;
    int ht_Size;
    int vecDimension;
    int numberOfHashFunctions;
    int w;
    
   HashTable<Data<T> *>** hash_tables; 
public:
    Lsh(int L, int totalVectors, int dim, int k, int w, Data<T> *data)
    : numberOfHashTables(L), ht_Size(totalVectors / 2), vecDimension(dim), numberOfHashFunctions(k), w(w)
    {   
        hash_tables = new HashTable<Data<T> *>*[numberOfHashTables];
        for(int i = 0; i < numberOfHashTables; i++) {
            hash_tables[i] = new HashTable<Data<T>*>(ht_Size, w, numberOfHashFunctions, vecDimension);
        }
        // let's insert some data from the dataset in hash table
        // with the use of hash function
        for(int i = 0; i < totalVectors; i++) {
            for(int j = 0; j < numberOfHashTables; j++) 
                hash_tables[j]->insert(&data[i], data[i].id);
        }
    }

    ~Lsh() {
        // for (int i = 0; i < numberOfHashTables; i++) 
            // /delete[] hash_tables[i];
        // delete[] hash_tables;
    }
};