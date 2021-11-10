//Lsh basic functionality.
#include  "lsh.hpp"
//Function to print the hashTables of the LSH 
void Print_HTs(Lsh<double> lsh){
    for(int j = 0; j < numberOfHashTables; j++){
        cout <<"___________ HASH TABLE ["<<j<<"] ____________"
        hash_tables[j]->PrintHT();
    }

}