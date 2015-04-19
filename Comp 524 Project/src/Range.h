/*
 * Range.h
 *
 *  Created on: Apr 9, 2015
 *      Author: amb6470
 */

#ifndef RANGE_H_
#define RANGE_H_

class Range {
public:
	Range(int start, int end);
	Range(int start, int end, Range* source); // used when splitting
	Range(const Range& other);
	virtual ~Range();
	int start;
	int end;
	// Idea is maybe focus on ranges that have proven especially useful
	int numOfUses;

	/* Each element will be 25 in size
	usesArraySize = (end - start / 25) + 1
	*/
	int usesArraySize;
	int** usesArray;



	void incrementUses(int valueUsed);

};

#endif /* RANGE_H_ */
