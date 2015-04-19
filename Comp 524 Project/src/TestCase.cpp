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
	edgesCovered = NULL;

	delete[] predicatesCovered;
	predicatesCovered = NULL;

	delete[] inputParameters;
	inputParameters = NULL;
}


TestCase::TestCase() {
	numberOfEdges      = targetCFG->getNumberOfEdges();
	numberOfParameters = targetCFG->getNumberOfParameters();
	numberOfPredicates = targetCFG->getNumberOfPredicates();

	edgesCovered      = new bool[numberOfEdges] { };
	predicatesCovered = new bool[numberOfPredicates] { };
	inputParameters   = new int[numberOfParameters] { };

	numCovered = 0;

	generateRandomParameters();

	targetCFG->setCoverageOfTestCase(this);
}

TestCase::TestCase(const TestCase& that) {

	numberOfEdges = that.numberOfEdges;
	numberOfParameters = that.numberOfParameters;
	numberOfPredicates = that.numberOfPredicates;
	numCovered = that.numCovered;

	edgesCovered = new bool[numberOfEdges] { };
	memcpy(edgesCovered, that.edgesCovered, sizeof(bool) * numberOfEdges);

	predicatesCovered = new bool[numberOfPredicates] { };
	memcpy(predicatesCovered, that.predicatesCovered, sizeof(bool) * numberOfPredicates);

	inputParameters = new int[numberOfParameters] { };
	memcpy(inputParameters, that.inputParameters, sizeof(int) * numberOfParameters);
}

void TestCase::generateRandomParameters() {

	for(int i = 0; i < numberOfParameters; i++)	{
		inputParameters[i] = uniformInRange(targetCFG->getLowerBoundForParameter(i),
											targetCFG->getUpperBoundForParameter(i));
	}
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

	targetCFG->setCoverageOfTestCase(this);
}

void TestCase::clearCoverage() {
	numCovered = 0;

	for(int i = 0; i < numberOfEdges; i++) {
		edgesCovered[i] = false;
	}

	for(int i = 0; i < numberOfPredicates; i++)	{
		predicatesCovered[i] = false;
	}
}

bool* TestCase::getEdgesCovered() const{
	return  edgesCovered;
}

bool* TestCase::getPredicatesCovered() const{
	return  predicatesCovered;
}

int* TestCase::getInputParameters() const{
	return  inputParameters;
}

int TestCase::getInputParameterAtIndex(int index) const{
	assert(index >= 0 && index < numberOfParameters);
	return inputParameters[index];
}

void TestCase::addEdgeCoverage(int edge) {
	if (!edgesCovered[edge]) {
		numCovered++;
		edgesCovered[edge] = true;
	}
}

void TestCase::addPredicateCoverage(int predicate) {
	if (!predicatesCovered[predicate]) {
		numCovered++;
		predicatesCovered[predicate] = true;
	}
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

void TestCase::printInputsAndCoverage() {
	targetCFG->printTestCaseCoverage(this);
}

void TestCase::printInputsOnly() {
	for (int i = 0; i < this->numberOfParameters; i++) {
		cout << "\t" << inputParameters[i];
	}
	cout << endl;
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
		memcpy(edgesCovered, org.edgesCovered, sizeof(bool) * org.numberOfEdges);
		memcpy(inputParameters, org.inputParameters, sizeof(int) * org.numberOfParameters);
		memcpy(predicatesCovered, org.predicatesCovered, sizeof(bool) * org.numberOfPredicates);
	}
	return *this;
}
