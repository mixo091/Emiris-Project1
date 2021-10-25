#include <iostream>
#include <vector>
#include "HashTable.h"

HashTable::HashTable(int ht_size):num_of_buckets(ht_size){
    //Allocate The buckets.
    hash_table =(Bucket*) malloc(num_of_buckets*sizeof(Bucket));
    for(int i =0; i <= num_of_buckets; i++){
        hash_table[i].id = i ;
    }
};

void HashTable::DisplayHT(){
    //Display The buckets.
    for(int i =0; i <= num_of_buckets; i++){
        cout<<"\n Bucket ["<<hash_table[i].id<<"] :";
    }
};

