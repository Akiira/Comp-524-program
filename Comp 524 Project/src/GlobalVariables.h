/*
 * GlobalVariables.h
 *
 *  Created on: Mar 8, 2015
 *      Author: Austin and Randall
 */

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

class RangeSet;
class ControlFlowGraph;
class Population;

extern ControlFlowGraph* targetCFG;
extern RangeSet* rangeSet;
extern Population* globalPopulation;

typedef int typeOfScaling;
enum {NONE, LINEAR, EXPONENTIAL, RANKED, SHARING};

#endif /* GLOBALVARIABLES_H_ */
