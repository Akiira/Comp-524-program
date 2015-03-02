///////////////////////////////////////////////////////////
//  ControlFlowGraph.h
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
#define EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_

#include "TestCase.h"

class ControlFlowGraph
{

public:
	ControlFlowGraph();
	virtual ~ControlFlowGraph();

	bool* getBranchEdgesCoveredByTestCase(TestCase testCase);

private:
	bool * block1(TestCase testCase);
	bool * block2(TestCase testCase);
	bool * block3(TestCase testCase);
	bool * block4(TestCase testCase);

	int* predicates;

};


/*
 * First we will use this toy example while programming our project
 *  it has 2 possible branches.
 *
 * 	   Start
 * 		 |
 * 		 |
 *      x > 2
 *      / \
 *     /   \
 *    /     \
 *  x++      x--
 *    \      /
 *     \    /
 *      \  /
 *     print x
 *
 */

#endif // !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
