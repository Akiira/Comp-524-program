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

extern bool printPopFitnessOnce;
extern bool printPopFitness;
extern bool printGenerationAndRatio;
extern bool printCoverageRatioForTScrossover;
extern bool printCoverageRatioForTScrossoverOnce;
extern bool printReplacement;

extern bool pauseOnTSCrossover;
extern bool pauseOnTCcrossover;
extern bool pauseONlocalOpt;
extern bool pause;

typedef int typeOfScaling;
enum {NONE, LINEAR, EXPONENTIAL, RANKED, SHARING};

#endif /* GLOBALVARIABLES_H_ */
