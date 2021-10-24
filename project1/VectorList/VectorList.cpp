#include <iostream>
#include <vector>
#include "VectorList.h"

// _______________ VectorNode functions ______________ //
VectorNode* Create_VectorNode(vector<int> new_vector, VectorNode *next)
{
    VectorNode *node =(VectorNode*) malloc(sizeof(VectorNode));
  //Using assignment operator to copy one vector to other.
    node->v=new_vector;
    node->next = next;
    return node;
}

void Print_VectorNode(vector<int> v){
    cout<<" [";
    for (auto i = v.begin(); i != v.end(); ++i)
        cout << *i << ",";
    cout<<"] ";
}

// ________________ VectorList functions _______________ //

VectorList* VectorList_Init(int Dimension){
    VectorList* list =(VectorList*)malloc(sizeof(VectorList));
    list->head = NULL;
    list->dimension = Dimension;
    list->size = 0;
    return list;
}

//Double ** because we change the list itme inside the function. 
void VectorList_Insert(VectorList** list,vector<int> new_vector){
    if ( (*list)->dimension != new_vector.size() ) {
        cout<<"ERROR:This vector can not be inserted b/c it has different dimension"<<endl;
    }
    VectorNode * new_node = Create_VectorNode(new_vector, NULL);
    new_node->next=(*list)->head;
    (*list)->head=new_node;
    (*list)->size++;
    return ;
}
  
void VectorList_Print(VectorList* list){
    cout<<"Vector List :";
    VectorNode* printer = list->head;
    for(int i =0 ; i < list->size ; i++)
    {
        Print_VectorNode(printer->v);
        printer = printer->next;
        if( (i>0) && (i % 8 == 0) ){
            cout<<endl;
        }
    }
    cout<<endl;
    return;
}