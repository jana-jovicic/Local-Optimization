#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <list>
#include "line.hpp"

using namespace std;

void optimize(list<line> *lineList);

void algebraicTransformations(list<line> *lineList);
void zeroOneTransformation(list<line> *lineList);
void strengthReduction(list<line> *lineList);
void constantFolding(list<line> *lineList);
void constantPropagation(list<line> *lineList);
void copyPropagation(list<line> *lineList);
void commonSubexpressionElimination(list<line> *lineList);
void deadCodeElimination(list<line> *lineList);

#endif