///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include "GlobalVariables.h"
#include "Random.h"
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
	maxNumberOfTestCases = testSuite.maxNumberOfTestCases;
	numberOfTestCases = testSuite.numberOfTestCases;
	numberOfParameters = testSuite.numberOfParameters;
	numberOfEdges = testSuite.numberOfEdges;
	numberOfPredicates = testSuite.numberOfPredicates;

	duplicateEdgesCovered = new int[numberOfEdges] { };
	duplicatePredicatesCovered = new int[numberOfPredicates] { };
	testCases = new TestCase*[maxNumberOfTestCases] { };
}

// Fill the new suite with random test cases and evaluate the coverage of all the test cases
// Note still need to call calculateTestSuiteCoverage which will be done by Organism.setFitness
TestSuite::TestSuite(int numberOfTestCases, int maxNumberOfTestCases){
	initializeMembersAndAllocateMemory(numberOfTestCases, maxNumberOfTestCases);
	this->testCases = new TestCase*[maxNumberOfTestCases] { };
	fillTestSuiteWithRandomTestCases();
}

TestSuite::TestSuite(int numberOfTestCases, int maxNumberOfTestCases, TestCase** testCases) {
	initializeMembersAndAllocateMemory(numberOfTestCases, maxNumberOfTestCases);
	this->testCases = testCases;
}

void TestSuite::initializeMembersAndAllocateMemory(int numberOfTestCases, int maxNumberOfTestCases) {
	assert(numberOfTestCases > 0 );
	assert(maxNumberOfTestCases >= numberOfTestCases);
	this->maxNumberOfTestCases = maxNumberOfTestCases;
	this->numberOfTestCases = numberOfTestCases;
	this->numberOfParameters = targetCFG->getNumberOfParameters();
	this->numberOfEdges = targetCFG->getNumberOfEdges();
	this->numberOfPredicates = targetCFG->getNumberOfPredicates();
	this->duplicateEdgesCovered = new int[numberOfEdges] { };
	this->duplicatePredicatesCovered = new int[numberOfPredicates] { };
}

void TestSuite::fillTestSuiteWithRandomTestCases() {
	assert(testCases != 0);
	for(int i = 0; i < numberOfTestCases; i++){
		testCases[i] = new TestCase { };
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

// Just deletes it if there isn't room
// Im thinking about only adding if it covers something new
void TestSuite::addTestCase(TestCase* testCase) {
	if (numberOfTestCases < maxNumberOfTestCases) {
		testCases[numberOfTestCases] = testCase;
		numberOfTestCases++;
	}
	else {
		delete testCase;
	}
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

void TestSuite::resetCoverage() {
	for (int j = 0; j < numberOfEdges; j++) {
		duplicateEdgesCovered[j] = 0;
	}

	for (int j = 0; j < numberOfPredicates; j++) {
		duplicatePredicatesCovered[j] = 0;
	}
}

// This code is ridiculously inefficient, maybe we can switch to bitsets instead,
//	then use |= on entire bitsets.
void TestSuite::calculateTestSuiteCoverage() {
	for (int i = 0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfEdges; j++) {
			duplicateEdgesCovered[j] += testCases[i]->getEdgesCovered()[j];
		}
	}

	for (int i = 0; i < numberOfTestCases; i++) {
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
		maxNumberOfTestCases = other.maxNumberOfTestCases;
		numberOfParameters = other.numberOfParameters;
		numberOfEdges = other.numberOfEdges;
		numberOfPredicates = other.numberOfPredicates;

		duplicateEdgesCovered = new int[numberOfEdges] { };
		duplicatePredicatesCovered = new int[numberOfPredicates] { };
		testCases = new TestCase*[maxNumberOfTestCases] { } ;

		using std::memcpy;

		memcpy(duplicateEdgesCovered, other.duplicateEdgesCovered, sizeof(int) * numberOfEdges);
		memcpy(duplicatePredicatesCovered, other.duplicatePredicatesCovered, sizeof(int) * numberOfPredicates);

		for(int i = 0; i < numberOfTestCases; i++){
			testCases[i] = new TestCase { *other.testCases[i] };
		}
	}

	return *this;
}
