/*
 * SimpleIfElseControlFlowGraph.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "SimpleIfElseControlFlowGraph.h"
#include <iostream>
using std::endl;

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

void SimpleIfElseControlFlowGraph::printInputParameters(int* inputParameters) {
	std::cout << endl << "Input Parameters" << std::endl;
	std::cout << " x " << std::endl;
	std::cout << "---" << std::endl;
	std::cout << " " << inputParameters[0] << std::endl;
}
void SimpleIfElseControlFlowGraph::printEdgesCovered(bool* edgesCovered) {
	std::cout << endl << "Edge Coverage" << std::endl;
	std::cout << " B1toB2 | B1toB3 | B2toB4 | B3toB4 " << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	for(int i = 0; i < numberOfEdges; i++)
	{
		std::cout << "   " << edgesCovered[i] << "    |";
	}
	std::cout << std::endl;
}
void SimpleIfElseControlFlowGraph::printPredicatesCovered(bool* predicatesCovered) {
	std::cout << endl << "Predicate Coverage" << std::endl;
	std::cout << " B1_T | B1_F " << std::endl;
	std::cout << "-------------" << std::endl;
	for(int i = 0; i < numberOfPredicates; i++)
	{
		std::cout << "  " << predicatesCovered[i] << "   |";
	}
	std::cout << std::endl;
}

void SimpleIfElseControlFlowGraph::printEdgesCovered(int* edgesCovered) {
	std::cout << endl << "Edge Coverage" << std::endl;
	std::cout << " B1toB2 | B1toB3 | B2toB4 | B3toB4 " << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	for(int i = 0; i < numberOfEdges; i++)
	{
		std::cout << "   " << edgesCovered[i] << "    |";
	}
	std::cout << std::endl;
}
void SimpleIfElseControlFlowGraph::printPredicatesCovered(int* predicatesCovered) {
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

