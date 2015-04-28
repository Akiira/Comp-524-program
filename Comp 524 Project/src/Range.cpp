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

Range::Range(int start, int end) {
	this->start = start;
	this->end = end;
	this->numOfUses = 0;

	this->usesArraySize = ((end - start) / 25);
	if ((end - start) % 25 != 0 || usesArraySize == 0) {
		usesArraySize++;
	}

	this->usesArray = new int[usesArraySize]{};

    cout << "construct: ";
    printRangeSimple();
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
		assert(j < usesArraySize);
		assert(i < source->usesArraySize);
		usesArray[j] = source->usesArray[i];
		numOfUses += source->usesArray[i];
	}
	// Handle the boundary cases
	if (j < usesArraySize) {
		assert(j < usesArraySize);
		if ((j* 25 < end) && i < source->usesArraySize) {
			assert(i < source->usesArraySize);
			usesArray[j] = source->usesArray[i];
		}
		else {
			usesArray[j] = 0;
		}
	}

    cout << "construct: ";
    printRangeSimple();

}

Range::Range(const Range& other) {
    cout << "copy: ";
    other.printRangeSimple();

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
    cout << "assign: ";
    other.printRangeSimple();

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

Range::~Range() {
    cout << "destruct: ";
    printRangeSimple();

    // No need to check if its 0, delete on null pointer does nothing
	delete[] usesArray;
	usesArray = 0;
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


