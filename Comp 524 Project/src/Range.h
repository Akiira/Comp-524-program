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
	Range(const Range& other);
	virtual ~Range();
	int start;
	int end;
	// Idea is maybe focus on ranges that have proven especially useful
	int numOfUses;



	void incrementUses();

};

#endif /* RANGE_H_ */
