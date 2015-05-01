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
#include "Range.h"
#include "RangeSet.h"

#include <string>
#include <cassert>
#include <cstring>
#include <iostream>

TestCase::~TestCase() {
	delete[] edgesCovered;
	delete[] predicatesCovered;
	delete[] inputParameters;
}

// IMPORTANT: In almost all cases now you should use the functions in rangeSet to get test cases
TestCase::TestCase() {
	numberOfEdges      = targetCFG->getNumberOfEdges();
	numberOfParameters = targetCFG->getNumberOfParameters();
	numberOfPredicates = targetCFG->getNumberOfPredicates();

	edgesCovered = new bool[numberOfEdges] { };
	predicatesCovered = new bool[numberOfPredicates] { };
	inputParameters = new int[numberOfParameters] { };
	numCovered = 0;
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

// Static member function, Can be used by random searcher.
TestCase* TestCase::getRandomTestCase() {
	TestCase* retval = new TestCase();

	for(int i = 0; i < retval->numberOfParameters; i++)	{
		retval->inputParameters[i] = uniformInRange(targetCFG->getLowerBoundForParameter(i),
											targetCFG->getUpperBoundForParameter(i));
	}
	targetCFG->setCoverageOfTestCase(retval);
	return retval;
}

bool TestCase::hasSameCoverage(TestCase* that) const {
	int edges = targetCFG->getNumberOfEdges();
	int preds = targetCFG->getNumberOfPredicates();
	bool same = true;
	for (int i = 0; i < edges && same; i++) {
		same &= this->edgesCovered[i] == that->edgesCovered[i];
	}
	for (int i = 0; i < preds && same; i++) {
		same &= this->predicatesCovered[i] == that->predicatesCovered[i];
	}
	return same;
}

void TestCase::mutate() {
	for(int i = 0; i < numberOfParameters; i++){
		//int x = uniformInRange(1, 10);
		// These numbers are most often between -3 and 3, and usually less than 0.3, scale that up a bit.
		int x = normalDist(0, 1, 10) * 100;

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

int TestCase::getParameter(int index) const{
	assert( index >= 0 );
	assert( index < numberOfParameters );
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
	for(int i = 0; i < numberOfParameters; i++)	{
		inputParameters[i] = newValues[i];
	}
}

void TestCase::setInputParameter(int index, int newValue) {
	assert( index >= 0 );
	assert( index < numberOfParameters );
	inputParameters[index] = newValue;
}

void TestCase::printInputsAndCoverage() const {
	targetCFG->printTestCaseCoverage(this);
}

void TestCase::printInputsOnly() const {
	for (int i = 0; i < this->numberOfParameters; i++) {
		cout << "\t" << inputParameters[i];
	}
	cout << endl;
}

TestCase& TestCase::operator=(const TestCase& org) {
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
