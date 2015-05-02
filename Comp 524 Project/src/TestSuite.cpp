///////////////////////////////////////////////////////////
//  TestSuite.cpp
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "GlobalVariables.h"
#include "Random.h"
#include "RangeSet.h"
#include "Range.h"

#include <iostream>
#include <cassert>
#include <cstring>

using std::cout;
using std::endl;

TestSuite::~TestSuite(){
	delete[] edgeCoverageCounts;
	delete[] predicateCoverageCounts;

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

	edgeCoverageCounts = new int[numberOfEdges] { };
	predicateCoverageCounts = new int[numberOfPredicates] { };
	coverageRatio = 0;
	testCases = new TestCase*[maxNumberOfTestCases] { };

	for (int i = 0; i < numberOfTestCases; ++i) {
		testCases[i] = new TestCase { *testSuite.testCases[i] };
	}
}

TestSuite::TestSuite(int numberOfTestCases, int maxNumberOfTestCases, TestCase** testCases, Range* range) {
	assert(numberOfTestCases >= 0 );
	assert(maxNumberOfTestCases >= numberOfTestCases);

	this->maxNumberOfTestCases = maxNumberOfTestCases;
	this->numberOfTestCases = numberOfTestCases;
	this->numberOfParameters = targetCFG->getNumberOfParameters();
	this->numberOfEdges = targetCFG->getNumberOfEdges();
	this->numberOfPredicates = targetCFG->getNumberOfPredicates();
	this->edgeCoverageCounts = new int[numberOfEdges] { };
	this->predicateCoverageCounts = new int[numberOfPredicates] { };

	if (testCases != NULL) {
		this->testCases = testCases;
	} else {
		this->testCases = new TestCase*[maxNumberOfTestCases] { };

		if (range != NULL) {
			// All test case parameters from this single range.
			for(int i = 0; i < numberOfTestCases; i++){
				this->testCases[i] = rangeSet->getNewTestCaseEntirelyFromRange(range);
				targetCFG->setCoverageOfTestCase(this->testCases[i]);
			}
		} else {
			for (int i = 0; i < numberOfTestCases; i++) {
				this->testCases[i] = rangeSet->getNewTestCase();
			}
		}
	}

	coverageRatio = 0;
}

void TestSuite::sortTestSuiteByCoverageCounts() {
	for (int i = numberOfTestCases - 1; i > 1; i--) {
		for (int j = 0; j < i; j++) {
			if (testCases[j]->getNumberCovered() < testCases[j + 1]->getNumberCovered() ) {
				TestCase *tmp = testCases[j];
				testCases[j] = testCases[j + 1];
				testCases[j + 1] = tmp;
			}
		}
	}
}

TestCase* TestSuite::getTestCaseThatCoversPredicate(int predicateNumber) const {
	if (!predicateCoverageCounts[predicateNumber] > 0) {
		return NULL;
	}
	else {
		for (int i = 0; i < numberOfTestCases; i++) {
			if (testCases[i]->getPredicatesCovered()[predicateNumber]) {
				return testCases[i];
			}
		}
	}

	assert(false);
	return NULL;
}

TestCase* TestSuite::getTestCaseThatCoversEdge(int edgeNumber) const {
	if (!edgeCoverageCounts[edgeNumber] > 0) {
		return NULL;
	}
	else {
		for (int i = 0; i < numberOfTestCases; i++) {
			if (testCases[i]->getEdgesCovered()[edgeNumber]) {
				return testCases[i];
			}
		}
	}

	return NULL;
}

void TestSuite::setTestCase(int index, TestCase* testCase) {
	delete testCases[index];
	testCases[index] = testCase;
}

void TestSuite::replaceDuplicateTestCase(TestCase* testCase) {
	auto tc = getDuplicateTestCase();

	if( tc ) {
		*tc = *testCase;
		// This handles the subtraction of old tc's coverage.
		calculateTestSuiteCoverage();
	}
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

void TestSuite::removeTestCase(int index) {
	assert(index >= 0 && index < numberOfTestCases);

	delete testCases[index];
	for (int i = index; i < numberOfTestCases-1; i++) {
		testCases[i] = testCases[i+1];
	}
	numberOfTestCases--;
}

TestCase* TestSuite::getDuplicateTestCase() const {

	for (int i = 0; i < numberOfTestCases; ++i) {
		if( canRemoveTestCaseWithoutChangingCoverage(i) ) {
			return getTestCase(i);
		}
	}
	assert(false);
	return NULL;
}

TestCase* TestSuite::getRandomTestCase() const {
	return testCases[uniformInRange(0, numberOfTestCases - 1)];
}

bool TestSuite::canRemoveTestCaseWithoutChangingCoverage(int index) const {
	assert(index >= 0 && index < numberOfTestCases);
	bool* edges = testCases[index]->getEdgesCovered();
	bool* preds = testCases[index]->getPredicatesCovered();

	for (int i = 0; i < targetCFG->getNumberOfEdges(); i++) {
		if (edges[i] && edgeCoverageCounts[i] == 1) {
			return false;
		}
	}

	for (int i = 0; i < targetCFG->getNumberOfPredicates(); i++) {
		if (preds[i] && predicateCoverageCounts[i] == 1) {
			return false;
		}
	}
	return true;
}


void TestSuite::printTestCaseInputsAndCoverage() const {
	cout << endl << "Test Cases:" << endl;
	for (int i = 0; i < numberOfTestCases; i++) {
		std::cout << endl << "Test Case #" << i << endl;
		testCases[i]->printInputsAndCoverage();
	}
}

void TestSuite::printTestCaseInputsOnly() const {
	cout << endl << "Test Cases:" << endl;
	for (int i = 0; i < numberOfTestCases; i++) {
		std::cout << "#" << i;
		testCases[i]->printInputsOnly();
	}
}

void TestSuite::printTestSuiteCoverage() const {
	cout << "Test Suite Coverage:" << endl;
	targetCFG->printTestSuiteCoverage(this);
}

void TestSuite::printTestSuiteCoverageRatio() const{
	cout << "Test Suite Coverage Ratio: " << coverageRatio << endl;
}

void TestSuite::printAll() const {
	printTestSuiteCoverage();
	printTestCaseInputsOnly();
	printTestSuiteCoverageRatio();
}

void TestSuite::resetCoverage() {
	for (int j = 0; j < numberOfEdges; j++) {
		edgeCoverageCounts[j] = 0;
	}

	for (int j = 0; j < numberOfPredicates; j++) {
		predicateCoverageCounts[j] = 0;
	}
	coverageRatio = 0;
}


void TestSuite::calculateTestSuiteCoverage() {
	resetCoverage();
	for (int i = 0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfEdges; j++) {
			edgeCoverageCounts[j] += testCases[i]->getEdgesCovered()[j];
		}
	}

	for (int i = 0; i < numberOfTestCases; i++) {
		for (int j = 0; j < numberOfPredicates; j++) {
			predicateCoverageCounts[j] += testCases[i]->getPredicatesCovered()[j];
		}
	}

	// Calculate the coverage ratio
	double numBranchCovered = 0, numPredCovered = 0;
	for (int j = 0; j < targetCFG->getNumberOfEdges(); ++j) {
		if (edgeCoverageCounts[j] > 0) {
			numBranchCovered++;
		}
	}
	for (int j = 0; j < targetCFG->getNumberOfPredicates(); ++j) {
		if (predicateCoverageCounts[j] > 0) {
			numPredCovered++;
		}
	}
	coverageRatio = (numBranchCovered+numPredCovered) / (numberOfEdges + numberOfPredicates);
	branchCoverageRatio = numBranchCovered / numberOfEdges;
	mccCoverageRatio = numPredCovered / numberOfPredicates;
}

bool TestSuite::isCoveringNew(const TestCase *tc) const {
	return (isCoveringNewEdge(tc->getEdgesCovered()) || isCoveringNewPred(tc->getPredicatesCovered()));
}

bool TestSuite::isCoveringNewEdge(const bool *coverage) const {
	for (int i = 0; i < numberOfEdges; ++i) {
		if(edgeCoverageCounts[i] == 0 && coverage[i] == true){
			return true;
		}
	}
	return false;
}

bool TestSuite::isCoveringNewPred(const bool *coverage) const {
	for (int i = 0; i < numberOfPredicates; ++i) {
		if(predicateCoverageCounts[i] == 0 && coverage[i] == true){
			return true;
		}
	}

	return false;
}

TestSuite& TestSuite::operator=(const TestSuite& other) {
	if(this != &other){
		delete[] edgeCoverageCounts;
		delete[] predicateCoverageCounts;

		for(int i = 0; i < numberOfTestCases; i++){
			delete testCases[i];
		}
		delete[] testCases;

		numberOfTestCases = other.numberOfTestCases;
		maxNumberOfTestCases = other.maxNumberOfTestCases;
		numberOfParameters = other.numberOfParameters;
		numberOfEdges = other.numberOfEdges;
		numberOfPredicates = other.numberOfPredicates;

		edgeCoverageCounts = new int[numberOfEdges] { };
		predicateCoverageCounts = new int[numberOfPredicates] { };
		testCases = new TestCase*[maxNumberOfTestCases] { } ;

		using std::memcpy;

		memcpy(edgeCoverageCounts, other.edgeCoverageCounts, sizeof(int) * numberOfEdges);
		memcpy(predicateCoverageCounts, other.predicateCoverageCounts, sizeof(int) * numberOfPredicates);

		for(int i = 0; i < numberOfTestCases; i++){
			testCases[i] = new TestCase { *other.testCases[i] };
		}
	}

	return *this;
}
