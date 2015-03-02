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

}

// Fill the new suite with random test cases and evaluate the coverage of all the test cases
TestSuite::TestSuite(int numberOfTestCases, ControlFlowGraph* cfg){
	cout << "here";
	this->numberOfTestCases = numberOfTestCases;
	this->numberOfParameters = cfg->getNumberOfParameters();
	this->numberOfEdges = cfg->getNumberOfEdges();
	this->numberOfPredicates = cfg->getNumberOfPredicates();


	edgesCovered = new bool[numberOfEdges];
	duplicateEdgesCovered = new int[numberOfEdges];

	predicatesCovered = new bool[numberOfPredicates];
	duplicatePredicatesCovered = new int[numberOfPredicates];

	testCases = new TestCase*[numberOfTestCases];

	for(int i = 0; i < numberOfTestCases; i++){
		// Generates random test cases and stores them
		testCases[i] = new TestCase(numberOfParameters, numberOfEdges, numberOfPredicates);
		cfg->setCoverageOfTestCase(testCases[i]);
	}
}


TestCase** TestSuite::getAllTestCases(){
	return  testCases;
}


TestCase* TestSuite::getTestCase(int index){

	assert(index >= 0  &&  index < numberOfTestCases);

	return  testCases[index];
}

void TestSuite::print() {
	std::cout << "here";
	for (int i = 0; i < numberOfTestCases; i++) {
		std::cout << "Test Case #" << i << std::endl;
		testCases[i]->print();
	}
}
