/*
 * RangeSet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: amb6470
 */

#ifndef RANGESET_H_
#define RANGESET_H_
class Range;
class RangeSet {
public:
	RangeSet();
	virtual ~RangeSet();
	int numberOfRanges, maxNumberOfRanges;
	Range** ranges;

	RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges);

	void addRange(Range* r);

	// Return an array of targetCFG.numberOfParameters ranges to use during test case gen
	Range** randomlySelectRangesForNewTestCase();

	void learnFromRanges(); // May split, delete, or combine ranges maybe
};

#endif /* RANGESET_H_ */
