#include <string>
#include <list>
#include <map>
#include <cstring>
#include "line.hpp"
#include "ssa.hpp"

using namespace std;

map<char*, int> variableNamesCounter;

extern void error(char *s);

void ssa(list<line> *lineList){
    
    list<line>::iterator iter1;
    list<line>::iterator iter2;
    list<line>::iterator iter3;
    unsigned i = 0;
    
    for(iter1 = lineList->begin(); iter1 != lineList->end(); iter1++){
        
        i++;
        char* goalVariable = iter1->leftSide;
        char* oldGoalVariable = strdup(goalVariable);
        variableNamesCounter[oldGoalVariable] = 1;
        bool goalRenamed = false;
        unsigned j = i;
        
        /* 
           Passing through all lines after current and if there is a line in which variable on the left side
           of assignment has the same name as the variable on the left side of current line, we change names
           of these variables.
           eg:  x := 1              x1 := 1
                z := 10             z := 10
                x := 6      ->      x2 := 6
                x := 2              x3 := 2
        */
        for(iter2 = next(lineList->begin(), i); iter2 != lineList->end(); iter2++){
            j++;
            char *tmpVariable = iter2->leftSide;
            if(strcmp(oldGoalVariable, tmpVariable) == 0){
                if(goalRenamed == false){
                    sprintf(goalVariable, "%s%d", oldGoalVariable, variableNamesCounter[oldGoalVariable]);
                    goalRenamed = true;
                    variableNamesCounter[oldGoalVariable]++;
                }
                sprintf(tmpVariable, "%s%d", oldGoalVariable, variableNamesCounter[oldGoalVariable]);
                variableNamesCounter[oldGoalVariable]++;
                
                
                /* 
                   Canging names of appropriate variables on the right side of assignment between two assignments to the same variable
                   eg:      x := 1                  x1 := 1
                            a := x + 2      ->      a := x1 + 2
                            b := 3 - x              b := 3 - x1
                            x := 6                  x2 := 6
                            y := x + 1              y := x + 1      // TODO: from last assignment to the end of code
                */
                for(iter3 = next(lineList->begin(), i); iter3 != next(lineList->begin(), j); iter3++){
                    if(iter3->type1 == ID && strcmp(iter3->firstArg.str, oldGoalVariable) == 0){
                        sprintf(iter3->firstArg.str, "%s%d", oldGoalVariable, variableNamesCounter[oldGoalVariable]-2);
                    }
                    if(iter3->type2 == ID && strcmp(iter3->secondArg.str, oldGoalVariable) == 0){
                        sprintf(iter3->secondArg.str, "%s%d", oldGoalVariable, variableNamesCounter[oldGoalVariable]-2);
                    }
                }
            }
        }
    }
}
