///////////////////////////////////////////////////////////
//  TestCase.cpp
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestCase.h"
#include "ControlFlowGraph.h"
#include "Random.h"

TestCase::~TestCase(){

	delete[] edgesCovered;
	delete[] predicatesCovered;
	delete[] inputParameters;
}

TestCase::TestCase(int numberOfParameters, int numberOfEdges, int numberOfPredicates){
	this->numberOfParameters = numberOfParameters;
	this->numberOfEdges = numberOfEdges;
	this->numberOfPredicates = numberOfPredicates;

	edgesCovered = new bool[numberOfEdges] { };
	predicatesCovered = new bool[numberOfPredicates] { };
	inputParameters = new int[numberOfParameters] { };

	generateRandomParameters();
}

TestCase::TestCase(ControlFlowGraph& cfg) {
	numberOfEdges      = cfg.getNumberOfEdges();
	numberOfParameters = cfg.getNumberOfParameters();
	numberOfPredicates = cfg.getNumberOfPredicates();

	edgesCovered = new bool[numberOfEdges] { };
	predicatesCovered = new bool[numberOfPredicates] { };
	inputParameters = new int[numberOfParameters] { };

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

void TestCase::print(ControlFlowGraph* cfg) {
	cfg->printTestCaseCoverage(this);
}



bool* TestCase::getEdgesCovered(){
	return  edgesCovered;
}

bool* TestCase::getPredicatesCovered(){
	return  predicatesCovered;
}

void TestCase::addEdgeCoverage(int edge) {
	edgesCovered[edge] = true;
}

void TestCase::addPredicateCoverage(int predicate) {
	predicatesCovered[predicate] = true;
}
