///////////////////////////////////////////////////////////
//  TestCase.cpp
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "TestCase.h"
#include "ControlFlowGraph.h"
#include "Random.h"
#include "GlobalVariables.h"

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

TestCase::TestCase() {
	numberOfEdges      = targetCFG->getNumberOfEdges();
	numberOfParameters = targetCFG->getNumberOfParameters();
	numberOfPredicates = targetCFG->getNumberOfPredicates();

	edgesCovered = new bool[numberOfEdges] { };
	predicatesCovered = new bool[numberOfPredicates] { };
	inputParameters = new int[numberOfParameters] { };

	generateRandomParameters();
}

TestCase::TestCase(const TestCase& that) {

	numberOfEdges = that.numberOfEdges;
	numberOfParameters = that.numberOfParameters;
	numberOfPredicates = that.numberOfPredicates;

	edgesCovered = new bool[numberOfEdges] { };
	memcpy(edgesCovered, that.edgesCovered, sizeof *edgesCovered * numberOfEdges);

	predicatesCovered = new bool[numberOfPredicates] { };
	memcpy(predicatesCovered, that.predicatesCovered, sizeof *predicatesCovered * numberOfPredicates);

	inputParameters = new int[numberOfParameters] { };
	memcpy(inputParameters, that.inputParameters, sizeof *inputParameters * numberOfParameters);
}


//This is another type of mutation we could use, perhaps in addition to other operators.
//The only thing that would need changing if we use this, is to change it from uniformInRange
// to a gaussian distribution; so it has a higher probability of picking closer numbers.
// Atleast, that how it is in the book.
void TestCase::mutate() {
	for(int i = 0; i < numberOfParameters; i++){
		int x = uniformInRange(1, 10);

		if(uniformInRange(0,1)){
			inputParameters[i] += x;
			if( inputParameters[i] > targetCFG->getUpperBoundForParameter(i) ){
				inputParameters[i] = targetCFG->getUpperBoundForParameter(i);
			}
		} else {
			inputParameters[i] -= x;
			if( inputParameters[i] < targetCFG->getLowerBoundForParameter(i) ){
				inputParameters[i] = targetCFG->getLowerBoundForParameter(i);
			}
		}
	}
}

void TestCase::generateRandomParameters() {
	//for each parameter generate a random value
	for(int i = 0; i < numberOfParameters; i++)
	{
		inputParameters[i] = uniformInRange(targetCFG->getLowerBoundForParameter(i),
											targetCFG->getUpperBoundForParameter(i));
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
