/*
 * Range.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: amb6470
 */

#include "Range.h"
Range::Range(int start, int end) {
	this->start = start;
	this->end = end;
	this->numOfUses = 0;
	this->usesArraySize = (end - start) / 25 + 1;
	this->usesArray = new int*[usesArraySize];
}

Range::Range(int start, int end, Range* source) {
	this->start = start;
	this->end = end;
	this->numOfUses = 0;
	this->usesArraySize = (end - start) / 25 + 1;
	this->usesArray = new int*[usesArraySize];
	// Todo not sure about this logic

	for (int j = 0, i = (source->end - start) / 25; j < usesArraySize; j++, i++) {
		usesArray[j] = source->usesArray[i];
		numOfUses += *(source->usesArray[i]);
	}
}

Range::Range(const Range& other) {
	this->start = other.start;
	this->end = other.end;
	this->numOfUses = other.numOfUses;
	this->usesArraySize = other.usesArraySize;
	this->usesArray = new int*[usesArraySize];
	for (int i = 0; i < usesArraySize; i++) {
		this->usesArray[i] = other.usesArray[i];
	}
}

Range::~Range() {
	delete[] usesArray;
}

void Range::incrementUses(int valueUsed) {
	int bucket = (valueUsed - start) / 25;
	usesArray[bucket]++;
	numOfUses++;
}


