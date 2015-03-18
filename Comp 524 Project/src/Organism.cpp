///////////////////////////////////////////////////////////
//  Organism.cpp
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "Organism.h"
#include "ControlFlowGraph.h"
#include "GlobalVariables.h"
#include <iostream>
#include <cassert>
#include "Random.h"
using std::cout;
using std::endl;

Organism::~Organism(){
	delete chromosome;
}

Organism::Organism(int numOfTestCases, int maxNumberOfTestCases, TestCase** testCases) {
	chromosome = new TestSuite { numOfTestCases, maxNumberOfTestCases, testCases };
	fitness = 0;
	scaledFitness = 0;
}


Organism::Organism(int numOfTestCases, int maxNumberOfTestCases ) {
	chromosome = new TestSuite { numOfTestCases, maxNumberOfTestCases};
	evaluateBaseFitness();
	scaledFitness = 0;
}

TestSuite* Organism::getChromosome() const{
	return  chromosome;
}

int Organism::getFitness() const{
	return fitness;
}


void Organism::mutate(double mutationProb) {
	int numberOfTestCases = chromosome->getNumberOfTestCases();

	for (int i = 0; i < numberOfTestCases; i++) {
		double toss = uniform01();
		if (toss < mutationProb) {
			TestCase* newTestCase = new TestCase();
			targetCFG->setCoverageOfTestCase(newTestCase);
			double toss2 = uniform01();
			if (toss2 < .75) {
				chromosome->setTestCase(i, newTestCase);
			}
			else {
				chromosome->addTestCase(newTestCase);
			}
		}
	}

	evaluateBaseFitness();
}

void Organism::evaluateBaseFitness(){
	//assert(initialized == true);
	fitness = 0;
	chromosome->calculateTestSuiteCoverage();
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
			fitness += baseReward - edgeCoverage[i];
		}
	}

	for(int i = 0; i < (targetCFG->getNumberOfPredicates()); i++){
		if (predicateCoverage[i] > 0) {
			fitness += baseReward - predicateCoverage[i];
		}
	}
}

void Organism::printSimple() {
	chromosome->printSimple();
}

void Organism::printFitnessAndCoverage() {
	cout << "Fitness: " << fitness << endl;
	chromosome->print();
}

int Organism::getNumberOfTestCases() const {
	if(chromosome)
		return chromosome->getNumberOfTestCases();
	else
		return -1;
}

int Organism::getMaxNumberOfTestCases() const {
	if(chromosome)
		return chromosome->getMaxNumberOfTestCases();
	else
		return -1;
}


//===========================OVERLOADED OPERATORS===========================

bool Organism::operator<=(const Organism& right) {
	return (fitness <= right.fitness) ? true : false;
}

bool Organism::operator<(const Organism& right) {
	return (fitness < right.fitness) ? true : false;
}

bool Organism::operator==(const Organism* right) {

	//They could be the same objects in memory or just have the same
	// test cases and coverage
	if( this->chromosome == right->chromosome ) {
		return true;
	}
	else if ( *this->chromosome == *right->chromosome ) {
		return true;
	} else {
		return false;
	}
}

Organism& Organism::operator=(const Organism& org) {
	if (this != &org) {
		*chromosome = *org.chromosome;

		assert(chromosome != NULL);
		fitness = org.fitness;
	}
	return *this;
}
