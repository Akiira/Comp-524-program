/*
 * RangeSet.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: amb6470
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

RangeSet::RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges) {
	this->numberOfRanges = numberOfRanges;
	this->maxNumberOfRanges = maxNumberOfRanges;
	this->minNumberOfRanges = 5;
	this->ranges = ranges;
	this->totalUsefulness = 0;
	for (int i = 0; i < numberOfRanges; i++) {
		totalUsefulness += ranges[i]->numOfUses;	// Likely pointless because they'll be 0.
	}

	// Start as blank organism. Grow whenever rangeSet generates a test case that covers something new.
	int edgesPlusPred = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	finalTestSuite = new TestSuite(0, edgesPlusPred, new TestCase*[edgesPlusPred]);
	//printRanges();
}

RangeSet::RangeSet(int numberOfRanges, int maxNumberOfRanges) {
	this->numberOfRanges = numberOfRanges;
	this->maxNumberOfRanges = maxNumberOfRanges;
	this->minNumberOfRanges = 5;
	this->ranges = new Range*[maxNumberOfRanges];
	this->totalUsefulness = 0;

	// Start as blank organism. Grow whenever rangeSet generates a test case that covers something new.
	int edgesPlusPred = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	finalTestSuite = new TestSuite(0, edgesPlusPred, new TestCase*[edgesPlusPred]);
}

RangeSet::~RangeSet() {

	for (int i = 0; i < numberOfRanges; i++) {
		delete ranges[i];
	}
	delete[] ranges;
	delete finalTestSuite;
}

// This is called from tryLocalOpt, the only place a test case can be generated
//	outside of the rangeSet.
void RangeSet::offerToFinalTestSuite(TestCase* tc) {
	if (finalTestSuite->isCoveringNew(tc)) {
		finalTestSuite->addTestCase(new TestCase(*tc));
		finalTestSuite->calculateTestSuiteCoverage();
	}
}

TestCase* RangeSet::getNewTestCase() {
	int toss = uniformInRange(0, 100);
	if (toss < 25) {
		return getNewTestCaseEntirelyFromRange(ranges[uniformInRange(0, numberOfRanges-1)]);
	}
	else {
		Range** tmp = selectRangesForNewTestCaseProportionalToUsefulness();
		TestCase* retval = new TestCase(); // empty test case

		for(int i = 0; i < targetCFG->getNumberOfParameters(); i++)	{
			retval->setInputParameter(i, uniformInRange(tmp[i]->start, tmp[i]->end));
		}
		targetCFG->setCoverageOfTestCase(retval);

		if (finalTestSuite->isCoveringNew(retval)) {
			finalTestSuite->addTestCase(new TestCase(*retval));
			finalTestSuite->calculateTestSuiteCoverage();

			for (int i = 0; i < targetCFG->getNumberOfParameters(); i++) {
				tmp[i]->incrementUses(retval->getParameter(i));
				totalUsefulness++;
			}
			sortRangesByUsefulness();
		}

		delete[] tmp;

		return retval;
	}
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

Range** RangeSet::randomlySelectRangesForNewTestCase() {
	int numberOfParameters = targetCFG->getNumberOfParameters();
	Range** retval = new Range*[numberOfParameters];
	for (int i = 0; i < numberOfParameters; i++) {
		int x = uniformInRange(0, numberOfRanges-1);
		retval[i] = ranges[x];
	}
	return retval;
}

Range** RangeSet::selectRangesForNewTestCaseProportionalToUsefulness() {
	//If totalFitness is zero then an organism is selected at random.
	long toss;
	int i;
	int sum;
	int numberOfParameters = targetCFG->getNumberOfParameters();
	Range** retval = new Range*[numberOfParameters];
	for (int param = 0; param < numberOfParameters; param++) {
		i = 0;
		if (totalUsefulness == 0) {
			i = uniformInRange(0, numberOfRanges - 1);
		} else {
			sum = ranges[0]->numOfUses;
			toss = uniformInRange(0, totalUsefulness-1);
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
	cout << "Mean Usefulness: " << mean << " StdDev: " << stdDev << endl;
	cout << "Old Range set: " << endl;
	printRangesSimple();
	int index = numberOfRanges-1;
	while (index >= 0 && numberOfRanges > minNumberOfRanges && ranges[index]->numOfUses < mean - (2 * stdDev))
	{
		//cout << endl <<  "Ranges before delete bad range: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();
		deleteRange(index);
		//cout << endl <<  "Ranges after delete bad range: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();
		index--;
	}

	index = 0;
	while (ranges[index]->numOfUses > mean + (0.5 *stdDev))
	{
		//cout << endl <<  "Splitting a really good range and exploring adjacents" << endl;
		//cout << endl <<  "Ranges before expore adjacent: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();
		addRangesAdjacentToExistingRange(index);


		//cout << endl << "Ranges after explore adjacent and before split range: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();
		splitRange(index);
		//cout << "Ranges after split range: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();
		index++;
	}

	addNewRandomRange();

	sortRangesByUsefulness();
	cout << "New Range set: " << endl;
	printRangesSimple();
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
		cout << "Cant split anymore because range is same size as the buckets." << endl;
	}
}

void RangeSet::deleteRange(int index) {
	assert(numberOfRanges > minNumberOfRanges);
	totalUsefulness -= ranges[index]->numOfUses;

	delete ranges[index];
	numberOfRanges--;

	for (int i = index; i < numberOfRanges; i++) {
		ranges[i] = ranges[i+1];
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
			delete ranges[i];
		}
		delete[] ranges;
		ranges = tmp;
		ranges[numberOfRanges] = r;
	}
	totalUsefulness += ranges[numberOfRanges]->numOfUses;
	moveRangeToSortedPosition(numberOfRanges);
	numberOfRanges++;
}

void RangeSet::sortRangesByUsefulness() {
	int i, j;
	Range* tmp;
	for (i = numberOfRanges - 1; i > 1; i--) {
		for (j = 0; j < i; j++) {
			if (ranges[j]->numOfUses < ranges[j + 1]->numOfUses) {
				tmp = ranges[j];
				ranges[j] = ranges[j + 1];
				ranges[j + 1] = tmp;
			}
		}
	}
}

void RangeSet::moveRangeToSortedPosition(int indexToSort) {
	Range* tmp;
	int i = indexToSort;
	// Move the child left while its fitness is greater than it's left neighbor
	while ((i > 0) && (ranges[i]->numOfUses > ranges[i - 1]->numOfUses)) {
		tmp = ranges[i];
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
