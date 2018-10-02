#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <cstring>
#include "line.hpp"

using namespace std;

extern void error(char *s);

void readLines(list<line> *lineList, char *file){
    
    char *lineptr = NULL;
    size_t len = 0;
    ssize_t read;

    FILE* stream = fopen(file, "r");
    if (stream == NULL)
        error("File open failed.");

    while ((read = getline(&lineptr, &len, stream)) != -1) {
        
        line *l = (line*)malloc(sizeof(line));
        if(l == NULL)
            error("Malloc failed");
        
        char *token;
        
        for(int i = 1; ; i++, lineptr = NULL){
            
            token = strtok(lineptr, " :=\n");
            
            if(token == NULL){
                // if third token (which corresponds to operator) is NULL, then we have only one token on the right side of assignment (eg. the line is a := 5)
                if(i == 3){
                    strcpy(l->op, "N");
                    l->type2 = N;
                }
                break;
            }
            
            if(i == 1){     // token on the left side of assignment
                l->leftSide = strdup(token);
                if(l->leftSide == NULL)
                    error("readLines function: reading left side of line failed.");
            }
            
            if(i == 2){     // first token on the right side of assignment
                if(isdigit(token[0])){   // number
                    l->type1 = INT;
                    l->firstArg.number = atoi(token);
                }
                else{   // varable
                    l->type1 = ID;
                    l->firstArg.str = strdup(token);
                    if(l->firstArg.str == NULL)
                        error("readLines function: reading first right token failed.");
                }
            }
            
            if(i == 3){     // operator
                strcpy(l->op, token);
            }
            
            if(i == 4){  // second token on the right side of assignment
                if(isdigit(token[0])){   // number
                    l->type2 = INT;
                    l->secondArg.number = atoi(token);
                }
                else{   // variable
                    l->type2 = ID;
                    l->secondArg.str = strdup(token);
                    if(l->secondArg.str == NULL)
                        error("readLines function: reading first right token failed.");
                }
            }
        }
        
        lineList->push_back(*l);
    }
}


void writeLines(list<line> lineList){
    
    for(line l : lineList){
        
        cout << l.leftSide << " := ";
        
        if(l.type1 == INT)
            cout << l.firstArg.number << " ";
        else
            cout << l.firstArg.str << " ";
        
        if(strcmp(l.op, "N") != 0){
            cout << l.op << " ";
            if(l.type2 == INT)
                cout << l.secondArg.number << " ";
            else
                cout << l.secondArg.str << " ";
        }
        cout << endl;
    }
}

line copyLine(line l){
    
    line newLine;
    newLine.leftSide = strdup(l.leftSide);
    newLine.type1 = l.type1;
    if(l.type1 == INT)
        newLine.firstArg.number = l.firstArg.number;
    else if(l.type1 == ID)
        newLine.firstArg.str = strdup(l.firstArg.str);
    strcpy(newLine.op, strdup(l.op));
    newLine.type2 = l.type2;
    if(l.type2 == INT)
        newLine.secondArg.number = l.secondArg.number;
    else if(l.type2 == ID)
        newLine.secondArg.str = strdup(l.secondArg.str);
    return newLine;
}

bool equalLineLists(list<line> lineList1, list<line> lineList2){
    
    list<line>::iterator iter1;
    list<line>::iterator iter2;
    
    for(iter1 = lineList1.begin(), iter2 = lineList2.begin(); iter1 != lineList1.end() || iter2 != lineList2.end(); iter1++, iter2++){
        
        if(strcmp(iter1->leftSide, iter2->leftSide))
            return false;
        
        if(iter1->type1 != iter2->type1)
            return false;
        
        if(iter1->type1 == INT){
            if(iter1->firstArg.number != iter2->firstArg.number)
                return false;
        }
        else if(iter1->type1 == ID){
            if(strcmp(iter1->firstArg.str, iter2->firstArg.str))
                return false;
        }
            
        if(strcmp(iter1->op, iter2->op))
            return false;
        
        if(strcmp(iter1->op, "N")){
            
            if(iter1->type2 != iter2->type2)
                return false;
        
            if(iter1->type2 == INT){
                if(iter1->secondArg.number != iter2->secondArg.number)
                    return false;
            }
            else if(iter1->type2 == ID){
                if(strcmp(iter1->secondArg.str, iter2->secondArg.str))
                    return false;
            }
        }
    }
    
    if(iter1 != lineList1.end() && iter2 == lineList2.end())
        return false;
    
    if(iter1 == lineList1.end() && iter2 != lineList2.end())
        return false;
    
    return true;
}