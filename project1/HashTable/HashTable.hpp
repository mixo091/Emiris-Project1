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

// A bucketEntry is an Item of a bucket , a.ka points to a data_point */
template <typename K>
struct BucketEntry {
    int query_trick; //Trick for avoiding to calculate everything.
    K key;          //The key entry.

    BucketEntry(K k, int ID) {
        key = k;
        query_trick = ID;
    }
    //Print the bucket entry.
    void printEntry() {
        Data<double> *data = (Data<double>*) key;
        data->printVector();
        //cout << "ID(P) = " << query_trick << endl << endl;
    }
    void printEntryId(){
        Data<double> *data = (Data<double>*) key;
        data->printVectorId();

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
    list<BucketEntry<K>*> bucket_entries; 

    Bucket(int i) {
        id = i;
        number_of_entries = 0;
    }

    void insert(K key, int ID){
        bucket_entries.push_back(new BucketEntry<K>(key, ID));
        number_of_entries++; //Increase list items
    }
    //Display The bucket.
    void displayBucket(){
        cout<<"BUCKET ["<<id<<"] :"<<endl;
        for(int i=0; i < number_of_entries; i++){
            typename list<BucketEntry<K>*>::iterator it = bucket_entries.begin();
            advance(it, i); // 'it' points to the element at index i
            (*it)->printEntryId();
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
        //Allocate the buckets.
        hash_table = new Bucket<K>*[table_size];
        for(int i = 0; i < table_size; i++)
            hash_table[i] = NULL;
        //Create the hash function
        h_fun = new HashFunction(w, k, dim);
    }

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

    void find_NN(Data<double> *q, map<double, int> &k_nearest_map , double *time_NN,int k) {
        double temp_min[k];
        cout<<k<<endl;
            int i;
        for(i=0; i<k; i++)
            temp_min[i] = INT_MAX;
        map<double,int> distance_map;
        int id = 0;
        int query_trick = 0;
        // --- Get the bucket to be traversed. --- //
        unsigned int index = h_fun->hashValue(q->getVector(), table_size, &query_trick);
        struct Bucket<K> *bucket = hash_table[index];
        if(bucket != NULL) {
            //cout <<"Bucket to be traversed" << bucket->id <<endl;
            //Traverse the list of Data at current bucket.
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                //if(query_trick == (*it)->getQueryTrick()) {
                    double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                    distance_map.insert(pair<double, int>( euDist, (*it)->getId()));
               // } else continue;  
            }
            //=== Keep the k best from the bucket.
            int item = 0 ;
            for (auto it = distance_map.cbegin(); it != distance_map.cend(); ++it) {
                k_nearest_map.insert(pair<double, int>((*it).first,(*it).second));
                //cout<<"test "<<q->getId()<<" "<<(*it).first<<" "<<(*it).second<<endl;
                item++;
                if(item == k){ break ;}
            }
         
            distance_map.clear();
            //Calculate time
            //*time_NN = double(end_time - begin_time) / CLOCKS_PER_SEC;            
        }
    }
    void PrintHT(){
        for (int i = 0; i < table_size; ++i) {
            cout<<"++++++ BUCKET ["<<i<<"] ++++++"<<endl;
            if(hash_table[i] != NULL)
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
