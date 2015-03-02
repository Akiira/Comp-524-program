///////////////////////////////////////////////////////////
//  TestCase.cpp
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestCase.h"
#include <iostream>
TestCase::~TestCase(){

	delete[] edgesCovered;
	delete[] predicatesCovered;
	delete[] inputParameters;
}

TestCase::TestCase(int numberOfParameters, int numberOfEdges, int numberOfPredicates){
	this->numberOfParameters = numberOfParameters;
	this->numberOfEdges = numberOfEdges;
	this->numberOfPredicates = numberOfPredicates;

	edgesCovered =  new bool[numberOfEdges];
	predicatesCovered = new bool[numberOfPredicates];
	inputParameters = new int[numberOfParameters];

	generateRandomParameters();
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

void TestCase::setInputParameterAtIndex(int index, int newValue) {
	inputParameters[index] = newValue;
}

void TestCase::generateRandomParameters() {
	//for each parameter generate a random value
	for(int i = 0; i < numberOfParameters; i++)
	{
		inputParameters[i] = uniformInRange(1, 10);
	}
}

void TestCase::clearCoverage() {
	for(int i = 0; i < numberOfEdges; i++)
	{
		edgesCovered[i] = false;
	}

	for(int i = 0; i < numberOfPredicates; i++)
	{
		predicatesCovered[i] = false;
	}
}

void TestCase::print() {
	std::cout << "Edge Coverage" << std::endl;
	for(int i = 0; i < numberOfEdges; i++)
	{
		std::cout << edgesCovered[i] << " | ";
	}
	std::cout << std::endl;

	std::cout << "Predicate Coverage" << std::endl;
	for(int i = 0; i < numberOfPredicates; i++)
	{
		std::cout << predicatesCovered[i] << " | ";
	}
}


/*
bool* TestCase::getEdgesCovered(){
	return  edgesCovered;
}

bool* TestCase::getpredicatesCovered(){
	return  predicatesCovered;
}
*/
