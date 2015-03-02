///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include <cassert>

TestSuite::~TestSuite(){

}


TestSuite::TestSuite(int numberOfTestCases, int numberOfParameters, int numberOfEdges, int numberOfPredicates){
	this->numberOfTestCases = numberOfTestCases;
	edgesCovered = new bool[numberOfEdges];
	duplicates = new int[numberOfEdges];

	testCases = new TestCase*[numberOfTestCases];

	for(int i = 0; i < numberOfTestCases; i++){
		testCases[i] = new TestCase(numberOfEdges, numberOfParameters, numberOfPredicates);
	}
}


TestCase** TestSuite::getAllTestCases(){
	return  testCases;
}


TestCase* TestSuite::getTestCase(int index){

	assert(index >= 0  &&  index < numberOfTestCases);

	return  testCases[index];
}
