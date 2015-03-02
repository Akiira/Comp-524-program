///////////////////////////////////////////////////////////
//  TestCase.cpp
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestCase.h"

/*
TestCase::TestCase(){

}
*/



TestCase::~TestCase(){

}

TestCase::TestCase(int numberOfEdges, int numberOfPredicates, int numberOfParameters){
	this->numberOfParameters = numberOfParameters;
	branchEdgesCovered = new bool[numberOfEdges];
	multiConditionCovered = new bool[numberOfPredicates];
	inputParameters = new int[numberOfParameters];
}


bool* TestCase::getEdgesCovered(){
	return  branchEdgesCovered;
}

bool* TestCase::getMultiConditionCovered(){
	return  multiConditionCovered;
}


int* TestCase::getInputParameters(){

	return  inputParameters;
}

void TestCase::setInputParameters(int newValues[]) {
	for(int i = 0; i < numberOfParameters; i++)
	{
		inputParameters[i] = newValues[i];
	}
}

void TestCase::generateRandomParameters() {
	//for each parameter generate a random value
}
