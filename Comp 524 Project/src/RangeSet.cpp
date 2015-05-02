/*
 * RangeSet.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: Austin and Randall
 */

#include "RangeSet.h"
#include "Range.h"
#include "Random.h"
#include "GlobalVariables.h"
#include "ControlFlowGraph.h"
#include "TestCase.h"
#include "Population.h"
#include "TestSuite.h"

#include <iostream>
#include <limits>

using std::numeric_limits;

RangeSet::~RangeSet() {
	for (int i = 0; i < numberOfRanges; i++) {
			delete ranges[i];
	}

	delete[] ranges;
	delete finalTestSuite;
	delete[] testCaseSources;
}

RangeSet::RangeSet(int numberOfRanges, int maxNumberOfRanges) {
	this->numberOfRanges = numberOfRanges;
	this->maxNumberOfRanges = maxNumberOfRanges;
	this->minNumberOfRanges = 5;
	this->ranges = new Range*[maxNumberOfRanges];
	this->totalUsefulness = 0;

	this->testCaseSources = new int[5] {};

	// Start as blank test suite. Grow whenever rangeSet generates a test case that covers something new.
	int edgesPlusPred = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	finalTestSuite = new TestSuite(0, edgesPlusPred, new TestCase*[edgesPlusPred]);
}

// This is called from tryLocalOpt and after test case crossover and mutation,
//	the only place a test case can be generated outside of the rangeSet.
void RangeSet::offerToFinalTestSuite(TestCase* tc, int source) {

	if (finalTestSuite->isCoveringNew(tc)) {
		finalTestSuite->addTestCase(new TestCase(*tc));
		finalTestSuite->calculateTestSuiteCoverage();

		testCaseSources[source]++;

		// Increment uses of a corresponding range(s) in the range set.
		for (int i = 0; i < tc->getNumberOfParameters(); i++) {
			int param = tc->getInputParameters()[i];

			for (int j = 0; j < numberOfRanges; j++) {
				if (param >= ranges[j]->start && param <= ranges[j]->end) {
					ranges[j]->incrementUses(param);
				}
			}
		}
		sortRangesByUsefulness();
	}
}

TestCase* RangeSet::getNewTestCase() {
		Range** tmp = selectRangesForNewTestCaseProportionalToUsefulness();
		TestCase* retval = new TestCase(); // empty test case

		for(int i = 0; i < targetCFG->getNumberOfParameters(); i++)	{
			retval->setInputParameter(i, uniformInRange(tmp[i]->start, tmp[i]->end));
		}
		targetCFG->setCoverageOfTestCase(retval);

		if (finalTestSuite->isCoveringNew(retval)) {
			finalTestSuite->addTestCase(new TestCase(*retval));
			finalTestSuite->calculateTestSuiteCoverage();

			// Test suite mutation
			testCaseSources[1]++;

			for (int i = 0; i < targetCFG->getNumberOfParameters(); i++) {
				tmp[i]->incrementUses(retval->getParameter(i));
				totalUsefulness++;
			}
			sortRangesByUsefulness();
		}

		delete[] tmp;

		return retval;
}

TestCase* RangeSet::getNewTestCaseEntirelyFromRange(Range* range) {
	TestCase* retval = new TestCase(); // empty test case

	for(int i = 0; i < targetCFG->getNumberOfParameters(); i++)	{
		retval->setInputParameter(i, uniformInRange(range->start, range->end));
	}
	targetCFG->setCoverageOfTestCase(retval);

	if (finalTestSuite->isCoveringNew(retval)) {
		finalTestSuite->addTestCase(new TestCase(*retval));
		finalTestSuite->calculateTestSuiteCoverage();

		// Initial population
		testCaseSources[0]++;

		for (int i = 0; i < targetCFG->getNumberOfParameters(); i++) {
			range->incrementUses(retval->getParameter(i));
			totalUsefulness++;
		}
		sortRangesByUsefulness();
	}

	return retval;
}

TestCase* RangeSet::getNewTestCaseEntirelyFromRange(int start, int end) {
	TestCase* retval = new TestCase(); // empty test case

	for(int i = 0; i < targetCFG->getNumberOfParameters(); i++)	{
		retval->setInputParameter(i, uniformInRange(start, end));
	}

	targetCFG->setCoverageOfTestCase(retval);

	return retval;
}

unsigned int RangeSet::getRandomRange() const {
	return uniformInRange(0, numberOfRanges-1);
}

Range** RangeSet::selectRangesForNewTestCaseProportionalToUsefulness() {
	Range** retval = new Range*[targetCFG->getNumberOfParameters()];
	int sum;

	for (int param = 0; param < targetCFG->getNumberOfParameters(); param++) {
		int i = 0;

		if (totalUsefulness == 0) {
			i = getRandomRange();
		} else {
			sum = ranges[0]->numOfUses;
			long toss = uniformInRange(0, totalUsefulness-1);
			while (sum < toss) {
				i++;
				sum += ranges[i]->numOfUses;
			}
		}

		retval[param] = ranges[i];
	}

	return retval;
}

void RangeSet::adaptRangesBasedOnUsefulness() {
	double mean = totalUsefulness / numberOfRanges;
	double stdDev = 0;

	double tmp = 0.0;
	for (int i = 0; i< numberOfRanges; i++) {
		tmp = mean - ranges[i]->numOfUses;
		stdDev += tmp * tmp;
	}
	stdDev /= numberOfRanges;
	stdDev = sqrt(stdDev);
	//cout << "\t\tAdpating Ranges: " << "Mean Usefulness: " << mean << " StdDev: " << stdDev << endl;
	//cout << "Old Range set: " << endl;
	//printRangesSimple();
	int index = numberOfRanges-1;
	while (index >= 0 && numberOfRanges > minNumberOfRanges && (ranges[index]->numOfUses == 0 || ranges[index]->numOfUses < mean -  stdDev))
	{
		deleteRange(index);
		index--;
	}

	index = 0;
	while (index < numberOfRanges && ranges[index]->numOfUses > mean + stdDev)
	{
		if (((ranges[index]->end - ranges[index]->start) / 2) > 25) {
			addRangesAdjacentToExistingRange(index);
			splitRange(index);
		}
		index++;
	}
	addNewRandomRange();

	sortRangesByUsefulness();
	//cout << "New Range set: " << endl;
	//printRangesSimple();
} // May split, delete, or combine ranges maybe

void RangeSet::addNewRandomRange() {
	int size = uniformInRange(100, 100000);

	int randomStart = uniformInRange(numeric_limits<int>::min(), numeric_limits<int>::max() - size);

	addRange(new Range(randomStart, randomStart + size));
}
void RangeSet::splitRange(int index) {
	Range* old = ranges[index];
	int oldSize = (old->end - old->start);
	if ((oldSize / 2) > 25) {
		Range* new1 = new Range(old->start, old->start + (oldSize / 2), old);
		Range* new2 = new Range( old->start + (oldSize / 2), old->end, old);

		deleteRange(index);
		addRange(new1);
		addRange(new2);
	}
	else {
		//cout << "Cant split anymore because range is same size as the buckets." << endl;
	}
}

void RangeSet::deleteRange(int index) {

	assert(index >= 0);
	assert(index < numberOfRanges);
	if(numberOfRanges >= minNumberOfRanges) {
		totalUsefulness -= ranges[index]->numOfUses;

		if( ranges[index] ) {
			delete ranges[index];
			ranges[index] = 0;
		}

		numberOfRanges--;

		for (int i = index; i < numberOfRanges; i++) {
			ranges[i] = ranges[i+1];
		}
	}
}

void RangeSet::addRangesAdjacentToExistingRange(int index) {
	Range* existing = ranges[index];
	int size = existing->end - existing->start;
	Range* new1 = new Range(existing->start - size, existing->start);
	Range* new2 = new Range(existing->end, existing->end + size);
	// Seed them with some usefulness so they aren't immedietly discarded and
	//	since we know this area does seem promising.
	new1->numOfUses = existing->numOfUses / 2;
	new2->numOfUses = existing->numOfUses / 2;
	addRange(new1);
	addRange(new2);
}

void RangeSet::addRange(Range* r) {
	if (numberOfRanges < maxNumberOfRanges) {
		this->ranges[numberOfRanges] = r;
	}
	else {
		maxNumberOfRanges *= 2;
		Range** tmp = new Range*[maxNumberOfRanges];
		for (int i = 0; i < numberOfRanges; i++) {
			tmp[i] = new Range(*ranges[i]);
		}
		for (int i = 0; i < numberOfRanges; i++) {
			if( ranges[i] ){
				delete ranges[i];
				ranges[i] = 0;
			}
		}
		delete[] ranges;
		ranges = tmp;
		ranges[numberOfRanges] = r;
	}
	totalUsefulness += ranges[numberOfRanges]->numOfUses;
	numberOfRanges++;
	moveRangeToSortedPosition(numberOfRanges - 1);
}

void RangeSet::sortRangesByUsefulness() {
	for (int i = numberOfRanges - 1; i > 1; i--) {
		for (int j = 0; j < i; j++) {
			assert(i < numberOfRanges);
			assert(j+1 < numberOfRanges);
			if (ranges[j]->numOfUses < ranges[j + 1]->numOfUses) {
				Range* tmp = ranges[j];
				ranges[j] = ranges[j + 1];
				ranges[j + 1] = tmp;
			}
		}
	}
}

void RangeSet::moveRangeToSortedPosition(int indexToSort) {
	int i = indexToSort;
	// Move the child left while its fitness is greater than it's left neighbor
	while ((i > 0) && (ranges[i]->numOfUses > ranges[i - 1]->numOfUses)) {
		assert(i < numberOfRanges);
		Range* tmp = ranges[i];
		ranges[i] = ranges[i - 1];
		ranges[i - 1] = tmp;
		i--;
	}
}

void RangeSet::printRanges() {
	for (int i = 0; i < numberOfRanges; i++) {
		ranges[i]->printRange();
	}
}

void RangeSet::printRangesSimple() {
	for (int i = 0; i < numberOfRanges; i++) {
		ranges[i]->printRangeSimple();
	}
}
