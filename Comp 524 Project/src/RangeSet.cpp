/*
 * RangeSet.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: Austin and Randall
 */

#include "RangeSet.h"

#include <cmath>
#include <iterator>
#include <limits>

#include "ControlFlowGraph.h"
#include "GlobalVariables.h"
#include "Random.h"
#include "TestCase.h"
#include "TestSuite.h"

using std::numeric_limits;
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>    // std::sort



RangeSet::RangeSet() {
	this->minNumberOfRanges = 5;
	this->totalUsefulness = 0;

	// Start as blank organism. Grow whenever rangeSet generates a test case that covers something new.
	int edgesPlusPred = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	finalTestSuite = new TestSuite(0, edgesPlusPred, new TestCase*[edgesPlusPred]);
}

RangeSet::~RangeSet() {

	if( finalTestSuite ) {
		delete finalTestSuite;
	}
}

// This is called from tryLocalOpt and after test case crossover and mutation,
//	the only place a test case can be generated outside of the rangeSet.
void RangeSet::offerToFinalTestSuite(TestCase* tc) {

	if (finalTestSuite->isCoveringNew(tc)) {
		TestCase* copy = new TestCase(*tc);
		finalTestSuite->addTestCase(copy);
		finalTestSuite->calculateTestSuiteCoverage();

		// Increment uses of a corresponding range in the range set.
		for (int i = 0; i < copy->getNumberOfParameters(); i++) {
			int param = copy->getInputParameters()[i];

			for (int j = 0; j < (int)rangesVector.size(); j++) {
				if (param >= rangesVector[j].start && param <= rangesVector[j].end) {
					rangesVector[j].incrementUses(param);
				}
			}
		}

	}
}

TestCase* RangeSet::getNewTestCase() {
		vector<Range> tmp = selectRangesForNewTestCaseProportionalToUsefulness();
		TestCase* retval = new TestCase(); // empty test case

		for(int i = 0; i < (int)tmp.size(); i++)	{
			retval->setInputParameter(i, uniformInRange(tmp[i].start, tmp[i].end));
		}
		targetCFG->setCoverageOfTestCase(retval);

		if (finalTestSuite->isCoveringNew(retval)) {
			finalTestSuite->addTestCase(new TestCase(*retval));
			finalTestSuite->calculateTestSuiteCoverage();

			for (int i = 0; i < targetCFG->getNumberOfParameters(); i++) {
				tmp[i].incrementUses(retval->getParameter(i));
				totalUsefulness++;
			}
			sortRangesByUsefulness();
		}

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
	return uniformInRange(0, rangesVector.size()-1);
}

vector<Range> RangeSet::selectRangesForNewTestCaseProportionalToUsefulness() {
	vector<Range> retval;
	int sum;

	for (int param = 0; param < targetCFG->getNumberOfParameters(); param++) {
		int i = 0;

		if (totalUsefulness == 0) {
			i = getRandomRange();
		} else {
			sum = rangesVector[0].numOfUses;
			long toss = uniformInRange(0, totalUsefulness-1);
			while (sum < toss) {
				i++;
				sum += rangesVector[i].numOfUses;
			}
		}

		retval.push_back(rangesVector[i]);
	}

	return retval;
}

void RangeSet::adaptRangesBasedOnUsefulness() {
	int numberOfRanges = rangesVector.size();
	double mean = totalUsefulness / numberOfRanges;
	double stdDev = 0;

	double tmp = 0.0;

	for (int i = 0; i< numberOfRanges; i++) {
		tmp = mean - rangesVector[i].numOfUses;
		stdDev += tmp * tmp;
	}
	stdDev /= numberOfRanges;
	stdDev = sqrt(stdDev);

	//cout << "\t\tAdpating Ranges: " << "Mean Usefulness: " << mean << " StdDev: " << stdDev << endl;
	//cout << "Old Range set: " << endl;
	//printRangesSimple();

	int index = rangesVector.size()-1;
	while (index >= 0 && numberOfRanges > minNumberOfRanges && (rangesVector[index].numOfUses == 0 || rangesVector[index].numOfUses < mean -  stdDev))
	{
		assert(index >= 0);
		assert(index < numberOfRanges);

		//cout << endl <<  "Ranges before delete bad range: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();

		deleteRange(index);

		//cout << endl <<  "Ranges after delete bad range: " << numberOfRanges << " totalUsefulness: " << totalUsefulness << endl;
		//printRangesSimple();

		index--;
	}

	index = 0;
	while (rangesVector[index].numOfUses > mean + stdDev)
	{
		assert(index >= 0);
		assert(index < numberOfRanges);

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
	//cout << "New Range set: " << endl;
	//printRangesSimple();
} // May split, delete, or combine ranges maybe

void RangeSet::addNewRandomRange() {
	int size = uniformInRange(100, 100000);

	int randomStart = uniformInRange(numeric_limits<int>::min(), numeric_limits<int>::max() - size);

	addRange(new Range(randomStart, randomStart + size));
}
void RangeSet::splitRange(int index) {
	Range* old = &rangesVector[index];
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
	assert(index < (int)rangesVector.size());
	if((int)rangesVector.size() >= minNumberOfRanges) {
		totalUsefulness -= rangesVector[index].numOfUses;
		rangesVector.erase (rangesVector.begin()+index);
	}
}

void RangeSet::addRangesAdjacentToExistingRange(int index) {
	Range* existing = &rangesVector[index];
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

	rangesVector.push_back(*r);

	totalUsefulness += r->numOfUses;
	// push_back calls copy constructor
	delete r;
	sortRangesByUsefulness();
}

void RangeSet::sortRangesByUsefulness() {
	std::sort(rangesVector.begin(), rangesVector.end());
}

void RangeSet::printRanges() {
	for (int i = 0; i < (int)rangesVector.size(); i++) {
		rangesVector[i].printRange();
	}
}

void RangeSet::printRangesSimple() {
	for (int i = 0; i < (int)rangesVector.size(); i++) {
		rangesVector[i].printRangeSimple();
	}
}
