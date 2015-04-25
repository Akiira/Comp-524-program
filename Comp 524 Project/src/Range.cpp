/*
 * Range.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: amb6470
 */

#include "Range.h"
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

Range::Range(int start, int end) {
	this->start = start;
	this->end = end;
	this->numOfUses = 0;
	this->usesArraySize = (end - start) / 25;
	if ((end - start) % 25 != 0 || usesArraySize == 0) {
		usesArraySize++;
	}

	this->usesArray = new int[usesArraySize]{};
}

// Used by RangeSet::splitRange
Range::Range(int start, int end, Range* source) {
	this->start = start;
	this->end = end;
	this->numOfUses = 0;

	this->usesArraySize = (end - start) / 25;
	if ((end - start) % 25 != 0 || usesArraySize == 0) {
		usesArraySize++;
	}
	this->usesArray = new int[usesArraySize] {};

		int i = (start - source->start) / 25;	// The bucket that start falls into
		int j = 0; // index for the new Range's usesArray.
		for (; j < source->usesArraySize / 2; j++, i++) {
			usesArray[j] = source->usesArray[i];
			numOfUses += source->usesArray[i];
		}
		// Handle the boundary cases
		if (j < usesArraySize) {
			if ((j* 25 < end) && i < source->usesArraySize) {
				usesArray[j] = source->usesArray[i];
			}
			else {
				usesArray[j] = 0;
			}
		}

}

Range::Range(const Range& other) {
	this->start = other.start;
	this->end = other.end;
	this->numOfUses = other.numOfUses;
	this->usesArraySize = other.usesArraySize;
	assert(usesArraySize > 0);
	this->usesArray = new int[usesArraySize] {};
	for (int i = 0; i < usesArraySize; i++) {
		this->usesArray[i] = other.usesArray[i];
	}
}

Range::~Range() {
	delete[] usesArray;
}

void Range::printRange() {
	cout << "[ " << start << ", " << end << " ] " << "Size " << (end-start) << endl;
	cout << "Uses:" << numOfUses <<  endl;
	cout << "Usage buckets: " << endl;
	for (int i = 0; i < usesArraySize; i++) {
		 cout << usesArray[i] << ", ";
	}
	cout << endl << endl;
}


void Range::printRangeSimple() {
	cout << "[ " << start << ", " << end << " ] " << "Size " << (end-start) << " Uses: " << numOfUses << endl;
}

void Range::incrementUses(int valueUsed) {
	int bucket = (valueUsed - start) / 25;
	assert(bucket >= 0 || bucket < usesArraySize);

	usesArray[bucket]++;
	numOfUses++;
	assert(numOfUses > 0);
}


