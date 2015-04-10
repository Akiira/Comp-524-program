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
class RangeSet {
public:
	RangeSet();
	virtual ~RangeSet();


	RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges);

	// Return an array of targetCFG.numberOfParameters ranges to use during test case gen

	// Depreciated should use getNewTestCase instead. TestCase constructor should be changed.
	//	But will leave for now because I'm tired.
	Range** randomlySelectRangesForNewTestCase();
	Range** selectRangesForNewTestCaseProportionalToUsefulness();

	TestCase* getNewTestCase();

	void adaptRangesBasedOnUsefulness(); // May split, delete, or combine ranges maybe

private:
	int totalUsefulness;
	int numberOfRanges, maxNumberOfRanges;
	Range** ranges;
	void addRange(Range* r);
	void splitRange(int index);
	void deleteRange(int index);
	void sortRangesByUsefulness();
	void moveRangeToSortedPosition(int indexToSort);
};

#endif /* RANGESET_H_ */
