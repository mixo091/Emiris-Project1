#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;
//Function to get the args for LSH.
int Handle_LSH_args ( 
    int argc, char **argv, 
    string *i_file, string *qr_file, string *o_file,
    int *k, int *L, int *N, int *R ) 
    {
    if(argc < 7) {
        cerr << "Wrong arguments. Please try again!" << endl;
        exit(0);
    }
    if(strcmp(argv[1], "-i") != 0) {    
        cerr << "Please give the input file." << endl;
        return -1;
    }
    *i_file = argv[2];
    if(strcmp(argv[3], "-q") != 0) {    
        cerr << "Please give the input file." << endl;
        return -1;
    }
    *qr_file = argv[4];
    for(int i = 5; i < argc; i++) {
        if(strcmp(argv[i], "-k") == 0) {
            *k = atoi(argv[i + 1]);
        } else if(strcmp(argv[i], "-L") == 0) {
            *L = atoi(argv[i+1]);
        } else if(strcmp(argv[i], "-N") == 0) {
            *N = atoi(argv[i+1]);
        } else if(strcmp(argv[i], "-R") == 0) {
            *R = atoi(argv[i+1]);
        } else if(strcmp(argv[i], "-o") == 0) {
            *o_file = argv[i+1];
        }
    }
    if(o_file->empty()) {
        cerr << "No output file given.";
        return -1;
    }
    return 1;
}