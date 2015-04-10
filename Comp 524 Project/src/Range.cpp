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
}

Range::Range(const Range& other) {
	this->start = other.start;
	this->end = other.end;
	this->numOfUses = other.numOfUses;
}

Range::~Range() {
	// TODO Auto-generated destructor stub
}

void Range::incrementUses() {
	numOfUses++;
}

