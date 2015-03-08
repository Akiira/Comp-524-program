///////////////////////////////////////////////////////////
//  Organism.cpp
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Organism.h"
#include "ControlFlowGraph.h"
#include <iostream>
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

void Organism::print() {
	cout << "Fitness: " << fitness << endl;
	chromosome->printOnlyTestSuiteCoverage();
}
