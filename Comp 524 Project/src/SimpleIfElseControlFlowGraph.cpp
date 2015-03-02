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
		//testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B1toB2] = true;
		testCase->addEdgeCoverage(edges::B1toB2);
		//testCase->predicatesCovered[SimpleIfElseControlFlowGraph::predicates::B1_T] = true;
		testCase->addPredicateCoverage(predicates::B1_T);
		block2();
	}
	else
	{
		//testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B1toB3] = true;
		testCase->addEdgeCoverage(edges::B1toB3);
		//testCase->predicatesCovered[SimpleIfElseControlFlowGraph::predicates::B1_F] = true;
		testCase->addPredicateCoverage(predicates::B1_F);
		block3();
	}
}

void SimpleIfElseControlFlowGraph::block2() {
	//testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B2toB4] = true;
	testCase->addEdgeCoverage(edges::B2toB4);
	block4();
}

void SimpleIfElseControlFlowGraph::block3() {
	//testCase->edgesCovered[SimpleIfElseControlFlowGraph::edges::B3toB4] = true;
	testCase->addEdgeCoverage(edges::B3toB4);
	block4();
}

void SimpleIfElseControlFlowGraph::block4() {
	return;
}

