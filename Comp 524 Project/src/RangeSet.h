/*
 * RangeSet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: Austin and Randall
 */

#ifndef RANGESET_H_
#define RANGESET_H_

class Range;
class TestCase;
class TestSuite;

#include <cassert>
class RangeSet
{
public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//

	virtual ~RangeSet();

	RangeSet();
	RangeSet(int numberOfRanges, int maxNumberOfRanges);
	RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges);

	//========================== CORE FUNCTIONS ====================================//

	Range** selectRangesForNewTestCaseProportionalToUsefulness();
	void addRange(Range* r);
	void adaptRangesBasedOnUsefulness(); // May split, delete, or combine ranges maybe
	void offerToFinalTestSuite(TestCase* tc);

	//========================== GETTER FUNCTIONS ==================================//

	TestCase* getNewTestCase();
	TestCase* getNewTestCaseEntirelyFromRange(Range* range);
	TestCase* getNewTestCaseEntirelyFromRange(int start, int end);

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

	//========================== PRINT FUNCTIONS ===================================//

	void printRanges();
	void printRangesSimple();

private:
	int totalUsefulness, numberOfRanges, maxNumberOfRanges, minNumberOfRanges;
	Range** ranges;
	TestSuite* finalTestSuite;

	unsigned int getRandomRange() const;

	void addRangesAdjacentToExistingRange(int index);

	void splitRange(int index);
	void deleteRange(int index);

	void sortRangesByUsefulness();
	void moveRangeToSortedPosition(int indexToSort);
};

#endif /* RANGESET_H_ */
