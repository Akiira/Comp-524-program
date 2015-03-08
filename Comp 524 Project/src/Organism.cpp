///////////////////////////////////////////////////////////
//  Organism.cpp
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Organism.h"
#include "ControlFlowGraph.h"
#include <iostream>
#include <cassert>
#include "Random.h"
using std::cout;
using std::endl;

Organism::~Organism(){
	delete chromosome;
}

// Note have to explicitly call a initializeChromosome method to create the chromosome.
//	and have to explicitly call setFitness to evaluate the fitness of the chromosome.
Organism::Organism(ControlFlowGraph& target) {
	targetCFG = &target;
	initialized = false;
	evaluated = false;
	fitness = 0;
	chromosome = 0;
}

/**
 * IMPORTANT: Perform deep copy on the testCases before calling this.
 * Takes an array of pointers to test cases which will ultimately be directly assigned to the testCases array
 * of a new TestSuite instance (that will become the chromosome of this Organism)
 * Necessary for crossover
 */
void Organism::initializeChromosomeFromTestCases(int numberOfTestCases, TestCase** testCases) {
	chromosome = new TestSuite(numberOfTestCases, testCases, targetCFG);
	initialized = true;
}

void Organism::initializeRandomChromosome(int numberOfTestCases) {
	chromosome = new TestSuite(numberOfTestCases, targetCFG);
	initialized = true;
}

TestSuite* Organism::getChromosome() const{
	assert(initialized == true);
	return  chromosome;
}

int Organism::getFitness() const{
	//assert(evaluated == true);
	return fitness;
}


void Organism::mutate(double mutationProb) {
	assert(initialized == true);
	int numberOfTestCases = chromosome->getNumberOfTestCases();
	for (int i = 0; i < numberOfTestCases; i++) {
		double toss = uniform01();
		if (toss < mutationProb) {
			TestCase* newTestCase = new TestCase(*targetCFG);
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
		chromosome->~TestSuite();
		chromosome = org.chromosome;

		//TODO implement CFG assignment op
		targetCFG = org.targetCFG;
		assert(chromosome != NULL);
		fitness = org.fitness;
		initialized = org.initialized;
		evaluated = org.evaluated;
	}	//if
	return *this;
}//operator=

void Organism::print() {
	cout << "Fitness: " << fitness << endl;
	chromosome->printOnlyTestSuiteCoverage();
}
