#pragma once

#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

//Represents a Data_point in space.
template <typename T>
struct Data
{
    long int id;
    vector<T> v;

    Data() {
        id = -1;
    }
    Data(const Data<T> &obj) : id(obj.id), v(obj.v) {}
    Data(const long int &item_id, const T &key ) : id(item_id) { v.push_back(key); }
    ~Data() { v.clear(); }
    void setId(const long int &item_id) { id = item_id; }
    void setVector( const T &d) {  v.push_back(d); }
    vector<T> getVector() { return v; }
    long int getId() { return id; }
    void printVector() {
        cout << "id = " << id << endl;
        for(auto it = v.begin(); it < v.end(); it++) {
            cout << *it << ",";
        } 
        cout << endl;
    }
};