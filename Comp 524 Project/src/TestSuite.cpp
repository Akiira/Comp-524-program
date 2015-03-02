///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include <iostream>
#include <cassert>

TestSuite::~TestSuite(){
	delete[] edgesCovered;
	delete[] predicatesCovered;
	delete[] duplicateEdgesCovered;
	delete[] duplicatePredicatesCovered;
	delete[] testCases;
}

// Fill the new suite with random test cases and evaluate the coverage of all the test cases
TestSuite::TestSuite(int numberOfTestCases, ControlFlowGraph* targetCFG){
	assert(numberOfTestCases > 0);

	this->targetCFG = targetCFG;

	this->numberOfTestCases = numberOfTestCases;
	this->numberOfParameters = targetCFG->getNumberOfParameters();
	this->numberOfEdges = targetCFG->getNumberOfEdges();
	this->numberOfPredicates = targetCFG->getNumberOfPredicates();


	edgesCovered = new bool[numberOfEdges];
	duplicateEdgesCovered = new int[numberOfEdges];

	predicatesCovered = new bool[numberOfPredicates];
	duplicatePredicatesCovered = new int[numberOfPredicates];

	testCases = new TestCase*[numberOfTestCases];

	for(int i = 0; i < numberOfTestCases; i++){
		// Generates random test cases and stores them
		testCases[i] = new TestCase(numberOfParameters, numberOfEdges, numberOfPredicates);
		targetCFG->setCoverageOfTestCase(testCases[i]);
	}

	calculateTestSuiteCoverage();
}


TestCase** TestSuite::getAllTestCases(){
	return  testCases;
}


TestCase* TestSuite::getTestCase(int index){

	assert(index >= 0  &&  index < numberOfTestCases);

	return  testCases[index];
}

void TestSuite::print() {
	cout << "Test Suite Coverage:" << endl;
	targetCFG->printTestSuiteCoverage(this);

	cout << "Test Case Coverage:" << endl;
	for (int i = 0; i < numberOfTestCases; i++) {
		std::cout << std::endl << std::endl << "Test Case #" << i ;
		testCases[i]->print(targetCFG);
	}
}

// This code is ridiculously inefficient, maybe we can switch to bitsets instead,
//	then use |= on entire bitsets.
void TestSuite::calculateTestSuiteCoverage() {
	for (int i =0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfEdges; j++) {
			edgesCovered[j] |= testCases[i]->getEdgesCovered()[j];
		}
	}

	for (int i =0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfPredicates; j++) {
			predicatesCovered[j] |= testCases[i]->getPredicatesCovered()[j];
		}
	}

}

