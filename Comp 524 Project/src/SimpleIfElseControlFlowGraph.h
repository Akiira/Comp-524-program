/*
 * SimpleIfElseControlFlowGraph.h
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#ifndef SIMPLEIFELSECONTROLFLOWGRAPH_H_
#define SIMPLEIFELSECONTROLFLOWGRAPH_H_

#include "ControlFlowGraph.h"

class SimpleIfElseControlFlowGraph: public ControlFlowGraph {
public:
	SimpleIfElseControlFlowGraph();
	virtual ~SimpleIfElseControlFlowGraph();

private:
	bool * block1(TestCase testCase);
	bool * block2(TestCase testCase);
	bool * block3(TestCase testCase);
	bool * block4(TestCase testCase);
};

#endif /* SIMPLEIFELSECONTROLFLOWGRAPH_H_ */



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

