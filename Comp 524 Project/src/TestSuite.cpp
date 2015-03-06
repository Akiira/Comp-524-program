///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include <iostream>
#include <cassert>
#include <cstring>

using std::cout;
using std::endl;

TestSuite::~TestSuite(){
	delete[] edgesCovered;
	delete[] predicatesCovered;
	delete[] duplicateEdgesCovered;
	delete[] duplicatePredicatesCovered;
	for(int i = 0; i < numberOfTestCases; i++){
		delete testCases[i];
	}
	delete[] testCases;
}

// Fill the new suite with random test cases and evaluate the coverage of all the test cases
TestSuite::TestSuite(int numberOfTestCases, ControlFlowGraph* targetCFG){
	initializeMembersAndAllocateMemory(numberOfTestCases, targetCFG);
	fillTestSuiteWithRandomTestCases();
	calculateTestSuiteCoverage();
}

// Create a new suite out of existing testCases, need to perform deep copy on the testCases array
TestSuite::TestSuite(int numberOfTestCases, TestCase** testCasesToCopy, ControlFlowGraph* targetCFG) {
	initializeMembersAndAllocateMemory(numberOfTestCases, targetCFG);
	fillTestSuiteWithExistingTestCases(testCasesToCopy);
	// Still have to re-evaluate the test suite but all the test cases will be the same
	// TODO rethink this because there's stil; mutation to worry about
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
	//TODO I agree we can just get by with the ints. I will remove the bools when i get a chance.
	edgesCovered = new bool[numberOfEdges];
	duplicateEdgesCovered = new int[numberOfEdges] { };
	predicatesCovered = new bool[numberOfPredicates];
	duplicatePredicatesCovered = new int[numberOfPredicates] { };

	testCases = new TestCase*[numberOfTestCases] { };
}

void TestSuite::fillTestSuiteWithRandomTestCases() {
	assert(testCases != 0);
	for(int i = 0; i < numberOfTestCases; i++){
		// Generates random test cases and stores them
		testCases[i] = new TestCase(numberOfParameters, numberOfEdges, numberOfPredicates);
		targetCFG->setCoverageOfTestCase(testCases[i]);
	}
}

void TestSuite::fillTestSuiteWithExistingTestCases(TestCase** testCasesToCopy) {
	assert(testCases != 0);
	for(int i = 0; i < numberOfTestCases; i++){
		// Call copy constructor to perform deep copy of the test cases one by one.
		testCases[i] = new TestCase(*testCasesToCopy[i]);

		// Don't need to setCoverage again because test case will have already been evaluated
		//targetCFG->setCoverageOfTestCase(testCases[i]);
	}
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

TestSuite& TestSuite::operator =(const TestSuite& other) {
	if(this != &other){
		delete[] edgesCovered;
		delete[] predicatesCovered;
		delete[] duplicateEdgesCovered;
		delete[] duplicatePredicatesCovered;

		for(int i = 0; i < numberOfTestCases; i++){
			delete testCases[i];
		}
		delete[] testCases;

		numberOfTestCases = other.numberOfTestCases;
		numberOfParameters = other.numberOfParameters;
		numberOfEdges = other.numberOfEdges;
		numberOfPredicates = other.numberOfPredicates;

		edgesCovered = new bool[numberOfEdges] { };
		duplicateEdgesCovered = new int[numberOfEdges] { };
		predicatesCovered = new bool[numberOfPredicates] { };
		duplicatePredicatesCovered = new int[numberOfPredicates] { };
		testCases = new TestCase*[numberOfTestCases] { } ;

		using std::memcpy;
		memcpy(edgesCovered, other.edgesCovered, sizeof(numberOfEdges * sizeof(bool)));
		memcpy(duplicateEdgesCovered, other.duplicateEdgesCovered, sizeof(numberOfEdges * sizeof(int)));
		memcpy(predicatesCovered, other.predicatesCovered, sizeof(numberOfEdges * sizeof(bool)));
		memcpy(duplicatePredicatesCovered, other.duplicatePredicatesCovered, sizeof(numberOfEdges * sizeof(int)));

		for(int i = 0; i < numberOfTestCases; i++){
			testCases[i] = new TestCase { *other.testCases[i] };
		}
	}

	return *this;
}
