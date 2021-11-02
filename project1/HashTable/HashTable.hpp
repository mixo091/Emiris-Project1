#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <bits/stdc++.h>
#include <assert.h>
#include <ctime>
#include <map>

#include "../Data/Data.hpp"
#include "../HashFunction/HashFunction.hpp"

using namespace std;
/* A bucket is a list of points in space 
 In this case as in LSH we keep the points many times
 we will have a bucket as a list of pointers in points (vectors) */

template <typename K>
struct BucketEntry {
    /* ID: used from theory as Query Trick to avoid calculating Euclidean
    * distance, betwween query and every point in Bucket */
    int query_trick;
    /* This is our key */
    K key;

    BucketEntry(K k, int ID) {
        key = k;
        query_trick = ID;
    }

    void print() {
        Data<double> *data = (Data<double> *) key;
        data->printVector();
        cout << "ID(P) = " << query_trick << endl << endl;
    }
    int getQueryTrick() { return query_trick; }
    vector<double> getVector() { return key->v; }
    int getId() { return key->id; }
};

/* This is the bucket of our hash table */
template <typename K>
struct Bucket {
    int number_of_entries;
    int id;
    list<BucketEntry<K> *> bucket_entries; 

    Bucket(int i) {
        id = i;
        number_of_entries = 0;
    }

    void insert(K key, int ID){
        // struct BucketEntry bucket_entry = new BucketEntry<K>(key, ID);
        bucket_entries.push_back(new BucketEntry<K>(key, ID));
        // increase list items
        number_of_entries++;
    }

    // void displayBucket(){
    //     cout<<"BUCKET ["<<id<<"] :"<<endl;
    //     for(int i=0; i < number_of_entries; i++){
    //         typename list<K>::iterator it = bucket_entries.begin();
    //         advance(it, i); // 'it' points to the element at index i
    //         (*it)->printVector();
    //     }
    // }
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
            hash_table[i] = NULL;
        
        // create the hash function
        h_fun = new HashFunction(w, k, dim);
    }

    // query_trick = (h_1 * r_1 ) mod M + (h2 * r_2) mod M.......

    void insert(Data<double> *key, const int &id) {
        // this is used from theory as query trick
        int query_trick = 0;
        unsigned int index = h_fun->hashValue(key->getVector(), table_size, &query_trick);
       // check size of index
        assert(index <= INT_MAX);

        if(hash_table[index] != NULL) hash_table[index]->insert(key, query_trick);
        else {
            // create new Bucket and insert key
            hash_table[index] = new Bucket<K>(index);
            hash_table[index]->insert(key, query_trick);
        }
    }

    void find_NN(Data<double> *q, map<double, int> &my_map , double *time_NN) {
        double temp_min = INT_MAX;
        int id = 0;
        int query_trick = 0;
        unsigned int index = h_fun->hashValue(q->getVector(), table_size, &query_trick);
        // this is the bucket that q vector is hashed
        struct Bucket<K> *bucket = hash_table[index];
        // check if current bucket exist
        if(bucket != NULL) {
            // cout << "Bucket " << bucket->id << " with " << bucket->number_of_entries \
            //                                             << " entries." << endl;

            // we need to calculate the time
            const clock_t begin_time = clock();
            clock_t end_time;
            
            // traverse the list of Data of current bucket
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                // calculate euclidean_distance of q and every item in the list
                if(query_trick == (*it)->getQueryTrick()) {

                    double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                    if(euDist < temp_min) {
                        temp_min = euDist;
                        //  hold the id
                        id = (*it)->getId();

                        // insert in map
                        my_map.insert(pair<double, int>(temp_min, id));
                    }
                } else continue;
                
            }
            end_time = clock();
            
            // calculate time
            *time_NN = double(end_time - begin_time) / CLOCKS_PER_SEC;            
        }
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
