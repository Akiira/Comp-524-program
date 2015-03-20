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

	int getNumberOfEdges();
	int getNumberOfPredicates();
	int getNumberOfParameters();
	int getLowerBoundForParameter(int);
	int getUpperBoundForParameter(int);
	TestCase* localOptVersion1(int, bool);

private:
	struct edges {
		enum {B1toB2, B1toB3, B2toB4, B3toB4};
	};
	struct predicates {
		enum { B1_T, B1_F };
	};
	int rangeForEachParameter[3][2];

	void runTestCase();
	void block1();
	void block2();
	void block3();
	void block4();

	void printInputParameters(int* inputParameters) const;
	void printEdgesCovered(bool* edgesCovered) const;
	void printPredicatesCovered(bool* predicatesCovered) const;

	void printEdgesCovered(int* edgesCovered) const;
	void printPredicatesCovered(int* predicatesCovered) const;

	// same as superclass
	//SimpleIfElseControlFlowGraph& operator=(const SimpleIfElseControlFlowGraph& other);
};


#endif /* SIMPLEIFELSECONTROLFLOWGRAPH_H_ */



/*
 * First we will use this toy example while programming our project
 *  it has 2 possible branches.
 *
 * 	   Start
 * 		 |
 * 		 |
 *      x > 2 (B1)
 *     /     \
 *    /       \
 *   /         \
 *  x++ (B2)   x-- (B3)
 *    \      /
 *     \    /
 *      \  /
 *     print x (B4)
 *       |
 *       |
 *      End
 *
 *
 */

