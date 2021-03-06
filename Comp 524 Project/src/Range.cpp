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
using std::cerr;

Range::~Range() {
	delete[] usesArray;
}

Range::Range(int start, int end) {
	// Should never happen but just in case.
	if (start > end) {
		int tmp = start;
		start = end;
		end = tmp;
	}
	else if (start == end) {
		end = start + 1;
	}

	this->start = start;
	this->end = end;
	this->numOfUses = 0;

	this->usesArraySize = ((end - start) / 25);
	if ((end - start) % 25 != 0 || usesArraySize == 0) {
		usesArraySize++;
	}

	this->usesArray = new int[usesArraySize]{};
}

// Used by RangeSet::splitRange
Range::Range(int start, int end, Range* source) {
	// Should never happen but just in case.
	if (start > end) {
		int tmp = start;
		start = end;
		end = tmp;
	}
	else if (start == end) {
		end = start + 1;
	}

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
		// Be extra careful since we've had so many problems with this code.
		if (j < usesArraySize) {
			if (i < source->usesArraySize) {
				usesArray[j] = source->usesArray[i];
				numOfUses += source->usesArray[i];
			} else {
				usesArray[j] = 0;
			}
		}

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
    //cout << "copy: ";
    //other.printRangeSimple();

	this->start = other.start;
	this->end = other.end;
	this->numOfUses = other.numOfUses;
	this->usesArraySize = other.usesArraySize;
	assert(other.usesArraySize > 0);

	usesArray = new int[other.usesArraySize];
	for (int i = 0; i < other.usesArraySize; i++) {
		usesArray[i] = other.usesArray[i];
	}
}


Range& Range::operator= (const Range& other) {
    //cout << "assign: ";
    //other.printRangeSimple();

	if (this != &other) {
		this->start = other.start;
		this->end = other.end;
		this->numOfUses = other.numOfUses;

		assert(other.usesArraySize > 0);


		int * newArray = new int[other.usesArraySize];
		for (int i = 0; i < other.usesArraySize; i++) {
			newArray[i] = other.usesArray[i];
		}
		delete [] usesArray;

		usesArray = newArray;
		usesArraySize = other.usesArraySize;
	}
	return *this;
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


void Range::printRangeSimple() const{
	cout << "[ " << start << ", " << end << " ] " << "Size " << (end-start) << " Uses: " << numOfUses << endl;
}

void Range::incrementUses(int valueUsed) {
	if (valueUsed >= start && valueUsed <= end) {
		// minus one so that when == end it falls into the last bucket.
		int bucket = (valueUsed - start) / 25;
		// happens when range size is perfectly divisible by 25. In cases where there is
		//	a remainder we are adding an additional bucket so this shouldn't be an issue.
		//	I don't think it makes sense to add an additional bucket just for the last element
		//	though, just put it in the last bucket (so last actually might have 26 elements.)
		if (bucket == usesArraySize) {
			bucket = usesArraySize - 1;
		}
		//cout << "value: " << valueUsed << " bucket: " << bucket << " usesArraySize: " << usesArraySize << " start: " << start << " end: " << end << endl;
		assert(bucket >= 0);
		assert(bucket < usesArraySize);

		usesArray[bucket]++;
		numOfUses++;
	}
	else {
		cerr << "Attempt to incrementUses with out of range parameter" << endl;
	}
}


