///////////////////////////////////////////////////////////
//  ControlFlowGraph.cpp
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall and Austin
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

void ControlFlowGraph::printTestCaseCoverage(TestCase* testCase) const{
	int* inputParameters = testCase->getInputParameters();
	bool* edgesCovered = testCase->getEdgesCovered();
	bool* predicatesCovered = testCase->getPredicatesCovered();

	printInputParameters(inputParameters);
	printEdgesCovered(edgesCovered);
	printPredicatesCovered(predicatesCovered);
}

void ControlFlowGraph::printTestSuiteCoverage(TestSuite* testSuite) const{
	int* edgesCovered = testSuite->getDuplicateEdgesCovered();
	int* predicatesCovered = testSuite->getDuplicatePredicatesCovered();

	printEdgesCovered(edgesCovered);
	printPredicatesCovered(predicatesCovered);
}

void ControlFlowGraph::printPopulationCoverage(int* edges, int* predicates) const {
	printEdgesCovered(edges);
	printPredicatesCovered(predicates);
}

ControlFlowGraph& ControlFlowGraph::operator=(const ControlFlowGraph& other)
{
	if (this != &other) {
		delete testCase;
		testCase = other.testCase;
		numberOfEdges = other.numberOfEdges;
		numberOfPredicates = other.numberOfPredicates;
		numberOfParameters = other.numberOfParameters;
	}	//if
	return *this;
}//operator=
