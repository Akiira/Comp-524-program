/*
 * ParameterTestCFG.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "ParameterTestCFG.h"
#include "TestCase.h"

#include <iostream>
#include <cassert>
#include <limits>

using std::endl;
using std::numeric_limits;

ParameterTestCFG::ParameterTestCFG() {
	numberOfEdges = 2;
	numberOfPredicates = 4;
	numberOfParameters = 2;

	testCase = 0;
}

ParameterTestCFG::~ParameterTestCFG() {
	// Just implicitly call ControlFlowGraph destructor
}

int ParameterTestCFG::getNumberOfEdges() {
	return numberOfEdges;
}
int ParameterTestCFG::getNumberOfPredicates() {
	return numberOfPredicates;
}
int ParameterTestCFG::getNumberOfParameters() {
	return numberOfParameters;
}
int ParameterTestCFG::getLowerBoundForParameter(int parameter) {
	return numeric_limits<int>::min();
}

int ParameterTestCFG::getUpperBoundForParameter(int parameter) {
	return numeric_limits<int>::max();
}

void ParameterTestCFG::runTestCase() {
	testCase->clearCoverage();
	block1();
}

inline void ParameterTestCFG::block1() {
	int* parameters = testCase->getInputParameters();

	if(parameters[0] > numeric_limits<int>::max() - 1000 && parameters[1] < numeric_limits<int>::min() + 1000) {
		testCase->addEdgeCoverage(edges::B1toB2);
		testCase->addPredicateCoverage(predicates::B1_TT);
	} else {
		if( parameters[0] > numeric_limits<int>::max() - 1000  ) {
			testCase->addPredicateCoverage(predicates::B1_TF);
		} else if( parameters[1] < numeric_limits<int>::min() + 1000 ) {
			testCase->addPredicateCoverage(predicates::B1_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B1_FF);
		}
		testCase->addEdgeCoverage(edges::B1toB3);
	}
}

void ParameterTestCFG::printInputParameters(int* inputParameters) const{
//	std::cout << endl << "Input Parameters" << std::endl;
//	std::cout << " x " << std::endl;
//	std::cout << "---" << std::endl;
//	std::cout << " " << inputParameters[0] << std::endl;
}
void ParameterTestCFG::printEdgesCovered(bool* edgesCovered) const{
//	std::cout << endl << "Edge Coverage" << std::endl;
//	std::cout << " B1toB2 | B1toB3 | B2toB4 | B3toB4 " << std::endl;
//	std::cout << "-----------------------------------" << std::endl;
//	for(int i = 0; i < numberOfEdges; i++)
//	{
//		std::cout << "   " << edgesCovered[i] << "    |";
//	}
//	std::cout << std::endl;
}
void ParameterTestCFG::printPredicatesCovered(bool* predicatesCovered) const{
//	std::cout << endl << "Predicate Coverage" << std::endl;
//	std::cout << " B1_T | B1_F " << std::endl;
//	std::cout << "-------------" << std::endl;
//	for(int i = 0; i < numberOfPredicates; i++)
//	{
//		std::cout << "  " << predicatesCovered[i] << "   |";
//	}
//	std::cout << std::endl;
}

void ParameterTestCFG::printEdgesCovered(int* edgesCovered) const{
//	std::cout << endl << "Edge Coverage" << std::endl;
//	std::cout << " B1toB2 | B1toB3 |  " << std::endl;
//	std::cout << "-----------------------------------" << std::endl;
//	for(int i = 0; i < numberOfEdges; i++)
//	{
//		std::cout << "   " << edgesCovered[i] << "    |";
//	}
//	std::cout << std::endl;
}
void ParameterTestCFG::printPredicatesCovered(int* predicatesCovered) const{
//	std::cout << endl << "Predicate Coverage" << std::endl;
//	std::cout << " B1_T | B1_F " << std::endl;
//	std::cout << "-------------" << std::endl;
//	for(int i = 0; i < numberOfPredicates; i++)
//	{
//		std::cout << "  " << predicatesCovered[i] << "   |";
//	}
//	std::cout << std::endl;
}
