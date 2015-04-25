/*
 * HardCFG.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Austin and Randall
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

	struct edges { enum { B0toB1, B1toB3, B1toB4, B3toB7, B3toB8, B4toB9, B4toB10, B7toB15,
						  B7toB16, B8toB17, B8toB18, B9toB19, B9toB20, B10toB21, B10toB22,
						  B15toB31, B15toB32, B16toB33, B16toB34, B16toB35, B16toB36,
						  B17toB37, B17toB38, B18toB39, B18toB40, B18toB41, B18toB42,
						  B19toB43, B19toB44, B20toB45, B20toB46, B20toB47, B20toB48,
						  B21toB49, B21toB50, B22toB51, B22toB52, B22toB53, B22toB54,

						  B0toB2, B2toB5, B2toB6, B5toB11, B5toB12, B6toB13, B6toB14, B11toB23,
						  B11toB24, B12toB25, B12toB26, B13toB27, B13toB28, B14toB29, B14toB30,
						  B23toB55, B23toB56, B24toB57, B24toB58, B24toB59, B24toB60,
						  B25toB61, B25toB62, B26toB63, B26toB64, B26toB65, B26toB66,
						  B27toB67, B27toB68, B28toB69, B28toB70, B28toB71, B28toB72,
						  B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78
	}; };

	struct predicates { enum {
	B0_T, B0_F,
	B1_T, B1_F,
	B2_T, B2_F,
	B3_T, B3_F,
	B4_T, B4_F,
	B5_T, B5_F,
	B6_T, B6_F,
	B7_T, B7_F,
	B8_T, B8_F,
	B9_T, B9_F,
	B10_T, B10_F,
	B11_T, B11_F,
	B12_T, B12_F,
	B13_T, B13_F,
	B14_T, B14_F,
	//30 + 8*4 + + 4*10 + 4*12 = 150
	B15_TT, B15_FT, B15_TF, B15_FF,
	B16A_TT, B16A_FT, B16A_TF, B16A_FF, B16B_TT, B16B_FT, B16B_TF, B16B_FF, B16C_TT, B16C_FT, B16C_TF, B16C_FF,
	B17_TT, B17_FT, B17_TF, B17_FF,
	B18A_TT, B18A_FT, B18A_TF, B18A_FF, B18B_TT, B18B_FT, B18B_TF, B18B_FF, B18C_TT, B18C_FT, B18C_TF, B18C_FF,
	B19_TT, B19_FT, B19_TF, B19_FF,
	B20A_TT, B20A_FT, B20A_TF, B20A_FF, B20B_TT, B20B_FT, B20B_TF, B20B_FF, B20C_TT, B20C_FT, B20C_TF, B20C_FF,
	B21_TT, B21_FT, B21_TF, B21_FF,
	B22A_TT, B22A_FT, B22A_TF, B22A_FF, B22B_TT, B22B_FT, B22B_TF, B22B_FF, B22C_TT, B22C_FT, B22C_TF, B22C_FF,
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
	int rangeForEachParameter[10][2];

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
