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
#include <vector>
#include "Range.h"
class RangeSet
{
public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//

	virtual ~RangeSet();

	RangeSet();

	//========================== CORE FUNCTIONS ====================================//

	std::vector<Range> selectRangesForNewTestCaseProportionalToUsefulness();
	void addRange(Range* r);
	void addNewRandomRange();
	void adaptRangesBasedOnUsefulness(); // May split, delete, or combine ranges maybe
	void offerToFinalTestSuite(TestCase* tc);

	//========================== GETTER FUNCTIONS ==================================//

	TestCase* getNewTestCase();
	TestCase* getNewTestCaseEntirelyFromRange(Range* range);
	TestCase* getNewTestCaseEntirelyFromRange(int start, int end);

	int getNumberOfRanges() const {
		return rangesVector.size();
	}

	Range* getRange(int index) {
		int size = rangesVector.size();
		assert(index >= 0 && index < size);
		return &rangesVector[index];
	}

	TestSuite* getFinalTestSuite() {
		return finalTestSuite;
	}

	//========================== PRINT FUNCTIONS ===================================//

	void printRanges();
	void printRangesSimple();


private:
	int totalUsefulness = 0, minNumberOfRanges = 5; /*, numberOfRanges, maxNumberOfRanges, */

	//Range** ranges;

	std::vector<Range> rangesVector;

	TestSuite* finalTestSuite;

	unsigned int getRandomRange() const;

	void addRangesAdjacentToExistingRange(int index);

	void splitRange(int index);
	void deleteRange(int index);

	void sortRangesByUsefulness();
	void moveRangeToSortedPosition(int indexToSort);
};

#endif /* RANGESET_H_ */
