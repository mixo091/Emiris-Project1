#include <iostream>
#include <vector>
using namespace std;

//Data Structure - VectorList -> A list of Vectors (In this case points in d-space).
//VectorNode.
typedef struct VectorNode {
    struct VectorNode *next;
    vector<int> v;
}VectorNode;

VectorNode* Create_VectorNode(vector<int> new_vector, VectorNode *next);
void Print_VectorNode(VectorNode *ptr_to_vector_node);

//VectorList 
typedef struct VectorList {
    VectorNode *head;
    int size;
    int dimension;
} VectorList;

VectorList* VectorList_Init(int Dimension);
void VectorList_Print(VectorList* list);
void VectorList_Insert(VectorList** list,vector<int> new_vector);





