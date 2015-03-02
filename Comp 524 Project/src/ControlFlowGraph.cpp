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
	numberOfEdges = 0;
	numberOfPredicates = 0;
}


ControlFlowGraph::~ControlFlowGraph(){
	delete[] coverage;
	delete testCase;
}

bool** ControlFlowGraph::getCoverageOfTestCase(TestCase* testCase) {
	// Reset coverage and assign testCase ptr to the passed testCase
	for (int i = 0; i < numberOfEdges; i++) {
		coverage[0][i] = false;
	}
	for (int i = 0; i < numberOfPredicates; i++) {
		coverage[1][i] = false;
	}
	this->testCase = testCase;

	// Run through the CFG calculating coverage as it goes.
	runTestCase();

	// Have to make a copy of the coverage array, otherwise it will always be the same
	// Need to fix this, I'm thinking these should just be set directly on the testCase as it goes instead of storing
	//	in this temporary location inside the CFG
	bool** retval = new bool*[2];
	retval[0] = new bool[numberOfEdges];
	retval[1] = new bool[numberOfPredicates];
	for (int i = 0; i < numberOfEdges; i++) {
		retval[0][i] = coverage[0][i];
	}
	for (int i = 0; i < numberOfPredicates; i++) {
		retval[1][i]  = coverage[1][i];
	}

	return retval;
}
