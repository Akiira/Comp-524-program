/*
 * SimpleIfElseControlFlowGraph.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "SimpleIfElseControlFlowGraph.h"

SimpleIfElseControlFlowGraph::SimpleIfElseControlFlowGraph() {
	NUM_OF_EDGES = 4;
	NUM_OF_PREDICATES = 2;

	coverage = new bool*[2];
	coverage[0] = new bool[NUM_OF_EDGES];
	coverage[1] = new bool[NUM_OF_PREDICATES];
	testCase = 0;
}

SimpleIfElseControlFlowGraph::~SimpleIfElseControlFlowGraph() {
	// Just implicitly call ControlFlowGraph destructor
}

void SimpleIfElseControlFlowGraph::runTestCase() {
	block1();
}
void SimpleIfElseControlFlowGraph::block1() {
	int* parameters = testCase->getInputParameters();

	if(parameters[0] > 2)
	{
		coverage[0][SimpleIfElseControlFlowGraph::edges::B1toB2] = true;
		coverage[1][SimpleIfElseControlFlowGraph::predicates::B1_T] = true;
		block2();
	}
	else
	{
		coverage[0][SimpleIfElseControlFlowGraph::edges::B1toB3] = true;
		coverage[1][SimpleIfElseControlFlowGraph::predicates::B1_F] = true;
		block3();
	}
}

void SimpleIfElseControlFlowGraph::block2() {
	coverage[0][SimpleIfElseControlFlowGraph::edges::B2toB4] = true;
	block4();
}

void SimpleIfElseControlFlowGraph::block3() {
	coverage[0][SimpleIfElseControlFlowGraph::edges::B3toB4] = true;
	block4();
}

void SimpleIfElseControlFlowGraph::block4() {
	return;
}

