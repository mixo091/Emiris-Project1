#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include<bits/stdc++.h>
#include "../VectorList/VectorList.h"

using namespace std;
//A bucket is a list of points in space 
//In this case as in LSH we keep the points many times
//we will have a bucket as a list of pointers in points (vectors)
typedef struct Bucket {
    list <VectorNode*> bucket;
    int BucketSize = 0;
    int id;
} Bucket;
 
class HashTable
{   
    int num_of_buckets;   /*size of the HT */   
    Bucket* hash_table ; /* pointer to the HT */ 

public:
    HashTable(int ht_size);  // Constructor
    void insertPoint(VectorNode* point); //insert point to the HT
    void DisplayHT();
};