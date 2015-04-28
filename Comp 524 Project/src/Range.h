/*
 * Range.h
 *
 *  Created on: Apr 9, 2015
 *      Author: Austin and Randall
 */

#ifndef RANGE_H_
#define RANGE_H_

class Range
{
public:

	int start, end, numOfUses, usesArraySize;
	int* usesArray;

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//
	virtual ~Range();

	Range(int start, int end);
	Range(int start, int end, Range* source); // used when splitting
	Range(const Range& other);

	//========================== CORE FUNCTIONS ====================================//

	void incrementUses(int valueUsed);

	//========================== PRINT FUNCTIONS ===================================//

	void printRange();
	void printRangeSimple() const;

	bool operator < (const Range& r) const
	{
		// SOrt in descending order
	    return (numOfUses > r.numOfUses);
	}

	Range& operator= (const Range& other);
};

#endif /* RANGE_H_ */
