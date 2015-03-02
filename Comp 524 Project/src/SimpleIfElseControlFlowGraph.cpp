/*
 * SimpleIfElseControlFlowGraph.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "SimpleIfElseControlFlowGraph.h"
#include <iostream>
SimpleIfElseControlFlowGraph::SimpleIfElseControlFlowGraph() {
	numberOfEdges = 4;
	numberOfPredicates = 2;
	numberOfParameters = 1;
	//coverage = new bool*[2];
	//testCase->edgesCovered = new bool[numberOfEdges];
	//testCase->predicatesCovered = new bool[numberOfPredicates];
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
int SimpleIfElseControlFlowGraph::getNumberOfParameters() {
	return numberOfParameters;
}

void SimpleIfElseControlFlowGraph::runTestCase() {
	testCase->clearCoverage();
	block1();
}
void SimpleIfElseControlFlowGraph::block1() {
	int* parameters = testCase->getInputParameters();

	if(parameters[0] > 2)
	{
		testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B1toB2] = true;
		testCase->predicatesCovered[SimpleIfElseControlFlowGraph::predicates::B1_T] = true;
		block2();
	}
	else
	{
		testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B1toB3] = true;
		testCase->predicatesCovered[SimpleIfElseControlFlowGraph::predicates::B1_F] = true;
		block3();
	}
}

void SimpleIfElseControlFlowGraph::block2() {
	testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B2toB4] = true;
	block4();
}

void SimpleIfElseControlFlowGraph::block3() {
	testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B3toB4] = true;
	block4();
}

void SimpleIfElseControlFlowGraph::block4() {
	return;
}

