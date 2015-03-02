///////////////////////////////////////////////////////////
//  ControlFlowGraph.cpp
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "ControlFlowGraph.h"


ControlFlowGraph::ControlFlowGraph(){
	coverage = 0;
	testCase = 0;
	NUM_OF_EDGES = 0;
	NUM_OF_PREDICATES = 0;
}

ControlFlowGraph::~ControlFlowGraph(){
	delete[] coverage;
	delete testCase;
}

bool** ControlFlowGraph::getCoverageOfTestCase(TestCase* testCase) {
	// Reset coverage and assign testCase ptr to the passed testCase
	for (int i = 0; i < NUM_OF_EDGES; i++) {
		coverage[0][i] = false;
	}
	for (int i = 0; i < NUM_OF_PREDICATES; i++) {
		coverage[1][i] = false;
	}
	this->testCase = testCase;

	// Run through the CFG calculating coverage as it goes.
	runTestCase();
	return coverage;
}

// Override in subclasses
void ControlFlowGraph::runTestCase(){
	return;
}
