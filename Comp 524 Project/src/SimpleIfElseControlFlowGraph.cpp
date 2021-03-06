/*
 * SimpleIfElseControlFlowGraph.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "SimpleIfElseControlFlowGraph.h"
#include <iostream>
#include <cassert>
#include "TestCase.h"
#include <limits>
using std::endl;
using std::numeric_limits;

SimpleIfElseControlFlowGraph::SimpleIfElseControlFlowGraph() {
	numberOfEdges = 4;
	numberOfPredicates = 2;
	numberOfParameters = 1;

	for (int i = 0; i < numberOfParameters; ++i) {
		rangeForEachParameter[i][0] = numeric_limits<int>::min();
		rangeForEachParameter[i][1] = numeric_limits<int>::max();
	}

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
int SimpleIfElseControlFlowGraph::getLowerBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < 1);
	return rangeForEachParameter[parameter][0];
}

int SimpleIfElseControlFlowGraph::getUpperBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < 1);
	return rangeForEachParameter[parameter][1];
}

void SimpleIfElseControlFlowGraph::printInputParameters(int* inputParameters) const{
	std::cout << endl << "Input Parameters" << std::endl;
	std::cout << " x " << std::endl;
	std::cout << "---" << std::endl;
	std::cout << " " << inputParameters[0] << std::endl;
}
void SimpleIfElseControlFlowGraph::printEdgesCovered(bool* edgesCovered) const{
	std::cout << endl << "Edge Coverage" << std::endl;
	std::cout << " B1toB2 | B1toB3 | B2toB4 | B3toB4 " << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	for(int i = 0; i < numberOfEdges; i++)
	{
		std::cout << "   " << edgesCovered[i] << "    |";
	}
	std::cout << std::endl;
}
void SimpleIfElseControlFlowGraph::printPredicatesCovered(bool* predicatesCovered) const{
	std::cout << endl << "Predicate Coverage" << std::endl;
	std::cout << " B1_T | B1_F " << std::endl;
	std::cout << "-------------" << std::endl;
	for(int i = 0; i < numberOfPredicates; i++)
	{
		std::cout << "  " << predicatesCovered[i] << "   |";
	}
	std::cout << std::endl;
}

void SimpleIfElseControlFlowGraph::printEdgesCovered(int* edgesCovered) const{
	std::cout << endl << "Edge Coverage" << std::endl;
	std::cout << " B1toB2 | B1toB3 | B2toB4 | B3toB4 " << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	for(int i = 0; i < numberOfEdges; i++)
	{
		std::cout << "   " << edgesCovered[i] << "    |";
	}
	std::cout << std::endl;
}
void SimpleIfElseControlFlowGraph::printPredicatesCovered(int* predicatesCovered) const{
	std::cout << endl << "Predicate Coverage" << std::endl;
	std::cout << " B1_T | B1_F " << std::endl;
	std::cout << "-------------" << std::endl;
	for(int i = 0; i < numberOfPredicates; i++)
	{
		std::cout << "  " << predicatesCovered[i] << "   |";
	}
	std::cout << std::endl;
}

void SimpleIfElseControlFlowGraph::runTestCase() {
	testCase->clearCoverage();
	block1();
}

inline void SimpleIfElseControlFlowGraph::block1() {
	int* parameters = testCase->getInputParameters();

	if(parameters[0] > 2) {
		testCase->addEdgeCoverage(edges::B1toB2);
		testCase->addPredicateCoverage(predicates::B1_T);
		block2();
	}
	else {
		testCase->addEdgeCoverage(edges::B1toB3);
		testCase->addPredicateCoverage(predicates::B1_F);
		block3();
	}
}

inline void SimpleIfElseControlFlowGraph::block2() {
	testCase->addEdgeCoverage(edges::B2toB4);
	block4();
}

inline void SimpleIfElseControlFlowGraph::block3() {
	testCase->addEdgeCoverage(edges::B3toB4);
	block4();
}

inline void SimpleIfElseControlFlowGraph::block4() {
	return;
}
