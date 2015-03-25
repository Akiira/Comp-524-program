/*
 * HardCFG.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Kitty
 */

#ifndef HARDCFG_H_
#define HARDCFG_H_

#include "ControlFlowGraph.h"

class HardCFG: public ControlFlowGraph {
public:
	HardCFG();
	virtual ~HardCFG();

	int getLowerBoundForParameter(int);
	int getUpperBoundForParameter(int);

	//TODO bottom level of tree (this CFG looks like a tree)
	struct edges { enum { B0toB1, B1toB3, B1toB4, B3toB7, B3toB8, B4toB9, B4toB10, B7toB15,
						  B7toB16, B8toB17, B8toB18, B9toB19, B9toB20, B10toB21, B10toB22,
						  B0toB2, B2toB5, B2toB6, B5toB11, B5toB12, B6toB13, B6toB14, B11toB23,
						  B11toB24, B12toB25, B12toB26, B13toB27, B13toB28, B14toB29, B14toB30 }; };

	//TODO all the predicates
	struct predicates { enum { B0_TTT, B0_TTF, B0_TFT, B0_FTT, B0_TFF, B0_FFT, B0_FTF, B0_FFF,
							   B2_TTT, B2_TTF, B2_TFT, B2_FTT,
							   B4_T, B4_F, B5_T, B5_F, B8_T, B8_F, B10_T, B10_F}; };

private:
	int rangeForEachParameter[10][2];
	double Mu, sigma;
	long long sumOfAll;
	long long sumOfFirstHalf;
	long long sumOfSecondHalf;

	void runTestCase();
	void block0();
	void block1();
	void block2();
	void block3();
	void block4();
	void block5();
	void block6();
	void block7();
	void block8();
	void block9();
	void block10();
	void block11();
	void block12();
	void block13();
	void block14();
	void block15();
	void block16();
	void block17();
	void block18();
	void block19();
	void block20();
	void block21();
	void block22();
	void block23();
	void block24();
	void block25();
	void block26();
	void block27();
	void block28();
	void block29();
	void block30();

	void printInputParameters(int* inputParameters) const;
	void printEdgesCovered(bool* edgesCovered) const;
	void printPredicatesCovered(bool* predicatesCovered) const;
	void printEdgesCovered(int* edgesCovered) const;
	void printPredicatesCovered(int* predicatesCovered) const;

	void calculateMean(const int* values);
	void calculateDeviation(const int* values);
	void calculateSums(const int* values);
};

#endif /* HARDCFG_H_ */
