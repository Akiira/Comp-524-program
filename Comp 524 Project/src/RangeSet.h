/*
 * RangeSet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: amb6470
 */

#ifndef RANGESET_H_
#define RANGESET_H_
class Range;
class TestCase;
class TestSuite;

#include <cassert>
class RangeSet {
public:
	RangeSet();
	virtual ~RangeSet();

	RangeSet(int numberOfRanges, int maxNumberOfRanges);
	RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges);

	// Return an array of targetCFG.numberOfParameters ranges to use during test case gen

	// Depreciated should use getNewTestCase instead. TestCase constructor should be changed.
	//	But will leave for now because I'm tired.
	Range** randomlySelectRangesForNewTestCase();
	Range** selectRangesForNewTestCaseProportionalToUsefulness();

	TestCase* getNewTestCase();
	TestCase* getNewTestCaseEntirelyFromRange(Range* range);
	TestCase* getNewTestCaseEntirelyFromRange(int start, int end);

	void adaptRangesBasedOnUsefulness(); // May split, delete, or combine ranges maybe

	int getNumberOfRanges() const {
		return numberOfRanges;
	}

	Range* getRange(int index) {
		assert(index >= 0 && index < numberOfRanges);
		return ranges[index];
	}

	TestSuite* getFinalTestSuite() {
		return finalTestSuite;
	}

	void offerToFinalTestSuite(TestCase* tc);

	void printRanges();
	void printRangesSimple();
	void addRange(Range* r);
	void addNewRandomRange();

private:
	int totalUsefulness;
	int numberOfRanges, maxNumberOfRanges;
	int minNumberOfRanges;
	Range** ranges;

	TestSuite* finalTestSuite;

	void splitRange(int index);
	void addRangesAdjacentToExistingRange(int index);
	void deleteRange(int index);
	void sortRangesByUsefulness();
	void moveRangeToSortedPosition(int indexToSort);
};

#endif /* RANGESET_H_ */
