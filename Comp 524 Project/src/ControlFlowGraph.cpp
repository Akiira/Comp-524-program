///////////////////////////////////////////////////////////
//  ControlFlowGraph.cpp
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "ControlFlowGraph.h"
#include <iostream>
ControlFlowGraph::ControlFlowGraph(){
	testCase = 0;
	numberOfEdges = 0;
	numberOfPredicates = 0;
	numberOfParameters = 0;
}


ControlFlowGraph::~ControlFlowGraph(){
	delete testCase;
}

void ControlFlowGraph::setCoverageOfTestCase(TestCase* testCase) {
	// Reset coverage and assign testCase ptr to the passed testCase
	this->testCase = testCase;

	// Run through the CFG calculating coverage as it goes.
	runTestCase();
}
