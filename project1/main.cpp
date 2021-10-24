#include <iostream>
#include <vector>
#include "./VectorList/VectorList.h"
using namespace std;


int main(){

    std::cout <<"Project starts..."<<endl;
    int Dimension = 4;
    /*Space is a list of vectors that represents 
    the points at the Space with dimension <Dimension> */ 
    VectorList* Space = VectorList_Init(Dimension);
    vector<int> vector1 = {3,5,6,7};
    VectorList_Insert(&Space,vector1);
    vector<int> vector2 = {323,55,643,437};
    VectorList_Insert(&Space,vector2);
    vector<int> vector3 = {34,65,621,75};
    VectorList_Insert(&Space,vector3);
    vector<int> vector4 = {13,15,16,37};
    VectorList_Insert(&Space,vector4);
    vector<int> vector5 = {4243,353,56,37};
    VectorList_Insert(&Space,vector5);
    VectorList_Print(Space);


    return 0 ;
}




