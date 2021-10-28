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
    K key;
    int bucketSize = 0;
    int id;
    struct Bucket *next;

    Bucket() {
        bucketSize = 0;
        id = -1;
        next = NULL;
    }

    Bucket(const K &_key, int _id) {
        key = key;
        id = _id;
        next = NULL;
    }

    Bucket(const Bucket<K> &obj) {
        cout << "Copy constructor of Bucket." << endl;
        
        key = obj.key;
        next = obj.next;
        id = obj.id;
    }

    ~Bucket() {
        delete key;
        next = NULL;
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

public:
    HashTable(int ht_size, int w, int k, int dim) 
    : table_size(ht_size) 
    {
        // allocate the buckets.
        hash_table = new Bucket<K>*[table_size];
        for(int i = 0; i < table_size; i++)
            hash_table[i] = NULL;
        
        // create the hash function
        h_fun = new HashFunction(w, k, dim);
    }

    void insert(Data<double> *key, const int &id) {
        unsigned int index = h_fun->hashValue(key->getVector(), table_size);
        cout << "Data with id " << id << " is to be inserted in index " << index << endl;
       // check size of index
        assert(index <= INT_MAX);

        struct Bucket<K> *prev = NULL;
        struct Bucket<K> *entry = hash_table[index];

        while(entry != NULL) {
            prev = entry; 
            entry = entry->next;
        }
        if(entry == NULL) 
            // do smthing
            entry = new Bucket<K>(key, id);

        if(prev == NULL) 
            // insert first
            hash_table[index] = entry;
        else 
            prev->next = entry;
    }

    ~HashTable() {
        // destroy all buckets one by one
        for (int i = 0; i < table_size; i++) {
            Bucket<K> *temp = hash_table[i];
            while (temp != NULL) {
                cout << "exei skoupidia!\n" << endl;
                Bucket<K> *prev = temp;
                temp = temp->next;
                
                delete prev;
            }
            hash_table[i] = NULL;
        }
        // destroy the hash table
        delete [] hash_table;

        delete [] h_fun;
    }

    
};