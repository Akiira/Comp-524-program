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
	fitness = 0;

	chromosome = new TestSuite { numOfTestCases };
	setFitness();
}

TestSuite* Organism::getChromosome() const{
	return  chromosome;
}

int Organism::getFitness() const{
	return fitness;
}


void Organism::mutate(double mutationProb){

}

int Organism::setFitness(){
	int sum = 0;

	int* edgeCoverage = chromosome->getDuplicateEdgesCovered();
	int* predicateCoverage = chromosome->getDuplicatePredicatesCovered();
	int baseReward = chromosome->getNumberOfTestCases() + 1;

	// An idea I had for a quick fitness function
	// Max fitness is numberOfTestCases*numberOfEdges + numberOfTestCases*numberOfPredicates
	// Min fitness is 0
	// They are punished for covering the same edges and predicates over and over
	//	The hope is that this brings organisms that managed to cover the hard to reach test cases to the top
	for(int i = 0; i < (targetCFG->getNumberOfEdges()); i++){
		if (edgeCoverage[i] > 0) {
			sum += baseReward - edgeCoverage[i];
		}
	}

	for(int i = 0; i < (targetCFG->getNumberOfPredicates()); i++){
		if (predicateCoverage[i] > 0) {
			sum += baseReward - predicateCoverage[i];
		}
	}

	fitness = sum;
	return fitness;
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
