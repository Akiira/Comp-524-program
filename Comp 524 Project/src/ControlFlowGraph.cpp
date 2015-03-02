///////////////////////////////////////////////////////////
//  ControlFlowGraph.cpp
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "ControlFlowGraph.h"
#include <iostream>



ControlFlowGraph::~ControlFlowGraph(){
	delete testCase;
}

void ControlFlowGraph::setCoverageOfTestCase(TestCase* testCase) {
	// Reset coverage and assign testCase ptr to the passed testCase
	this->testCase = testCase;

	// Run through the CFG calculating coverage as it goes.
	runTestCase();
}

void ControlFlowGraph::printTestCaseCoverage(TestCase* testCase){
	int* inputParameters = testCase->getInputParameters();
	bool* edgesCovered = testCase->getEdgesCovered();
	bool* predicatesCovered = testCase->getPredicatesCovered();

	printInputParameters(inputParameters);
	printEdgesCovered(edgesCovered);
	printPredicatesCovered(predicatesCovered);
}

void ControlFlowGraph::printTestSuiteCoverage(TestSuite* testSuite){
	bool* edgesCovered = testSuite->getEdgesCovered();
	bool* predicatesCovered = testSuite->getPredicatesCovered();

	printEdgesCovered(edgesCovered);
	printPredicatesCovered(predicatesCovered);
}
