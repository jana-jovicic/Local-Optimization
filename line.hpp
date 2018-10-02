#ifndef LINE_H
#define LINE_H

#include <string>
#include <list>
#include <iostream>
using namespace std;

enum type {INT,
           ID,
           N    // NONE, it will be used to mark that there is no second argument in expression
};

struct line {
    char* leftSide;     // left side of assinment :=
    type type1, type2;
    union{
        int number;
        char* str;
    }firstArg, secondArg;
    char op[3];     // operator or "N" if there is no operator in expression
};

void readLines(list<line> *lineList, char *file);
void writeLines(list<line> lineList);
line copyLine(line l);
bool equalLineLists(list<line> lineList1, list<line> lineList2);

#endif
