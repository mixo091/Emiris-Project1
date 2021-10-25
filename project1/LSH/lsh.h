#include <vector>
#include "../Data/Data.h"
#include "../HashTable/HashTable.h"
using namespace std;
class Lsh
{
private:
    int L_Tables;
    int ht_Size;
    int vecDimension;

   HashTable** hash_tables; 
public:
    Lsh(int L, int size, int dim, Data* d)
    :L_Tables(L), ht_Size(size), vecDimension(dim) 
    {
        hash_tables = new HashTable*[L_Tables];
        for(int i = 0; i < L_Tables; i++) {
            hash_tables[i] = new HashTable(ht_Size);
        }
        // let's hash all the items
    }
    ~Lsh() {
        for (int i = 0; i < L_Tables; i++) 
            delete[] hash_tables[i];
        delete[] hash_tables;
    }
};