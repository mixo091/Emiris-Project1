#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <bits/stdc++.h>
#include "../Data/Data.hpp"
#include "../HashFunction/HashFunction.hpp"

using namespace std;
/* A bucket is a list of points in space 
 In this case as in LSH we keep the points many times
 we will have a bucket as a list of pointers in points (vectors) */
/* This is the bucket of our hash table */
template <typename K>
struct BucketEntry {
    K key;
    int id;
    struct BucketEntry *next;

    BucketEntry() {
        id = -1;
        next = NULL;
    }

    BucketEntry(K _key, int _id) {
        key = key;
        id = _id;
    }
};

template <typename K>
class HashTable
{   
    int buckets_num; // Number of buckets .
    int BucketSize;  // The size of the buckets.
    int table_size;  // The size of the table.
    struct BucketEntry<K> **hash_table ; 
    HashFunction *h_fun; //The hashFunction of the table

public:
    HashTable(int ht_size, int w, int k, int dim) 
    : table_size(ht_size) 
    {
        //Allocating buckets.
        hash_table = new BucketEntry<K>*[table_size];
        for(int i = 0; i < table_size; i++)
            hash_table[i] = NULL; 
        //Creating the hash function
        h_fun = new HashFunction(w, k, dim);
    }
    void insert(Data<double> *key, const int &id) {
        unsigned int index = h_fun->hashValue(key->getVector(), table_size);
        cout << "Data with id " << id << " is to be inserted in index " << index << endl;
        struct BucketEntry<K> *prev = NULL;
        struct BucketEntry<K> *entry = hash_table[index];
        while(entry != NULL) {
            prev = entry; 
            entry = entry->next;
        }
        if(entry == NULL) 
            //Create new Entry.
            entry = new BucketEntry<K>(key, id);
        if(prev == NULL) 
            //Insert Fisrt Item.
            hash_table[index] = entry;
        else 
            prev->next = entry;
    }

    void PrintHT(){
        for (int i = 0; i < table_size; ++i) {
            cout<<"++++++ BUCKET ["<<i<<"] ++++++"<<endl;
            BucketEntry<K> *temp = hash_table[i];
        } 
    }

    ~HashTable() {
        //Destroy all buckets one by one
        for (int i = 0; i < table_size; ++i) {
            BucketEntry<K> *temp = hash_table[i]; 
            while (temp != NULL) {
                BucketEntry<K> *prev = temp;
                temp = temp->next;
                delete prev;
            }
            hash_table[i] = NULL;
        }
        // destroy the hash table
        // delete [] hash_table;
        delete [] h_fun;
    }   
};