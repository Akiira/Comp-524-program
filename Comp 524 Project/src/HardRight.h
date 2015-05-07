/*
 * HardCFG.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Austin and Randall
 */

#ifndef MedHardCFG_H_
#define MedHardCFG_H_

#include "ControlFlowGraph.h"

class HardRight: public ControlFlowGraph {
public:
	HardRight();
	virtual ~HardRight();

	int getLowerBoundForParameter(int);
	int getUpperBoundForParameter(int);

	struct edges { enum { B0toB1, B1toB3, B1toB4, B3toB7, B3toB8, B4toB9, B4toB10, B7toB15,
						  B7toB16, B8toB17, B8toB18, B9toB19, B9toB20, B10toB21, B10toB22,
						  B15toB31, B15toB32, B16toB33, B16toB34, B16toB35, B16toB36,
						  B17toB37, B17toB38, B18toB39, B18toB40, B18toB41, B18toB42,
						  B19toB43, B19toB44, B20toB45, B20toB46, B20toB47, B20toB48,
						  B21toB49, B21toB50, B22toB51, B22toB52, B22toB53, B22toB54
	}; };

	struct predicates { enum {
	B0_T,
	B1_T, B1_F,
	B3_T, B3_F,
	B4_T, B4_F,
	B7_T, B7_F,
	B8_T, B8_F,
	B9_T, B9_F,
	B10_T, B10_F,
	// 15 + 16 + 48
	B15_TT, B15_FT, B15_TF, B15_FF,
	B16A_TT, B16A_FT, B16A_TF, B16A_FF, B16B_TT, B16B_FT, B16B_TF, B16B_FF, B16C_TT, B16C_FT, B16C_TF, B16C_FF,
	B17_TT, B17_FT, B17_TF, B17_FF,
	B18A_TT, B18A_FT, B18A_TF, B18A_FF, B18B_TT, B18B_FT, B18B_TF, B18B_FF, B18C_TT, B18C_FT, B18C_TF, B18C_FF,
	B19_TT, B19_FT, B19_TF, B19_FF,
	B20A_TT, B20A_FT, B20A_TF, B20A_FF, B20B_TT, B20B_FT, B20B_TF, B20B_FF, B20C_TT, B20C_FT, B20C_TF, B20C_FF,
	B21_TT, B21_FT, B21_TF, B21_FF,
	B22A_TT, B22A_FT, B22A_TF, B22A_FF, B22B_TT, B22B_FT, B22B_TF, B22B_FF, B22C_TT, B22C_FT, B22C_TF, B22C_FF
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

#endif /* HARDCFG_H_ */
