/*
 * TriangleProblemGraph.h
 *
 *  Created on: Mar 21, 2015
 *      Author: Kitty
 */

#ifndef TRIANGLEPROBLEMGRAPH_H_
#define TRIANGLEPROBLEMGRAPH_H_

#include "ControlFlowGraph.h"

class TriangleProblemGraph: public ControlFlowGraph {
public:
	TriangleProblemGraph();
	virtual ~TriangleProblemGraph();

	int getLowerBoundForParameter(int);
	int getUpperBoundForParameter(int);

	struct edges { enum { B0toB1, B0toB2, B2toB3, B2toB4, B4toB5, B5toB6,
		B5toB7, B4toB8, B8toB9, B8toB10, B10toB11, B10toB12 }; };
	struct predicates { enum { B0_TTT, B0_TTF, B0_TFT, B0_FTT, B0_TFF, B0_FFT, B0_FTF, B0_FFF,
							   B2_TTT, B2_TTF, B2_TFT, B2_FTT, B2_TFF, B2_FFT, B2_FTF, B2_FFF,
							   B4_T, B4_F, B5_T, B5_F, B8_T, B8_F, B10_T, B10_F}; };

private:
	int rangeForEachParameter[3][2];
	int A;
	int B;
	int C;

	//http://www.ercim.eu/publication/Ercim_News/enw58/diaz1.gif

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

};

#endif /* TRIANGLEPROBLEMGRAPH_H_ */
