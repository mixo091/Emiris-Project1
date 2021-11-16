
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <bits/stdc++.h>
#include <assert.h>
#include <ctime>
#include <limits>
#include <map>
#include <string>
#include <random>
#include <chrono>	
#include <algorithm> 
#include <math.h>	
#include <fstream>
#include "../Data/Data.hpp"
#include "../LSH/lsh.hpp"
#include "../HashFunction/HashFunction.hpp"
#include "../hypercube/hypercube.hpp"

#define NONE -1
#define MIN_CHANGE 0
#define MAX_ITERATIONS 20
#define FIRST 0 
#define LAST MAX_ITERATIONS-1
#define MIDDLE (MAX_ITERATIONS - 1)/2
using namespace std;

template <typename K>
class Cluster
{   
  
    public:
    int number_of_items;
    int Cluster_Id;
    list <K*> Cluster_items; 
    vector<double> centroid;
    int CentroidId;
    Cluster(int id){
        Cluster_Id = id;
        CentroidId = NONE ;
        number_of_items = 0;
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
    int CalculateCentroid(){
        int ClusterChange = 0;
        if(Cluster_items.size()>0){
        vector<double> old_centroid = centroid;
        int dimension = centroid.size();
        for(int i =0 ; i < dimension ;i++){
            centroid.at(i) = 0.0;
            double sum  = 0.0;
            for (std::list<Data<double>*>::iterator it = Cluster_items.begin(); it != Cluster_items.end(); ++it){
                 sum = sum + (*it)->v.at(i);
            }
            centroid.at(i) = sum/number_of_items;
            ClusterChange = ClusterChange + abs(int(old_centroid.at(i)) - (sum/number_of_items));
        }
        old_centroid.clear();

        }
        //cout<<"ClusterChange:"<<ClusterChange<<endl;
        return ClusterChange;
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


        void K_meansPP()
        {    //Choose first Centroid Randomly.
            default_random_engine RandomEn(chrono::system_clock::now().time_since_epoch().count());
            int randomCentroid = rand()%TotalDataItems;
            Clusters[0]->SetCenter(DataSet[randomCentroid].v,DataSet[randomCentroid].id);
            Clusters[0]->PrintCluster();
            for (int i = 1; i < ClustersNum; i++)
            {
                //cout<<"FOR CENTROID:"<<i<<endl;
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
                //cout<<"maxDist: "<<maxDist<<endl;
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
                //cout<<"x:"<<x<<endl;

                int nextCentroid;
                for (int p = 1; p < TotalDataItems; p++)
                {
                    if (x <= P[p])
                    {       
                        nextCentroid = p;
                        break;
                    }
                }
                //cout<<"NextCentoid: "<<nextCentroid<<endl;
                Clusters[i]->SetCenter(DataSet[nextCentroid].v,DataSet[nextCentroid].id);
                
            }
       }




        double minDistBetween2centers() {
            double minDist = std::numeric_limits<double>::max();
            for(int i =0 ; i<ClustersNum ; i++){
                for(int j = i+1 ; j < ClustersNum ; j++ ){
                    double dist = euclidean_dist(DataSet[Clusters[i]->CentroidId].v, DataSet[Clusters[j]->CentroidId].v);
                    if(dist<minDist){
                        minDist = dist;
                    }
                }
            }
            return minDist ;
        }

        //=============  Loyds Clustering =============================//
        void Loyds_Clustering(const string &out_file , bool complete){
            ofstream output(out_file);
            if(!output.is_open()) {
                cout << "Error with output file\n";
                exit(-1);
            }
            int ClusterChange ;
            int TotalClusteringChange =  MIN_CHANGE + 1;

            // === Kmeans++ - Initialize Centroids === //
            K_meansPP();
            PrintClusters();
            int iteration = 0;
            auto start = high_resolution_clock::now();
            while (iteration < MAX_ITERATIONS && TotalClusteringChange > MIN_CHANGE)
            {   // cout<<"ok"<<endl;
                TotalClusteringChange = 0;
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
                    //cout<<(*it).second<<endl;
                    int ClusterToBeAssigned = (*it).second ;
                     if((iteration == FIRST) || (iteration==LAST )){
                         if(iteration==FIRST){
                            // FirstIteration<<"ITEM ID : "<<DataSet[i].id<<" --- Assigned to CLUSTER --------- "<<ClusterToBeAssigned<<endl;
                         }else{
                             //LastIteration<<"ITEM ID : "<<DataSet[i].id<<" --- Assigned to CLUSTER --------- "<<ClusterToBeAssigned<<endl;
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
                    ClusterChange =0 ;
                    ClusterChange = Clusters[i]->CalculateCentroid();
                    TotalClusteringChange += ClusterChange;
                }
                //cout<<"ClusteringChange: "<<TotalClusteringChange<<endl;
                iteration++;
                }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            //output..
            if(complete){
                output<<"Algorithm:Classic\n";
                for(int c = 0 ; c < ClustersNum ; c++){
                    output<<"Cluster-"<<c;
                    output<<" {";  
                    output<<",centroid:" ;
                    for(auto it =Clusters[c]->centroid.begin(); it < Clusters[c]->centroid.end(); it++) {
                         output << *it << " ";
                    } 
                    output<<", "<<endl;
                    //output<<Clusters[c]->id<<" ,";
                    for (std::list<Data<double>*>::iterator it = Clusters[c]->Cluster_items.begin(); it !=  Clusters[c]->Cluster_items.end(); ++it){
                        output<<(*it)->id<<" ,";
                    }
                    output<< "}"<<endl;
                }
                output<<"ClusteringTime : "<<duration.count()<<"microseconds"<<endl;
                
            }else{
                output<<"Algorithm:Classic\n";
                for(int c = 0 ; c < ClustersNum ; c++){
                    output<<"Cluster-"<<c;
                    output<<" { size:"<<Clusters[c]->number_of_items;
                    output<<",centroid:" ;
                    for(auto it =Clusters[c]->centroid.begin(); it < Clusters[c]->centroid.end(); it++) {
                         output << *it << " ";
                    } 
                    output<< "}"<<endl;
                }
                output<<"ClusteringTime : "<<duration.count()<<"microseconds"<<endl;


            }

            PrintClusters();
        }

        void Lsh_Clustering(int dimension , const string &out_file ,int L ,int k,bool complete){
            int w = 3000;  
            int ClusterChange ;
            int TotalClusteringChange =  MIN_CHANGE + 1;
            int TotalDataItems90percent = int(TotalDataItems*90/100);
            int assigned = 0;  
            //Initialize Centroids with Kmeans++ Initialazation.
            K_meansPP();
            double initialRadius =  minDistBetween2centers()/2  ; // minDist Between2centers.
            //cout<<"InitialRadius :"<<initialRadius<<endl;
            int iteration = 0;
            double R = initialRadius;
            ofstream output(out_file);
            if(!output.is_open()) {
                cout << "Error with output file\n";
                exit(-1);
            }
            output.clear();
            //cout << "Total90: "<<TotalDataItems90percent<<endl;
            //Lsh Structure with the Dataset.
            vector<int> idsInQueryBall;
            cout<<"L:"<<L<<endl;
            cout<<"k:"<<k<<endl;
            Lsh<double> lsh = Lsh<double>(L, TotalDataItems, dimension, k, w, DataSet);
            map<int, int> AssignmentsInRadius;
            auto start = high_resolution_clock::now();
            while (iteration < MAX_ITERATIONS  && assigned < TotalDataItems90percent)
            {   
                cout<<" --- ITERATION --- "<<iteration<<endl;

                //------------------------------------------------------------------//
                for(int i=0 ; i < ClustersNum ;i++){
                    if(iteration == 0 ){
                        cout<<"--- Cluster "<<i<<" --- intitial Centroid "<<Clusters[i]->CentroidId<<endl;
                    }
                    cout<<"--- Query Cluster ---"<<i<<" at Radius ---"<<R<< endl;
                    //what items the Cluster go in this Radius R //
                    idsInQueryBall=lsh.ReverseAssignment(Clusters[i]->centroid,Clusters[i]->Cluster_Id,R);
                    if (idsInQueryBall.size() >0 ){
                    cout<<"------ In Range of Cluster ---------- "<<i<<"---- iteration "<<iteration<<endl;
                    for(auto it = idsInQueryBall.begin(); it < idsInQueryBall.end(); it++) {
                        //cout <<"ID IN QURY"<< *it << " "<<idsInQueryBall.size()<<endl;
                        if(it !=  idsInQueryBall.end()){
                        int id = *it;
                        if(AssignmentsInRadius.count(id)){
                            //cout<< id << "is present! "<<endl;
                            map<int, int>::iterator it1 = AssignmentsInRadius.find(id);
                            double dist1=0.0;
                            double dist2=0.0;
                            //cout<<(*it1).second<<endl;
                            dist1 = euclidean_dist(DataSet[id-1].v,Clusters[i]->centroid);
                            dist2 = euclidean_dist(DataSet[id-1].v,Clusters[(*it1).second]->centroid);
                            //cout<<"dist1:"<<dist1<<" dist2:"<<dist2<<endl;
                            if(dist1>dist2){
                                AssignmentsInRadius.erase(id);
                                AssignmentsInRadius.insert(pair<int, int>(id, i));     
                            }
                            
                        }else{
                            AssignmentsInRadius.insert(pair<int, int>(id, i)); 
                        }

                        }                  
                    }
                    cout<<"\n ______________________________________________________________________"<<endl;
                    }
                idsInQueryBall.clear();
            //---------------------------------------------------------------//  
            }
 
                map<int, int>::iterator it2 = AssignmentsInRadius.begin();
                while (it2 != AssignmentsInRadius.end())
                {       //cout<<"Iteration_ "<<iteration<<" --- Assignment in Radius :"<<endl;
                        int item_id = (*it2).first;
                        int cluster_num = (*it2).second;
                        //cout<<"Item_id_"<<item_id <<" && cluster_num_"<< cluster_num<<endl; 
                        Clusters[cluster_num]->InsertItem(&DataSet[(*it2).first]);
                        assigned++;
                        //markIt
                        //cout<<"ok"<<endl;


                        it2++;
                }
                //cout<<"ok"<<endl;
                AssignmentsInRadius.clear();
                //cout<<"ok"<<endl;
                //for(int c =0 ; c< ClustersNum; c++){
                   // ClusterChange = Clusters[c]->CalculateCentroid();
                //}
                iteration++;
                R=R*2;
                //cout<<"ok"<<endl;
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            cout<<"assigned"<<assigned<<endl;
            if(complete){
                output<<"Algorithm:Range Search LSH\n";
                for(int c = 0 ; c < ClustersNum ; c++){
                    output<<"Cluster-"<<c;
                    output<<" {";  
                    output<<",centroid:" ;
                    for(auto it =Clusters[c]->centroid.begin(); it < Clusters[c]->centroid.end(); it++) {
                         output << *it << " ";
                    } 
                    output<<", "<<endl;
                    for (std::list<Data<double>*>::iterator it = Clusters[c]->Cluster_items.begin(); it !=  Clusters[c]->Cluster_items.end(); ++it){
                        output<<(*it)->id<<" ,";
                    }
                    output<< "}"<<endl;
                    
                }
                output<<"ClusteringTime : "<<duration.count()<<"microseconds"<<endl;
                
            }else if(complete == false){
                output<<"Algorithm:Range Search LSH\n";
                for(int c = 0 ; c < ClustersNum ; c++){
                    output<<"Cluster-"<<c;
                    output<<" { size:"<<Clusters[c]->number_of_items;
                    output<<",centroid:" ;
                    for(auto it =Clusters[c]->centroid.begin(); it < Clusters[c]->centroid.end(); it++) {
                         output << *it << " ";
                    } 
                    output<< "}"<<endl;
                }
                 output<<"ClusteringTime : "<<duration.count()<<"microseconds"<<endl;
            }
            PrintClusters();
            return;
        }



    // ============== hyperCube Clustering ====================//
        void Hcube_Clustering(int dimension){
            int w = 4000;  
            int M = 100;
            int k = 13 ;
            int probes = 3;
            int ClusterChange ;
            int TotalDataItems90percent = int(TotalDataItems*90/100);
            int assigned = 0;  
            //Initialize Centroids with Kmeans++ Initialazation.
            K_meansPP();
            double initialRadius =  minDistBetween2centers()/2  ; // minDist Between2centers.
            cout<<"InitialRadius :"<<initialRadius<<endl;
            int iteration = 0;
            double R = initialRadius;
            //cout << "Total90: "<<TotalDataItems90percent<<endl;
            
            //Lsh Structure with the Dataset.
            vector<int> idsInQueryBall;
            //INIT H CUBE 

            hypercube<double> cube = hypercube<double>(probes, M, w, k, dimension, TotalDataItems, DataSet);

            //-----------------------------------------//

            while (iteration < MAX_ITERATIONS &&  assigned < TotalDataItems90percent){
               // cout<<" --- ITERATION --- "<<iteration<<endl;
                for(int i=0 ; i < ClustersNum ;i++){
                    if(iteration == 0 ){
                       // cout<<"--- Cluster "<<i<<" --- intitial Centroid "<<Clusters[i]->CentroidId<<endl;
                    }
                    //cout<<"--- Query Cluster ---"<<i<<" at Radius ---"<<R<< endl;


                    idsInQueryBall=cube.ReverseAssignment(Clusters[i]->centroid,Clusters[i]->CentroidId ,R,probes , M );

                     for(auto it = idsInQueryBall.begin(); it < idsInQueryBall.end(); it++) {
                        cout << *it << " ";}
                    // cout<<"\n ______________________________________________________________________"<<endl;
                }
                idsInQueryBall.clear();
                iteration++;
                R=R*2;
            }
        return ;
        }

};