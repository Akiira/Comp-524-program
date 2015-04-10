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
RangeSet::RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges) {
	this->numberOfRanges = numberOfRanges;
	this->maxNumberOfRanges = maxNumberOfRanges;
	this->ranges = ranges;
}

RangeSet::~RangeSet() {
	delete[] ranges;
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
		numberOfRanges++;
	}
}

Range** RangeSet::randomlySelectRangesForNewTestCase() {
	int numberOfParameters = targetCFG->getNumberOfParameters();
	Range** retval = new Range*[numberOfParameters];
	for (int i = 0; i < numberOfParameters; i++) {
		retval[i] = ranges[uniformInRange(0, numberOfRanges-1)];
	}
	return retval;
}

void learnFromRanges() {} // May split, delete, or combine ranges maybe

void sortRangesByUsefulness(){} // Sort of Range.uses attribute.



