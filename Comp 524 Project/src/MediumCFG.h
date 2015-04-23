/*
 * HardCFG.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Austin and Randall
 */

#ifndef MediumCFG_H_
#define MediumCFG_H_

#include "ControlFlowGraph.h"

class MediumCFG: public ControlFlowGraph {
public:
	MediumCFG();
	virtual ~MediumCFG();

	int getLowerBoundForParameter(int);
	int getUpperBoundForParameter(int);

	struct edges { enum { B0toB2, B2toB5, B2toB6, B5toB11, B5toB12, B6toB13, B6toB14, B11toB23,
						  B11toB24, B12toB25, B12toB26, B13toB27, B13toB28, B14toB29, B14toB30,
						  B23toB55, B23toB56, B24toB57, B24toB58, B24toB59, B24toB60,
						  B25toB61, B25toB62, B26toB63, B26toB64, B26toB65, B26toB66,
						  B27toB67, B27toB68, B28toB69, B28toB70, B28toB71, B28toB72,
						  B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78
	}; };

	struct predicates { enum {
	B0_T, B0_F,
	B2_T, B2_F,
	B5_T, B5_F,
	B6_T, B6_F,
	B11_T, B11_F,
	B12_T, B12_F,
	B13_T, B13_F,
	B14_T, B14_F,
	B23_T, B23_F,
	B24_T, B24_F,
	B25_T, B25_F,
	B26_T, B26_F,
	B27_T, B27_F,
	B28_T, B28_F,
	B29_T, B29_F,
	B30_T, B30_F, //60 + 8*4 + + 4*10 + 4*12 = 180
	B23_TT, B23_FT, B23_TF, B23_FF,
	B24A_TT, B24A_FT, B24A_TF, B24A_FF, B24B_TT, B24B_FT, B24B_TF, B24B_FF, B24C_T, B24C_F,
	B25_TT, B25_FT, B25_TF, B25_FF,
	B26A_TT, B26A_FT, B26A_TF, B26A_FF, B26B_TT, B26B_FT, B26B_TF, B26B_FF, B26C_T, B26C_F,
	B27_TT, B27_FT, B27_TF, B27_FF,
	B28A_TT, B28A_FT, B28A_TF, B28A_FF, B28B_TT, B28B_FT, B28B_TF, B28B_FF, B28C_T, B28C_F,
	B29_TT, B29_FT, B29_TF, B29_FF,
	B30A_TT, B30A_FT, B30A_TF, B30A_FF, B30B_TT, B30B_FT, B30B_TF, B30B_FF, B30C_T, B30C_F
	}; };
	double Mu, sigma;
private:
	int rangeForEachParameter[5][2];

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

#endif /* MediumCFG_H_ */
