#include <cstring>
#include <math.h>
#include <map>
#include "optimization.hpp"
#include "line.hpp"

using namespace std;

void optimize(list<line> *lineList){
    
    bool loop = true;
    
    while(loop){
        
        list<line> lineListCopy;
        for(line l : *lineList){
            line copiedLine = copyLine(l);
            lineListCopy.push_back(copiedLine);
        }
        
        deadCodeElimination(lineList);
        cout<< "Dead code elimination:" << endl;
        writeLines(*lineList);
        cout << endl;
        
        algebraicTransformations(lineList);
        cout<< "Algebraic transformations:" << endl;
        writeLines(*lineList);
        cout << endl;
        
        constantPropagation(lineList);
        cout<< "Constant propagation:" << endl;
        writeLines(*lineList);
        cout << endl;
        
        copyPropagation(lineList);
        cout<< "Copy propagation:" << endl;
        writeLines(*lineList);
        cout << endl;
        
        commonSubexpressionElimination(lineList);
        cout<< "Common subexpression elimination:" << endl;
        writeLines(*lineList);
        cout << endl;
        
        if(equalLineLists(*lineList, lineListCopy))
            loop = false;
    }
    
}

void deadCodeElimination(list<line> *lineList){
    
    list<char*> variablesToErase;   // varables whose definitions need to be erased
    list<char*> variablesToStay;    // varables whose definitions need to be kept
    
    for(line l : *lineList)
        variablesToErase.push_back(l.leftSide);

    list<line>::iterator iter;
    for(iter = lineList->begin(); iter != lineList->end(); iter++){
        
        if(iter->type1 == ID){
            for(char* var : variablesToErase){
                if(!strcmp(var, iter->firstArg.str))
                    variablesToStay.push_back(var);
            }
        }
        
        if(iter->type2 == ID){
            for(char* var : variablesToErase){
            if(!strcmp(var, iter->secondArg.str))
                variablesToStay.push_back(var);
            }
        }
    }
    
    /* Adding variable from last line because it might be used afterwards */
    variablesToStay.push_back(lineList->back().leftSide);
    
    variablesToStay.unique();
    
    for(char* v : variablesToStay){
        variablesToErase.remove(v);
    }
        
    for(iter = lineList->begin(); iter != lineList->end(); iter++){
        
        for(char* var : variablesToErase){
            if(!strcmp(var, iter->leftSide))
                iter = lineList->erase(iter);
        }
    }
}


void commonSubexpressionElimination(list<line> *lineList){
    
    list<line>::iterator iter1;
    list<line>::iterator iter2;
    unsigned i = 0;
    bool equal;
    
    for(iter1 = lineList->begin(); iter1 != lineList->end(); iter1++){
        
        i++;
        
        if(strcmp(iter1->op, "N")){
            
            for(iter2 = next(lineList->begin(), i); iter2 != lineList->end(); iter2++){
        
                equal = true;
                if(strcmp(iter1->op, "N")){
                    
                    if(iter1->type1 != iter2->type1){
                        equal = false;
                        continue;
                    }
                    
                    if(iter1->type1 == INT){
                        if(iter1->firstArg.number != iter2->firstArg.number){
                            equal = false;
                            continue;
                        }
                    }
                    else if(iter1->type1 == ID){
                        if(strcmp(iter1->firstArg.str, iter2->firstArg.str)){
                            equal = false;
                            continue;
                        }
                    }
                    
                    if(strcmp(iter1->op, iter2->op)){
                        equal = false;
                        continue;
                    }
                    
                    if(iter1->type2 != iter2->type2){
                        equal = false;
                        continue;
                    }
                    
                    if(iter1->type2 == INT){
                        if(iter1->secondArg.number != iter2->secondArg.number){
                            equal = false;
                            continue;
                        }
                    }
                    else if(iter1->type2 == ID){
                        if(strcmp(iter1->secondArg.str, iter2->secondArg.str)){
                            equal = false;
                            continue;
                        }
                    }
                    
                    if(equal == true){
                        iter2->type1 = ID;
                        iter2->firstArg.str = strdup(iter1->leftSide);
                        strcpy(iter2->op, "N");
                        iter2->type2 = N;
                    }
                }
            }
        }
    }
}

void zeroOneTransformation(list<line> *lineList){
    
    list<line>::iterator iter;
    
    for(iter = lineList->begin(); iter != lineList->end(); iter++){
        
        if(iter->type1 == INT && iter->type2 == ID){
            
                if(iter->firstArg.number == 0 && !strcmp(iter->op, "+")){     // 0 + ID  ->  ID
                    iter->type1 = ID;
                    iter->type2 = N;
                    iter->firstArg.str = iter->secondArg.str;
                    strcpy(iter->op, "N");
                }
                
                if(iter->firstArg.number == 0 && (!strcmp(iter->op, "*") || !strcmp(iter->op, "/") || !strcmp(iter->op, "<<") || !strcmp(iter->op, ">>"))){     // 0 * ID -> 0,  0 / ID -> 0,  0 << ID -> 0,  0 >> ID  ->  0
                    iter->type2 = N;
                    strcpy(iter->op, "N");
                }
                
                if(iter->firstArg.number == 1 && !strcmp(iter->op, "*")){     // 1 * ID  ->  ID
                    iter->type1 = ID;
                    iter->type2 = N;
                    iter->firstArg.str = iter->secondArg.str;
                    strcpy(iter->op, "N");
                }
        }
        else if(iter->type1 == ID && iter->type2 == INT){
            
            if(iter->secondArg.number == 0 && (!strcmp(iter->op, "+") || !strcmp(iter->op, "<<") || !strcmp(iter->op, ">>"))){     // ID + 0 -> ID,  ID << 0 -> ID,  ID >> 0 -> ID
                iter->type2 = N;
                strcpy(iter->op, "N");
            }
            
            if(iter->secondArg.number == 0 && !strcmp(iter->op, "*")){     // ID * 0 -> 0
                iter->type1 = INT;
                iter->firstArg.number = 0;
                strcpy(iter->op, "N");
            }
            
            if(iter->secondArg.number == 1 && !strcmp(iter->op, "*")){    // ID * 1 -> ID
                iter->type2 = N;
                strcpy(iter->op, "N");
            }
        }
        
        if(iter->type1 == ID && iter->type2 == N){
            if(!strcmp(iter->leftSide, iter->firstArg.str)){     // x := x  -> brise se cela linija
                iter = lineList->erase(iter);
            }
        }
        
    }
}

void algebraicTransformations(list<line> *lineList){
    
    zeroOneTransformation(lineList);
    
    strengthReduction(lineList);
//     cout<< "Strength reduction:" << endl;
//     writeLines(*lineList);
//     cout << endl;
     
    constantFolding(lineList);
//     cout<< "Constant folding:" << endl;
//     writeLines(*lineList);
//     cout << endl;
}

void constantPropagation(list<line> *lineList){
    
    list<line>::iterator iter1;
    list<line>::iterator iter2;
    unsigned i =0;
    
    for(iter1 = lineList->begin(); iter1 != lineList->end(); iter1++){
        i++;
        
        if(!strcmp(iter1->op, "N") && iter1->type1 == INT){
            int num = iter1->firstArg.number;
            for(iter2 = next(lineList->begin(), i); iter2 != lineList->end(); iter2++){
                if(iter2->type1 == ID && !strcmp(iter2->firstArg.str, iter1->leftSide)){
                    iter2->type1 = INT;
                    iter2->firstArg.number = num;
                }
                if(strcmp(iter2->op, "N")){
                    if(iter2->type2 == ID && !strcmp(iter2->secondArg.str, iter1->leftSide)){
                        iter2->type2 = INT;
                        iter2->secondArg.number = num;
                    }
                }
            }
        }
    }
}

void copyPropagation(list<line> *lineList){
    
    list<line>::iterator iter1;
    list<line>::iterator iter2;
    unsigned i =0;
    
    for(iter1 = lineList->begin(); iter1 != lineList->end(); iter1++){
        i++;
        
        if(!strcmp(iter1->op, "N") && iter1->type1 == ID){
            for(iter2 = next(lineList->begin(), i); iter2 != lineList->end(); iter2++){
                if(iter2->type1 == ID && !strcmp(iter2->firstArg.str, iter1->leftSide)){
                    iter2->firstArg.str = strdup(iter1->firstArg.str);
                }
                if(strcmp(iter2->op, "N")){
                    if(iter2->type2 == ID && !strcmp(iter2->secondArg.str, iter1->leftSide))
                        iter2->secondArg.str = strdup(iter1->firstArg.str);
                }
            }
        }
    }
}

void strengthReduction(list<line> *lineList){
    
    list<line>::iterator iter;
    
    for(iter = lineList->begin(); iter != lineList->end(); iter++){
        // y := 2 * x  ->  y := x + x
        if(!strcmp(iter->op, "*") && iter->type1 == INT && iter->firstArg.number == 2){
            strcpy(iter->op, "+");
            iter->type1 = iter->type2;
            if(iter->type2 == INT)
                iter->firstArg.number = iter->secondArg.number;
            else
                iter->firstArg.str = strdup(iter->secondArg.str);
        }
        // y := x * 2  ->  y := x + x
        else if(!strcmp(iter->op, "*") && iter->type2 == INT && iter->secondArg.number == 2){
            strcpy(iter->op, "+");
            iter->type2 = iter->type1;
            if(iter->type1 == INT)
                iter->secondArg.number = iter->firstArg.number;
            else
                iter->secondArg.str = strdup(iter->firstArg.str);
        }
        // y := x ^ 2  ->  y := x * x
        else if(!strcmp(iter->op, "^") && iter->type2 == INT && iter->secondArg.number == 2){
            strcpy(iter->op, "*");
            iter->type2 = iter->type1;
            if(iter->type1 == INT)
                iter->secondArg.number = iter->firstArg.number;
            else
                iter->secondArg.str = strdup(iter->firstArg.str);
        }
    }
}

void constantFolding(list<line> *lineList){
    
    list<line>::iterator iter;
    
    for(iter = lineList->begin(); iter != lineList->end(); iter++){
        
        if(iter->type1 == INT && iter->type2 == INT){
            
            if(!strcmp(iter->op, "+"))
                iter->firstArg.number += iter->secondArg.number;
            if(!strcmp(iter->op, "-"))
                iter->firstArg.number -= iter->secondArg.number;
            if(!strcmp(iter->op, "*"))
                iter->firstArg.number *= iter->secondArg.number;
            if(!strcmp(iter->op, "/"))
                iter->firstArg.number /= iter->secondArg.number;
            if(!strcmp(iter->op, "^"))
                iter->firstArg.number = pow(iter->firstArg.number, iter->secondArg.number);
            if(!strcmp(iter->op, "<<"))
                iter->firstArg.number <<= iter->secondArg.number;
            if(!strcmp(iter->op, ">>"))
                iter->firstArg.number >>= iter->secondArg.number;
            
            strcpy(iter->op, "N");
            iter->type2 = N;
        }
    }
}