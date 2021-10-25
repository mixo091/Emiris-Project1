#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>
#include "./HashTable/HashTable.h"
#include "./Utilities/Utilities.h"
//#include "./VectorList/VectorList.h"




using namespace std;


int main(int argc, char **argv){

    string input_file, out_file, qr_file;
    int k, L, N, R;
    int totalVectors = 0; //Total points in space
    int Dimension = 0;
    std::cout <<"Project starts..."<<endl;
 
    input_file = argv[2];
    cout<<input_file<<endl;

    /*Space is a list of vectors that represents 
    the points at the Space with dimension <Dimension> */ 
    //Handling the args for The LSH. 
    int err_no = Handle_LSH_args(argc, argv, &input_file,&qr_file, &out_file, &k, &L, &N, &R);
    if(err_no <= 0 ) {
        cerr << "Something wrong with arguments!";
        exit(err_no);
    } 

    //Checking input.
    cout<<"Files :: Input file: "<<input_file<<" Output file: "<<out_file<<" Query file:"<<qr_file<<endl;
    cout<<"k:"<<k<<" L:"<<L<<" N:"<<N<<" R:"<<R<<endl;

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

    HashTable ht = HashTable(10);
    ht.DisplayHT();


    return 0 ;
}




