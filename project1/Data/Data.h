#pragma once
#include <vector>
#include <iostream>

using namespace std;
//Represents a Data_point in space.
struct Data
{
    long int id;
    vector<double> v;

    Data() {
        id = -1;
    }
    Data(const long int &item_id, const double &key ) : id(item_id) { v.push_back(key); }
    ~Data() {}
    void setId(const long int &item_id) { id = item_id; }
    void setVector( const double &d) {  v.push_back(d); }
    void printData() {
        for(auto const &i: v) {
            cout << i << " ";
        }
        cout << "\n";
    }
};