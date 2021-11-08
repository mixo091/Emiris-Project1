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

    ~BucketEntry() {

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

    ~Bucket() {
        // cout << "Destructing Bucket...";
        for(typename list<BucketEntry<K>*>::const_iterator it = bucket_entries.begin(); it != bucket_entries.end(); ++it)
        {
            delete *it;
        } 
        bucket_entries.clear();
    }

    void insert(K key, int ID){
        bucket_entries.push_back(new BucketEntry<K>(key, ID));
        // increase list items
        number_of_entries++;
    }

    void displayBucket(){
        // cout<<"BUCKET ["<<id<<"] :"<<endl;
        if(!bucket_entries.empty())
            for(auto &it : bucket_entries)
                it->key->printVector();

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

        // cout << "Constructing hash table with size " << table_size << endl;
        // allocate the buckets.
        hash_table = new Bucket<K>*[table_size];

        for(int i = 0; i < table_size; i++)
            hash_table[i] = NULL; 
        //Creating the hash function
        h_fun = new HashFunction(w, k, dim);
    }

    HashFunction *getHashFunction() { return h_fun; }

    // query_trick = (h_1 * r_1 ) mod M + (h2 * r_2) mod M.......

    void insert(Data<double> *key) {
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

    void insertHyperCube(Data<double> *key, int index) {
        if(hash_table[index] != NULL) hash_table[index]->insert(key, -1);
        else {
            hash_table[index] = new Bucket<K>(index);
            hash_table[index]->insert(key, -1);
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

    void find_NN(Data<double> *q, map<double, int> &k_nearest_map,int k, vector<int> neighbors, unsigned int current_bucket_num, int *M) {
        /* find NN for Hypercube checking current_bucket_num at first and if needed, every other neighbor*/

        map<double,int> distance_map;
        
        // amount of data searched must be <= M
        int count_items = 0; 
        bool stop_searching = false;

        // check hamming distance = 0 at first
        struct Bucket<K> *bucket = hash_table[current_bucket_num];
        if(bucket != NULL) {
            // cout << " not null bucket " << endl;
            // traverse the list of Data at current bucket.
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                    
                double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                distance_map.insert(pair<double, int>( euDist, (*it)->getId())); 

                // cout << "euDist of " << (*it)->getId() << " " << euDist << endl;

                if(++count_items >= *M) {
                    stop_searching = true;
                    break;
                }

                cout << "count_items = " << count_items << endl;
                        
            }

            // keep the k best from the bucket.
            int item = 0 ;
            for (auto it = distance_map.cbegin(); it != distance_map.cend(); ++it) {
                k_nearest_map.insert(pair<double, int>((*it).first,(*it).second));

                // if k reached, stop adding new neigbor
                if(++item >= k) break;
            }
            distance_map.clear();   

            // stop searching
            if(stop_searching) {cout << "counting " << count_items << endl; *M = -1; return; }
        }   
        // keep searching, M is not reached yet
        // cout << "search in neigbors: size" << neighbors.size()<<endl;
        
        for(int i = 0; i < (int)neighbors.size(); i++) {
            
            // cout << "neigbor-"<<neighbors.at(i) << endl;
            // traverse the vector of neighbors
            struct Bucket<K> *bucket = hash_table[neighbors.at(i)];
            if(bucket != NULL) {
            
            // traverse the list of Data at current bucket.
                typename std::list<BucketEntry<K> *>::iterator it;
                for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                    
                    double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                    distance_map.insert(pair<double, int>( euDist, (*it)->getId())); 

                    // cout << "euDist of " << (*it)->getId() << " " << euDist << endl;

                    if(++count_items >= *M) {
                        stop_searching = true;
                        break;
                    }
                        
                }

                // keep the k best from the bucket.
                int item = 0 ;
                for (auto it = distance_map.cbegin(); it != distance_map.cend(); ++it) {
                    k_nearest_map.insert(pair<double, int>((*it).first,(*it).second));

                    // if k reached, stop adding new neigbor
                    if(++item >= k) break;
                }
                distance_map.clear();   

                // stop searching
                if(stop_searching) {cout << "counting " << count_items << endl; *M = -1; return; }
            } 
        }

    }

    void PrintHT(){
        for (int i = 0; i < table_size; ++i) {
            cout<<"++++++ BUCKET ["<<i<<"] ++++++"<<endl;

            if(hash_table[i] != NULL)
                hash_table[i]->displayBucket();
        } 
    }

    ~HashTable() {
        // cout << "Destructing Hashtable..." << endl;
        Bucket<K> *temp = NULL;
        for(int i = 0; i < table_size; i++) {

            temp = hash_table[i];

            if(temp != NULL) {
                delete temp;

            }

            hash_table[i] = NULL;
        }
        delete[] hash_table;

        delete h_fun;
    }
};

