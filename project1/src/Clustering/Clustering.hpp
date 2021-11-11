#pragma once 
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <bits/stdc++.h>
#include <assert.h>
#include <ctime>
#include <map>
#include <string>
#include <random>
#include <chrono>	
#include <algorithm> 
#include <math.h>	
#include <fstream>
#include "../Data/Data.hpp"
#define NONE -1
#define MAX_ITERATIONS 200
#define FIRST 0 
#define LAST MAX_ITERATIONS-1
#define MIDDLE (MAX_ITERATIONS - 1)/2
using namespace std;
template<typename K>
double euclidean_dist(const K &v1, const K &v2) 
{
    double dist = 0;

    for(unsigned int i = 0; i < v1.size(); i++) 
        dist += pow(v1[i] - v2[i],  2);
    
    return sqrt(dist);
}

template <typename K>
class Cluster
{   
    int Cluster_Id;
    
    int number_of_items;
    list <K*> Cluster_items; 
public:
    vector<double> centroid;
    int CentroidId;
    Cluster(int id){
        Cluster_Id = id;
        CentroidId = NONE ;
    }
    void PrintCluster(){
        cout<<"CLUSTER["<<Cluster_Id<<"]:"<<endl;
        cout<<" INFO:"<<endl;
        cout<<"  CENTROID: "<<endl;
        cout<<"   ID:["<<CentroidId<<"]\n   VAVLUE:[";
        for(auto it = centroid.begin(); it < centroid.end(); it++) {
            cout << *it << " ";
        } 
        cout <<"]"<<endl;
        cout<<     "ITEMS:"<<number_of_items<<endl;
        cout<<"     ITEMS ID[";
        for (std::list<Data<double>*>::iterator it = Cluster_items.begin(); it != Cluster_items.end(); ++it){
             cout<<(*it)->id<<" ,";
        }
         

        cout <<"]"<<endl;
    }
    void InsertItem(Data<double> *Item) {
        Cluster_items.push_back(Item);
        number_of_items++;
    }
    void SetCenter(vector<double>  newCenter,int id ){
        centroid = newCenter;
        CentroidId = id ;
    }
    vector<double> GetCenter(){
        return centroid;
    }
    int GetId(){
        return  Cluster_Id;
    }    

    void ClearCluster(){
        //Clear assigned Items.
        Cluster_items.clear();
        number_of_items = 0;
    }
    void CalculateCentroid(){

        int dimension = centroid.size();
        for(int i =0 ; i < dimension ;i++){
            centroid.at(i) = 0.0;
            double sum  = 0.0;
            for (std::list<Data<double>*>::iterator it = Cluster_items.begin(); it != Cluster_items.end(); ++it){
                 sum = sum + (*it)->v.at(i);
            }
            centroid.at(i) = sum/number_of_items;
   
        }
    }

};



//---Implementing Clustering Structure---//

template <typename K>
class Clustering {

    Cluster<K>** Clusters;
    int ClustersNum;
    Data<double> * DataSet;
    int TotalDataItems ; 
    public:
        Clustering(int k , Data<double> * dataSet ,int TotalVectors){
            DataSet = dataSet;
            TotalDataItems = TotalVectors;
            ClustersNum = k;
            Clusters = new Cluster<K>*[ClustersNum];
            for(int i = 0 ; i<k ; i++ )
                Clusters[i] = new Cluster<Data<double>>(i);

        }

        void PrintData(){
            for (int i = 0; i < TotalDataItems; i++) 
                DataSet[i].printVector();
        }


        void PrintClusters(){
            for(int i = 0 ; i < ClustersNum ; i++)
                Clusters[i]->PrintCluster();
        }

        void ClearClusters(){
            for(int i = 0 ; i < ClustersNum ; i++)
                Clusters[i]->ClearCluster();
        }

         

        void GenerateCentroids(){
            //=== Generate K random Centroids from the dataset.
            srand(time(NULL));
            for (int i = 0; i < ClustersNum; i++){
                int randomCentroid = rand() % (TotalDataItems- 1) + 0;
                Clusters[i]->SetCenter(DataSet[randomCentroid].v,DataSet[randomCentroid].id);
            }
        }


        void K_means_plusplus()
       {    //Choose first Centroid Randomly.
            default_random_engine RandomEn(chrono::system_clock::now().time_since_epoch().count());
            int randomCentroid = rand()%TotalDataItems;
            Clusters[0]->SetCenter(DataSet[randomCentroid].v,DataSet[randomCentroid].id);
            Clusters[0]->PrintCluster();
            for (int i = 1; i < ClustersNum; i++)
            {
                cout<<"FOR CENTROID:"<<i<<endl;
                double maxDist = 0;
                vector<double> P(TotalDataItems + 1); 

                for (int j = 0 ; j<TotalDataItems ;j++)
                {   
                    double Di;
                    //MinimumDistFromCentroids
                    vector<double> Distances;
                    for(int c=0; c<ClustersNum; c++){
                        if(Clusters[c]->CentroidId != NONE){
                            double dist = euclidean_dist(DataSet[j].v, DataSet[Clusters[c]->CentroidId].v);
                            Distances.push_back(dist);
                        }
                    }
                    if(Distances.size()!=0){
                        Di =  *min_element(begin(Distances), end(Distances));
                    }
                    //===============================//
                   //cout<<"Di"<<j<<" "<<Di<<endl;

                    if (Di > maxDist)
                        maxDist = Di;
                    
                }


                cout<<"maxDist: "<<maxDist<<endl;
                P[0] = 0;
                for (int j = 1; j <TotalDataItems; j++)
                {

                    double Di;
                    //MinimumDistFromCentroids
                    vector<double> Distances;
                    for(int c=0; c<ClustersNum; c++){
                        if(Clusters[c]->CentroidId != NONE){
                            double dist = euclidean_dist(DataSet[j].v, DataSet[Clusters[c]->CentroidId].v);
                            Distances.push_back(dist);
                        }
                    }
                    if(Distances.size()!=0){
                        Di =  *min_element(begin(Distances), end(Distances));
                    }
                    P[j] = pow(Di / maxDist, 2) + P[j - 1];
                    //cout<<"P["<<j<<"]:"<<P[j]<<endl;
                }

                uniform_real_distribution<double> unif(0.0, P[TotalDataItems-1]);
                double x = unif(RandomEn);
                cout<<"x:"<<x<<endl;

                int nextCentroid;
                for (int p = 1; p < TotalDataItems; p++)
                {
                    if (x <= P[p])
                    {       
                        nextCentroid = p;
                        break;
                    }
                }
                cout<<"NextCentoid: "<<nextCentroid<<endl;
                Clusters[i]->SetCenter(DataSet[nextCentroid].v,DataSet[nextCentroid].id);
                
            }
       }







        void Loyds_Clustering(){
            ofstream FirstIteration("iteration0.txt");
            ofstream LastIteration("iterationN.txt");
            FirstIteration.clear();
            LastIteration.clear();
            // === Kmeans++ - Initialize Centroids === //
            K_means_plusplus();
            PrintClusters();

            /*
            GenerateCentroids();
            int iteration = 0;
            bool StopCreteria = false;
            while (iteration < MAX_ITERATIONS)
            {   // cout<<"ok"<<endl;
                if((iteration == FIRST) || (iteration == MIDDLE) || (iteration==LAST))
                    cout<<"ITERATION "<<iteration<<endl;
                // === Clear Clusters === // 
                //cout<<"Clearing Clusters ..."<<endl;
                ClearClusters();
                //cout<<"TOTAL:"<<TotalDataItems<<endl;
                // === Reassign the DataSet to the Clusters. === //
                for (int i = 0; i < TotalDataItems; i++)
                {
                   //=== Calculate Distance from Each Cluster (Centroid). === //
                    map<double, int> L2fromClusters; 
                    for(int j = 0; j < ClustersNum; j++)
                    {   
                        double euDist = euclidean_dist(DataSet[i].v,Clusters[j]->centroid);
                        L2fromClusters.insert(pair<double, int>(euDist, j));
                    }
                    //Assign to Apropriate Cluster Based on L2 ===//
                    auto it = L2fromClusters.cbegin();
                    cout<<(*it).second<<endl;
                    int ClusterToBeAssigned = (*it).second ;
                     if((iteration == FIRST) || (iteration==LAST)){
                         if(iteration==FIRST){
                             FirstIteration<<"ITEM ID : "<<DataSet[i].id<<" --- Assigned to CLUSTER --------- "<<ClusterToBeAssigned<<endl;
                         }else{
                             LastIteration<<"ITEM ID : "<<DataSet[i].id<<" --- Assigned to CLUSTER --------- "<<ClusterToBeAssigned<<endl;
                         }

                     }


                   // cout<<"ITEM ID : "<<DataSet[i].id<<" --- Assigned to CLUSTER --------- "<<ClusterToBeAssigned<<endl;
                    //cout<<"ClusterToBeAssigned: "<<ClusterToBeAssigned<<endl;
                    Clusters[ClusterToBeAssigned]->InsertItem(&DataSet[i]);
                    //Clear the map to use it foe the next point 
                    L2fromClusters.clear();

                    
                }
                // === Recalculate Centroids Based  on the Reassignment. ===//
                for(int i =0 ; i< ClustersNum; i++){
                    Clusters[i]->CalculateCentroid();
                }
                //PrintClusters();
                
                iteration++;
                }

                FirstIteration.close();
                LastIteration.close();
            */

        }

        void Lsh_Clustering(){
            //===Choose K Centroids === //
            GenerateCentroids();

        }

        void Hcube_Clustering(){

        }

};