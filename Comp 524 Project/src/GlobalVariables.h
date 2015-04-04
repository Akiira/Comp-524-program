/*
 * GlobalVariables.h
 *
 *  Created on: Mar 8, 2015
 *      Author: Austin and Randall
 */

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

#include "ControlFlowGraph.h"

extern ControlFlowGraph* targetCFG;

typedef int typeOfScaling;
enum {NONE, LINEAR, EXPONENTIAL, RANKED};

extern int NUM_OF_RANGES;
#endif /* GLOBALVARIABLES_H_ */
