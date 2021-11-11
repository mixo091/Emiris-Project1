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
        /* insertion for lsh*/ 

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
        /* insertion for hypercube */

        if(hash_table[index] != NULL) hash_table[index]->insert(key, -1);
        else {
            hash_table[index] = new Bucket<K>(index);
            hash_table[index]->insert(key, -1);
        }
    }

    void search_NN_in_radius(Data<double> *q, int current_bucket_num,
                            int probes, int *count_probes_searched,
                            int M, int *count_items_searched,
                            vector<int> &v_of_neighbors_in_radius, float radius)
    {
        /* 
        * Helper searching function to find nearest neighbors in radius.
        * Used in hypercube.
        */
        bool stop_searching = false;
        std::vector<int>::iterator vec_it;

        struct Bucket<K> *bucket = hash_table[current_bucket_num];
        if(bucket) {

            // traverse the list of Data at current bucket.
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                    
                double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                
                if(euDist < radius) {
                    // std::find function call
                    vec_it = std::find (v_of_neighbors_in_radius.begin(), v_of_neighbors_in_radius.end(), (*it)->getId());
                    if (vec_it == v_of_neighbors_in_radius.end())
                        v_of_neighbors_in_radius.push_back((*it)->getId());
                }

                if(++(*count_items_searched) >= M) {
                    stop_searching = true;
                    break;
                } 
            }
            // check the bucket in which query is hashed
            if(++(*count_probes_searched) >= probes) stop_searching = true;

            if(stop_searching) return;
        }
    }

    void search_NN_in_radius(Data<double> *q, vector<int> &v_of_neighbors_in_radius, float radius)
    {
        /* 
        * Helper searching function to find nearest neighbors in radius.
        * Used in LSH.
        */
        std::vector<int>::iterator vec_it;
        int query_trick = 0;
        int current_bucket_num = h_fun->hashValue(q->getVector(), table_size, &query_trick);

        struct Bucket<K> *bucket = hash_table[current_bucket_num];
        if(bucket) {
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                    
                double euDist = euclidean_dist(q->getVector(), (*it)->getVector());                
                if(euDist < radius) {
                    // std::find function call
                    vec_it = std::find (v_of_neighbors_in_radius.begin(), v_of_neighbors_in_radius.end(), (*it)->getId());
                    if (vec_it == v_of_neighbors_in_radius.end())
                        v_of_neighbors_in_radius.push_back((*it)->getId());
                }
            }
        }
    }

    void search_NN(Data<double> *q, map<double, int> &k_nearest_map, int k)
    {
        /* 
        * helper search function used in LSH in order to find k nearest neighbors
        * and store them in a map
        */
        map<double, int> distance_map;
        int query_trick = 0;

        int current_bucket_num = h_fun->hashValue(q->getVector(), table_size, &query_trick);

        struct Bucket<K> *bucket = hash_table[current_bucket_num];
        if(bucket) {

            // traverse the list of Data at current bucket.
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
            
                if(query_trick == (*it)->getQueryTrick()) {
                    double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                    distance_map.insert(pair<double, int>(euDist, (*it)->getId()));
                } else 
                    continue;           
            }

            // keep the k best from the bucket.
            int item = 0 ;
            for (auto it = distance_map.cbegin(); it != distance_map.cend(); ++it) {
                k_nearest_map.insert(pair<double, int>((*it).first,(*it).second));

                // if k reached, stop adding new neigbor
                if(++item >= k) break;
            }
            distance_map.clear();   
        }
    }

    void search_NN(Data<double> *q, int current_bucket_num, int M, int probes, int k,
                    int *count_items_searched, int *count_probes_searched,
                    map<double, int> &k_nearest_map)
    {
        /*  
        * helper function to find k nearest neighbors and store them in a map
        * used in hypercube
        */
        map<double, int> distance_map;
        bool stop_searching = false;

        struct Bucket<K> *bucket = hash_table[current_bucket_num];
        if(bucket) {

            // traverse the list of Data at current bucket.
            typename std::list<BucketEntry<K> *>::iterator it;
            for (it = bucket->bucket_entries.begin(); it != bucket->bucket_entries.end(); ++it) {
                    
                double euDist = euclidean_dist(q->getVector(), (*it)->getVector());
                distance_map.insert(pair<double, int>( euDist, (*it)->getId())); 

                if(++(*count_items_searched) >= M) {
                    stop_searching = true;
                    break;
                }                     
            }
            // check the bucket in which query is hashed
            if(++(*count_probes_searched) >= probes) stop_searching = true;

            // keep the k best from the bucket.
            int item = 0 ;
            for (auto it = distance_map.cbegin(); it != distance_map.cend(); ++it) {
                k_nearest_map.insert(pair<double, int>((*it).first,(*it).second));

                // if k reached, stop adding new neigbor
                if(++item >= k) break;
            }
            distance_map.clear();   

            // stop searching
            if(stop_searching) return; 
        }
    }

    void find_NN(Data<double> *q, map<double, int> &k_nearest_map,
                int k, vector<int> neighbors, int current_bucket_num, 
                int M, int probes, int *count_items_searched, int *count_probes_searched) 
    {
        /* find NN for Hypercube checking current_bucket_num at first and if needed, every other neighbor*/

        bool stop_searching = false;
        // at first search is done for current bucket
        search_NN(q, current_bucket_num, M, probes, 
                k, count_items_searched, 
                count_probes_searched, k_nearest_map);

        if(*count_items_searched >= M|| *count_probes_searched >= probes) stop_searching = true;

        // search is done for every neighbor
        for(int i = 0; i < (int)neighbors.size() && stop_searching == false; i++) {

            search_NN(q, neighbors.at(i), M, probes, k,
                    count_items_searched, count_probes_searched, 
                    k_nearest_map);

            if(*count_items_searched >= M|| *count_probes_searched >= probes) stop_searching = true;
        }
    }

    
    void find_NN(Data<double> *q, map<double, int> &k_nearest_map, int k) 
    {
        /* NN for lsh. We calculate euclidean distance with the use of query trick 
        * and we keep the best-k pair of <eu_dist, id> from every bucket  in  map
        */
        search_NN(q, k_nearest_map, k);   
        
    }

    vector<int> range_search(Data<double> *data, float radius) 
    {
        /*
        * Performing range search for LSH.
        * Returnin a vector of id's with eu_dist < radius.
        */
        vector<int> rad_nearest_neighbors;
        search_NN_in_radius(data, rad_nearest_neighbors, radius);

        return rad_nearest_neighbors;
    }

    vector<int> range_search(Data<double> *data, int bucket_num, 
                    int probes, int *count_probes_searched, 
                    int M, int *count_items_searched, 
                    float radius, vector<int> neighbors)
    {
        /*  
        *   Performing range search, adding every vertex with distance < radius in a vector of distances.
        *   Returing a vector of id's.
        *   Used in hypeprcube.
        */
        vector<int> rad_nearest_neighbors;
        bool stop_searching = false;
        
        search_NN_in_radius(data, bucket_num,
                        probes, count_probes_searched,
                        M, count_items_searched,
                        rad_nearest_neighbors, radius);

        
        if(*count_items_searched >= M|| *count_probes_searched >= probes) stop_searching = true;

        // range search is done for every neighbor
        for(size_t i = 0; i < neighbors.size() && stop_searching == false; i++) {

            search_NN_in_radius(data, bucket_num,
                        probes, count_probes_searched,
                        M, count_items_searched,
                        rad_nearest_neighbors, radius);

            if(*count_items_searched >= M|| *count_probes_searched >= probes) stop_searching = true;
        }

        return rad_nearest_neighbors;
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

