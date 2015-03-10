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


Organism::Organism(int numOfTestCases, TestCase** testCases) {
	chromosome = new TestSuite { numOfTestCases, testCases };
	initialized = true;
	evaluated = false;
	fitness = 0;
}

/**
 * Removed SetFitness()
 * The idea is to keep things consistent between the Organism constructors. Above
 * we can't set the fitness yet because it is used in crossover but the fitness
 * shouldn't be calculated until after mutation. Here we could set the fitness
 * but it seems better to maintain consistency and require that when you create
 * an organism you must call setFitness yourself at an appropriate time. This
 * constructor is used by the population constructor which calls setPopulationFitness
 * to evaluate the fitness of all individuals and sort them.
 */
Organism::Organism(int numOfTestCases ) {
	chromosome = new TestSuite { numOfTestCases, 10 };
	initialized = true;
	evaluated = false;
	fitness = 0;
}

TestSuite* Organism::getChromosome() const{
	assert(initialized == true);
	return  chromosome;
}

int Organism::getFitness() const{
	assert(evaluated == true);
	return fitness;
}


void Organism::mutate(double mutationProb) {
	assert(initialized == true);
	int numberOfTestCases = chromosome->getNumberOfTestCases();
	for (int i = 0; i < numberOfTestCases; i++) {
		double toss = uniform01();
		if (toss < mutationProb) {
			TestCase* newTestCase = new TestCase();
			targetCFG->setCoverageOfTestCase(newTestCase);
			chromosome->setTestCase(i, newTestCase);
		}
	}

	evaluated = false;
}

int Organism::setFitness(){
	assert(initialized == true);
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

	evaluated = true;
	return fitness;
}

void Organism::printSimple() {
	chromosome->printSimple();
}

void Organism::printFitnessAndCoverage() {
	cout << "Fitness: " << fitness << endl;
	chromosome->printOnlyTestSuiteCoverage();
}

int Organism::getNumberOfTestCases() const {
	if(chromosome)
		return chromosome->getNumberOfTestCases();
	else
		return -1;
}


//===========================OVERLOADED OPERATORS===========================

bool Organism::operator<=(const Organism& right) { //overloaded operator <=
	assert(evaluated == true);
	return (fitness <= right.fitness) ? true : false;
} //operator<=

bool Organism::operator==(const Organism& right) { //overloaded operator ==
	assert(evaluated == true);										   //TODO implement function
	return false;
	//return (strcmp(chromosome, right.chromosome) == 0)? true : false;
}	//operator==

Organism& Organism::operator=(const Organism& org) {	//assignment operator
	assert(org.initialized == true && org.evaluated == true);
	if (this != &org) {
		*chromosome = *org.chromosome;

		assert(chromosome != NULL);
		fitness = org.fitness;
		initialized = org.initialized;
		evaluated = org.evaluated;
	}	//if
	return *this;
}//operator=
