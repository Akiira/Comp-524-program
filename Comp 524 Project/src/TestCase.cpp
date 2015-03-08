///////////////////////////////////////////////////////////
//  TestCase.cpp
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "TestCase.h"
#include "ControlFlowGraph.h"
#include "Random.h"

#include <string>
#include <cassert>
#include <cstring>

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

// Copy constructor
//TODO Test that I did memcpy right
TestCase::TestCase(const TestCase& that) {

	numberOfEdges = that.numberOfEdges;
	numberOfParameters = that.numberOfParameters;
	numberOfPredicates = that.numberOfPredicates;

	edgesCovered = new bool[numberOfEdges] { };
	/*
	for (int i = 0; i < numberOfEdges; i++) {
		edgesCovered[i] = that.edgesCovered[i];
	}
	*/
	memcpy(edgesCovered, that.edgesCovered, sizeof *edgesCovered * numberOfEdges);
	predicatesCovered = new bool[numberOfPredicates] { };
	/*
	for (int i = 0; i < numberOfPredicates; i++) {
		predicatesCovered[i] = that.predicatesCovered[i];
	}
	*/
	memcpy(predicatesCovered, that.predicatesCovered, sizeof *predicatesCovered * numberOfPredicates);
	inputParameters = new int[numberOfParameters] { };
	/*
	for (int i = 0; i < numberOfParameters; i++) {
		inputParameters[i] = that.inputParameters[i];
	}
	*/
	memcpy(inputParameters, that.inputParameters, sizeof *inputParameters * numberOfParameters);
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

int* TestCase::getInputParameters(){
	return  inputParameters;
}

int TestCase::getInputParameterAtIndex(int index) {
	assert(index >= 0 && index < numberOfParameters);
	return inputParameters[index];
}

void TestCase::addEdgeCoverage(int edge) {
	edgesCovered[edge] = true;
}

void TestCase::addPredicateCoverage(int predicate) {
	predicatesCovered[predicate] = true;
}

void TestCase::setInputParameters(int newValues[]) {
	for(int i = 0; i < numberOfParameters; i++)
	{
		inputParameters[i] = newValues[i];
	}
}

void TestCase::setInputParameterAtIndex(int index, int newValue) {
	assert(index >= 0 && index < numberOfParameters);
	inputParameters[index] = newValue;
}

TestCase& TestCase::operator =(const TestCase& org) {
	if(this != &org){
		delete[] inputParameters;
		delete[] edgesCovered;
		delete[] predicatesCovered;

		numberOfEdges = org.numberOfEdges;
		numberOfParameters = org.numberOfParameters;
		numberOfPredicates = org.numberOfPredicates;

		edgesCovered = new bool[numberOfEdges] { };
		inputParameters = new int[numberOfParameters] { };
		predicatesCovered = new bool[numberOfPredicates] { };

		using std::memcpy;
		memcpy(edgesCovered, org.edgesCovered, sizeof(org.numberOfEdges * sizeof(bool)));
		memcpy(inputParameters, org.inputParameters, sizeof(org.numberOfParameters * 4));
		memcpy(predicatesCovered, org.predicatesCovered, sizeof(org.numberOfPredicates * sizeof(bool)));
	}
	return *this;
}
