///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include "GlobalVariables.h"
#include <iostream>
#include <cassert>
#include <cstring>

using std::cout;
using std::endl;

TestSuite::~TestSuite(){
	delete[] duplicateEdgesCovered;
	delete[] duplicatePredicatesCovered;
	for(int i = 0; i < numberOfTestCases; i++){
		delete testCases[i];
	}
	delete[] testCases;
}

TestSuite::TestSuite(const TestSuite& testSuite) {
	numberOfTestCases = testSuite.numberOfTestCases;
	numberOfParameters = testSuite.numberOfParameters;
	numberOfPredicates = testSuite.numberOfPredicates;
	numberOfEdges = testSuite.numberOfEdges;

	duplicateEdgesCovered = new int[numberOfEdges] { };
	duplicatePredicatesCovered = new int[numberOfPredicates] { };

	testCases = new TestCase*[numberOfTestCases] { };
}

// Fill the new suite with random test cases and evaluate the coverage of all the test cases
TestSuite::TestSuite(int numberOfTestCases){
	initializeMembersAndAllocateMemory(numberOfTestCases);
	fillTestSuiteWithRandomTestCases();
	// This is now called directly by Organism.setFitness
	//calculateTestSuiteCoverage();
}

TestSuite::TestSuite(int numberOfTestCases, TestCase** testCases) {

	initializeMembersAndAllocateMemory(numberOfTestCases);
	this->testCases = testCases;
	// This is now called directly by Organism.setFitness
	//calculateTestSuiteCoverage();
}

void TestSuite::initializeMembersAndAllocateMemory(int numberOfTestCases) {
	assert(numberOfTestCases > 0);

	this->numberOfTestCases = numberOfTestCases;
	this->numberOfParameters = targetCFG->getNumberOfParameters();
	this->numberOfEdges = targetCFG->getNumberOfEdges();
	this->numberOfPredicates = targetCFG->getNumberOfPredicates();


	duplicateEdgesCovered = new int[numberOfEdges] { };
	duplicatePredicatesCovered = new int[numberOfPredicates] { };

	testCases = new TestCase*[numberOfTestCases] { };
}

void TestSuite::fillTestSuiteWithRandomTestCases() {
	assert(testCases != 0);
	for(int i = 0; i < numberOfTestCases; i++){
		testCases[i] = new TestCase { numberOfParameters, numberOfEdges, numberOfPredicates };
		targetCFG->setCoverageOfTestCase(testCases[i]);
	}
}

TestCase** TestSuite::getAllTestCases() const{
	return  testCases;
}


TestCase* TestSuite::getTestCase(int index){

	assert(index >= 0  &&  index < numberOfTestCases);

	return  testCases[index];
}

void TestSuite::setTestCase(int index, TestCase* testCase) {
	delete testCases[index];
	testCases[index] = testCase;
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

void TestSuite::printOnlyTestSuiteCoverage() {
	targetCFG->printTestSuiteCoverage(this);
}

// This code is ridiculously inefficient, maybe we can switch to bitsets instead,
//	then use |= on entire bitsets.
void TestSuite::calculateTestSuiteCoverage() {
	for (int i =0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfEdges; j++) {
			duplicateEdgesCovered[j] += testCases[i]->getEdgesCovered()[j];
		}
	}

	for (int i =0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfPredicates; j++) {
			duplicatePredicatesCovered[j] += testCases[i]->getPredicatesCovered()[j];
		}
	}

}

TestSuite& TestSuite::operator =(const TestSuite& other) {
	if(this != &other){
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

		duplicateEdgesCovered = new int[numberOfEdges] { };
		duplicatePredicatesCovered = new int[numberOfPredicates] { };
		testCases = new TestCase*[numberOfTestCases] { } ;

		using std::memcpy;

		memcpy(duplicateEdgesCovered, other.duplicateEdgesCovered, sizeof(numberOfEdges * sizeof(int)));
		memcpy(duplicatePredicatesCovered, other.duplicatePredicatesCovered, sizeof(numberOfEdges * sizeof(int)));

		for(int i = 0; i < numberOfTestCases; i++){
			testCases[i] = new TestCase { *other.testCases[i] };
		}
	}

	return *this;
}
