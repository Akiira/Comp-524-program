/*
 * SimpleIfElseControlFlowGraph.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "SimpleIfElseControlFlowGraph.h"

SimpleIfElseControlFlowGraph::SimpleIfElseControlFlowGraph() {
	numberOfEdges = 4;
	numberOfPredicates = 2;

	coverage = new bool*[2];
	coverage[0] = new bool[numberOfEdges];
	coverage[1] = new bool[numberOfPredicates];
	testCase = 0;
}

SimpleIfElseControlFlowGraph::~SimpleIfElseControlFlowGraph() {
	// Just implicitly call ControlFlowGraph destructor
}

int SimpleIfElseControlFlowGraph::getNumberOfEdges() {
	return numberOfEdges;
}
int SimpleIfElseControlFlowGraph::getNumberOfPredicates() {
	return numberOfPredicates;
}

void SimpleIfElseControlFlowGraph::runTestCase() {
	block1();
}
void SimpleIfElseControlFlowGraph::block1() {
	int* parameters = testCase->getInputParameters();
	std::cout << parameters[0] << std::endl;
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

