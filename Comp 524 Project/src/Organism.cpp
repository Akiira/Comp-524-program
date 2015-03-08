///////////////////////////////////////////////////////////
//  Organism.cpp
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Organism.h"
#include "ControlFlowGraph.h"
#include "GlobalVariables.h"
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

Organism::~Organism(){
	delete chromosome;
}


Organism::Organism(int numOfTestCases, TestCase** testCases) {
	chromosome = new TestSuite { numOfTestCases, testCases };
	initialized = true;
	evaluated = false;
	fitness = 0;
}

Organism::Organism(int numOfTestCases ) {
	initialized = true;
	evaluated = false;
	fitness = 0;
	chromosome = 0;

	chromosome = new TestSuite { numOfTestCases };
}

TestSuite* Organism::getChromosome() const{
	return  chromosome;
}

int Organism::getFitness() const{
	return fitness;
}


void Organism::mutate(double mutationProb){

}

void Organism::setFitness(){
	int sum = 0;
	for(int i = 0; i < chromosome->getNumberOfTestCases(); i++){

	}

	fitness = sum;
}

void Organism::print() {
	chromosome->printSimple();
}

int Organism::getNumberOfTestCases() const {
	if(chromosome)
		return chromosome->getNumberOfTestCases();
	else
		return -1;
}


//===========================OVERLOADED OPERATORS===========================

bool Organism::operator<=(const Organism& right) { //overloaded operator <=
	return (fitness <= right.fitness) ? true : false;
} //operator<=

bool Organism::operator==(const Organism& right) { //overloaded operator ==
												   //TODO implement function
	return false;
	//return (strcmp(chromosome, right.chromosome) == 0)? true : false;
}	//operator==

Organism& Organism::operator=(const Organism& org) {	//assignment operator
	//TODO: implement this.
//	if (this != &org) {
//		delete[] chromosome;
//		chromosome = new char[org.length + 1];
//		assert(chromosome != NULL);
//		strcpy(chromosome, org.chromosome);
//		fitness = org.fitness;
//		length = org.length;
//	}	//if
	return *this;
}//operator=
