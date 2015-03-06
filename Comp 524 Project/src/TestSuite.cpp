///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

TestSuite::~TestSuite(){
	delete[] edgesCovered;
	delete[] predicatesCovered;
	delete[] duplicateEdgesCovered;
	delete[] duplicatePredicatesCovered;
	delete[] testCases;
}

// Fill the new suite with random test cases and evaluate the coverage of all the test cases
TestSuite::TestSuite(int numberOfTestCases, ControlFlowGraph* targetCFG){
	initializeMembersAndAllocateMemory(numberOfTestCases, targetCFG);
	fillTestSuiteWithRandomTestCases();
	calculateTestSuiteCoverage();
}

/** Create a new suite out of existing testCases
 * 	IMPORTANT: No copying is done here, just straight assignment of the pointer so ensure a deep copy was done
 * 	on them before calling this, should only be called from the crossover operator
 * 	(or maybe mutation or simulation in the future) where this copying is done.
 */
TestSuite::TestSuite(int numberOfTestCases, TestCase** testCases, ControlFlowGraph* targetCFG) {
	initializeMembersAndAllocateMemory(numberOfTestCases, targetCFG);
	fillTestSuiteWithExistingTestCases(testCases);
	calculateTestSuiteCoverage();
}

void TestSuite::initializeMembersAndAllocateMemory(int numberOfTestCases, ControlFlowGraph* targetCFG) {
	assert(numberOfTestCases > 0);
	this->targetCFG = targetCFG;
	this->numberOfTestCases = numberOfTestCases;
	this->numberOfParameters = targetCFG->getNumberOfParameters();
	this->numberOfEdges = targetCFG->getNumberOfEdges();
	this->numberOfPredicates = targetCFG->getNumberOfPredicates();

	//TODO Why have both boolean and int arrays?
	edgesCovered = new bool[numberOfEdges];
	duplicateEdgesCovered = new int[numberOfEdges];
	predicatesCovered = new bool[numberOfPredicates];
	duplicatePredicatesCovered = new int[numberOfPredicates];

	testCases = new TestCase*[numberOfTestCases];
}

void TestSuite::fillTestSuiteWithRandomTestCases() {
	assert(testCases != 0);
	for(int i = 0; i < numberOfTestCases; i++){
		// Generates random test cases and stores them
		testCases[i] = new TestCase(numberOfParameters, numberOfEdges, numberOfPredicates);
		targetCFG->setCoverageOfTestCase(testCases[i]);
	}
}

void TestSuite::fillTestSuiteWithExistingTestCases(TestCase** testCases) {
	assert(testCases != 0);
	this->testCases = testCases;
}

TestCase** TestSuite::getAllTestCases() const{
	return  testCases;
}


TestCase* TestSuite::getTestCase(int index){

	assert(index >= 0  &&  index < numberOfTestCases);

	return  testCases[index];
}

void TestSuite::print() {
	cout << "Test Suite Coverage:" << endl;
	targetCFG->printTestSuiteCoverage(this);

	cout << endl << "Test Case Coverage:" << endl;
	for (int i = 0; i < numberOfTestCases; i++) {
		std::cout << std::endl << "Test Case #" << i << std::endl;
		testCases[i]->print(targetCFG);
	}
}

void TestSuite::printSimple() {
	//cout << "Test Suite Coverage:" << endl;
	//targetCFG->printTestSuiteCoverage(this);

	//cout << endl << "First Parameter of Each test case:" << endl;
	for (int i = 0; i < numberOfTestCases; i++) {
		//std::cout << std::endl << "Test Case #" << i << std::endl;
		cout << testCases[i]->getInputParameterAtIndex(0) << " ";
	}
	cout << endl;
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

