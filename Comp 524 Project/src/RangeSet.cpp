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
#include <iostream>
RangeSet::RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges) {
	this->numberOfRanges = numberOfRanges;
	this->maxNumberOfRanges = maxNumberOfRanges;
	this->ranges = ranges;
	this->totalUsefulness = 0;
	for (int i = 0; i < numberOfRanges; i++) {
		totalUsefulness += ranges[i]->numOfUses;	// Likely pointless because they'll be 0.
	}
}

RangeSet::~RangeSet() {
	delete[] ranges;
}

TestCase* RangeSet::getNewTestCase() {
	//Range** tmp = selectRangesForNewTestCaseProportionalToUsefulness();
	Range** tmp = randomlySelectRangesForNewTestCase();
	int numberOfParameters = targetCFG->getNumberOfParameters();
	TestCase* retval = new TestCase(true); // empty test case
	int* inputParameters = new int[numberOfParameters];
	for(int i = 0; i < numberOfParameters; i++)
	{
		inputParameters[i] = uniformInRange(ranges[i]->start, ranges[i]->end);
	}
	retval->setInputParametersWithReference(&inputParameters);
	targetCFG->setCoverageOfTestCase(retval);
	if (globalPopulation->coversNewEdgesOrPredicates(retval->getEdgesCovered(), true) ||
			globalPopulation->coversNewEdgesOrPredicates(retval->getPredicatesCovered(), false) ) {
		for (int i = 0; i < numberOfParameters; i++) {
			tmp[i]->numOfUses++;
			totalUsefulness++;
		}
		sortRangesByUsefulness();
	}
	return retval;
}

Range** RangeSet::randomlySelectRangesForNewTestCase() {
	int numberOfParameters = targetCFG->getNumberOfParameters();
	Range** retval = new Range*[numberOfParameters];
	for (int i = 0; i < numberOfParameters; i++) {
		retval[i] = ranges[uniformInRange(0, numberOfRanges-1)];
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

	int index = 0;
	while (ranges[index]->numOfUses > 0.10 * globalPopulation->getTestSuiteSize())
	{
		cout << "Splitting a really good range" << endl;
		splitRange(index);
		index++;
	}
	index = numberOfRanges-1;
	while (ranges[index]->numOfUses < 0.01 * globalPopulation->getTestSuiteSize())
	{
		cout << "Deleting a bad range" << endl;
		deleteRange(index);
		index--;
	}
	sortRangesByUsefulness();

} // May split, delete, or combine ranges maybe

void RangeSet::splitRange(int index) {
	Range* old = ranges[index];
	Range* new1 = (new Range(old->start, old->end / 2));
	Range* new2 = (new Range(old->end / 2 + 1, old->end));
	new1->numOfUses = old->numOfUses / 2;
	new2->numOfUses = old->numOfUses / 2;
	deleteRange(index);
	addRange(new1);
	addRange(new2);
}

void RangeSet::deleteRange(int index) {
	totalUsefulness -= ranges[index]->numOfUses;
	delete ranges[index];
	for (int i = index; i < numberOfRanges-1; i++) {
		ranges[i] = ranges[i+1];
	}
	cout << "after delete" << endl;
	numberOfRanges--;
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


