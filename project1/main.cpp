#include <iostream>
#include <vector>
#include "./VectorList/VectorList.h"
using namespace std;


int main(){

     std::cout <<"Project starts..."<<endl;


    VectorList* vector_list = VectorList_Init(4);
    vector<int> vector1 = {3,5,6,7};
    VectorList_Insert(&vector_list,vector1);
    vector<int> vector2 = {323,55,643,437};
    VectorList_Insert(&vector_list,vector2);
    vector<int> vector3 = {34,65,621,75};
    VectorList_Insert(&vector_list,vector3);
    vector<int> vector4 = {13,15,16,37};
    VectorList_Insert(&vector_list,vector4);
    vector<int> vector5 = {4243,353,56,37};
    VectorList_Insert(&vector_list,vector5);
    VectorList_Print(vector_list);


    return 0 ;
}




