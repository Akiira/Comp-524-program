/*
 * HiLoControlFlowGraph.h
 *
 *  Created on: Feb 28, 2015
 *      Author: amb6470
 */

#ifndef HILOCONTROLFLOWGRAPH_H_
#define HILOCONTROLFLOWGRAPH_H_

#include "ControlFlowGraph.h"
class HiLoControlFlowGraph : public ControlFlowGraph
{

	public:
		HiLoControlFlowGraph();
		virtual ~HiLoControlFlowGraph();

		bool** getCoverageOfTestCase(TestCase* testCase);

	private:
		// This enumeration type stores the edges of the Control Flow Graph (CFG)
		const int NUM_OF_BRANCHES = 16;
		struct branches {
			enum { STARTtoB1, B1toB2, B2toB3, B2toB4, B3toB10, B4toB5, B4toB6, B5toB10,
				B6toB7, B6toB8, B7toB10, B8toB9, B8toB10, B9toB10, B10toB2, B10toEND};
		};
		const int NUM_OF_PREDICATES = 14;
		struct predicates {
			enum { B2_T, B2_F, B4_T, B4_F, B6_FF, B6_FT, B6_TF, B6_TT,
						B8_T, B8_F, B10_FF, B10_FT, B10_TF, B10_TT };
		};

		// seems far more efficient to use instance variables that is allocated once
		//	then overwritten each time getCoverageOfTestTace is called
		bool** coverage;
		TestCase* testCase;

		void start();
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
};



#endif /* HILOCONTROLFLOWGRAPH_H_ */
