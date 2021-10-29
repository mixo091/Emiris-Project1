#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <bits/stdc++.h>
#include <assert.h>

#include "../VectorList/VectorList.hpp"
#include "../Data/Data.hpp"
#include "../HashFunction/HashFunction.hpp"

using namespace std;
/* A bucket is a list of points in space 
 In this case as in LSH we keep the points many times
 we will have a bucket as a list of pointers in points (vectors) */


/* This is the bucket of our hash table */
template <typename K>
struct Bucket {
    // K key;
    int id;
    int number_of_entries = 0;
    // int id;
    // struct Bucket *next;
    list<K> bucket_entries; 

    Bucket(int i) {
        id = i;
        number_of_entries = 0;
    }

    void insert(K key){
        bucket_entries.push_back(key);
        // increase list items
        number_of_entries++;
    }

    void displayBucket(){
        cout<<"BUCKET ["<<id<<"] :"<<endl;
        for(int i=0; i < number_of_entries; i++){
            typename list<K>::iterator it = bucket_entries.begin();
            advance(it, i); // 'it' points to the element at index i
            (*it)->printVector();
        }
    }
};

template <typename K>
class HashTable
{   
    /* Number of buckets */
    int buckets;
    /* Hash table size */
    int table_size;
    struct Bucket<K> **hash_table ; 
    /* Every hash table has it's own hash function */
    HashFunction *h_fun;
 K key;
public:
    HashTable(int ht_size, int w, int k, int dim) 
    : table_size(ht_size) 
    {
        // allocate the buckets.
        hash_table = new Bucket<K>*[table_size];
        for(int i = 0; i < table_size; i++)
            hash_table[i] = new Bucket<K>(i);
        
        // create the hash function
        h_fun = new HashFunction(w, k, dim);
    }

    void insert(Data<double> *key, const int &id) {
        unsigned int index = h_fun->hashValue(key->getVector(), table_size);
        // cout << "Data with id " << id << " is to be inserted in index " << index << endl;
       // check size of index
        assert(index <= INT_MAX);
        assert(index <= 624);
        hash_table[index]->insert(key);

        // struct Bucket<K> *prev = NULL;
        // struct Bucket<K> *entry = hash_table[index];

        // while(entry != NULL) {
        //     prev = entry; 
        //     entry = entry->next;
        // }
        // if(entry == NULL) {
        //     // do smthing
        //     entry = new Bucket<K>(key, id);
        //     entry->printBucketEntry();
        // }

        // if(prev == NULL) 
        //     // insert first
        //     hash_table[index] = entry;
        // else 
        //     prev->next = entry;
    }

    void PrintHT(){
        for (int i = 0; i < table_size; ++i) {
            cout<<"++++++ BUCKET ["<<i<<"] ++++++"<<endl;

            hash_table[i]->displayBucket();
        } 
    }

//     ~HashTable() {
//         // destroy all buckets one by one
//         for (int i = 0; i < table_size; i++) {
//             Bucket<K> *temp = hash_table[i];
//             while (temp != NULL) {
//                 Bucket<K> *prev = temp;
//                 temp = temp->next;
                
//                 delete prev;
//             }
//             hash_table[i] = NULL;
//         }
//         // destroy the hash table
//         delete [] hash_table;

//         delete [] h_fun;
//     }
};
