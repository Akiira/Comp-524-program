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
		enum branches { STARTtoB1, B1toB2, B2toB3, B2toB4, B3toB10, B4toB5, B4toB6,
			B6toB7, B6toB8, B8toB9, B8toB10, B10toB2, B10toEND};

		enum predicates { B2_T, B2_F, B4_T, B4_F, B6_FF, B6_FT, B6_TF, B6_TT,
			B8_T, B8_F, B10_FF, B10_FT, B10_TF, B10_TT };


		bool** start(TestCase* testCase);
		bool** block1(TestCase* testCase);
		bool** block2(TestCase* testCase);
		bool** block3(TestCase* testCase);
		bool** block4(TestCase* testCase);
		bool** block5(TestCase* testCase);
		bool** block6(TestCase* testCase);
		bool** block7(TestCase* testCase);
		bool** block8(TestCase* testCase);
		bool** block9(TestCase* testCase);
		bool** block10(TestCase* testCase);
		bool** end(TestCase* testCase);

};



#endif /* HILOCONTROLFLOWGRAPH_H_ */
