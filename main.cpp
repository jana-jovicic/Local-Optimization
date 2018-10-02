#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "line.hpp"
#include "ssa.hpp"
#include "optimization.hpp"

using namespace std;

void error(char *s){
  cerr << s << endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv){

    if(argc != 2)
        error("Usage: ./main testFile");

    list<line> lineList;
    
    readLines(&lineList, argv[1]);
    cout << endl << "Original code:" << endl;
    writeLines(lineList);
    
    ssa(&lineList);
    cout << endl << "SSA:" << endl;
    writeLines(lineList);
    cout << endl;
    
    optimize(&lineList);
    cout << endl << "Final optimized code: " << endl;
    writeLines(lineList);
    
    return 0;
}
