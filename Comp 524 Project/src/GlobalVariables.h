/*
 * GlobalVariables.h
 *
 *  Created on: Mar 8, 2015
 *      Author: Austin and Randall
 */

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

//#include "ControlFlowGraph.h"
class RangeSet;
class ControlFlowGraph;

extern ControlFlowGraph* targetCFG;
extern RangeSet* rangeSet;

typedef int typeOfScaling;
enum {NONE, LINEAR, EXPONENTIAL, RANKED};

#endif /* GLOBALVARIABLES_H_ */
