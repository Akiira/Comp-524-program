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
#include "RangeSet.h"
using std::cout;
using std::endl;

Organism::~Organism(){
	delete chromosome;
	chromosome = NULL;
}

Organism::Organism(const Organism& org) {
	chromosome = new TestSuite { *org.chromosome };
	evaluateBaseFitness();
}

Organism::Organism(int numOfTestCases, int maxNumberOfTestCases, TestCase** testCases) {
	chromosome = new TestSuite { numOfTestCases, maxNumberOfTestCases, testCases };
	fitness = 0;
	scaledFitness = 0;
}

Organism::Organism(int numOfTestCases, int maxNumberOfTestCases ) {
	chromosome = new TestSuite { numOfTestCases, maxNumberOfTestCases};
	evaluateBaseFitness();
}

Organism::Organism(int numberOfTestCases, int maxNumberOfTestCases, Range *range) {
	chromosome = new TestSuite { range, numberOfTestCases, maxNumberOfTestCases};
	evaluateBaseFitness();
}

void Organism::mutate(double mutationProb) {
	int numberOfTestCases = chromosome->getNumberOfTestCases();

	//TODO: Revisit this
	for (int i = 0; i < numberOfTestCases; i++) {
		double toss = uniform01();
		if (toss < mutationProb) {

			TestCase* newTestCase = rangeSet->getNewTestCase();

			if (chromosome->isCoveringNew(newTestCase)) {
				chromosome->replaceDuplicateTestCase(newTestCase);
				// Organism fitness will be reevalated in the simulation loop
				//	so wont do it here.
			}
			delete newTestCase;
		}
	}
}

// An idea I had for a quick fitness function
// Max fitness is numberOfTestCases*numberOfEdges + numberOfTestCases*numberOfPredicates
// Min fitness is 0
// They are punished for covering the same edges and predicates over and over
//	The hope is that this brings organisms that managed to cover the hard to reach test cases to the top
int Organism::fitnessFunction01() {
	int retval = 0;
	int* edgeCoverage = chromosome->getEdgeCoverageCounts();
	int* predicateCoverage = chromosome->getPredicateCoverageCounts();
	int baseReward = chromosome->getNumberOfTestCases() + 1;

	for(int i = 0; i < (targetCFG->getNumberOfEdges()); i++){
		if (edgeCoverage[i] > 0) {
			retval += baseReward - edgeCoverage[i];
		}
	}

	for(int i = 0; i < (targetCFG->getNumberOfPredicates()); i++){
		if (predicateCoverage[i] > 0) {
			retval += baseReward - predicateCoverage[i];
		}
	}
	return retval;
}

// Returns a small number between 0 and numOfEdges+numOfPredicates / numberOfTestCases
int Organism::fitnessFunction02() {
	int retval = 0;
	int* edgeCoverage = chromosome->getEdgeCoverageCounts();
	int* predicateCoverage = chromosome->getPredicateCoverageCounts();

	for(int i = 0; i < (targetCFG->getNumberOfEdges()); i++){
		if (edgeCoverage[i] > 0) {
			retval += 1;
		}
	}

	for(int i = 0; i < (targetCFG->getNumberOfPredicates()); i++){
		if (predicateCoverage[i] > 0) {
			retval += 1;
		}
	}

	// multiply by 10000, because right now were expecting an integer for fitness
	//	and otherwise the integer division almost always will give you 1
	return retval * 10000 / chromosome->getNumberOfTestCases() ;
}

// This should be one using the population coverage stuff
int Organism::fitnessFunction03() {
	//TODO: Wanted to just make population a global variable, problem was witht he typeOfScaling type and enum, they
	//	would have had to be put in population instead because of the forward references. Wanted to talk to you about it
	//	before moving forward.
	//int* populationEdgeCoverage = population->getEdgesCovered();
	//int* populationPredicateCoverage = population->getPredicatesCovered();
	assert(false);
	return 0;
}
void Organism::evaluateBaseFitness(){
	fitness = 0;
	chromosome->calculateTestSuiteCoverage();
	auto counts = chromosome->getEdgeCoverageCounts();

	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
		fitness += (counts[i] ? 1 : 0);
	}

	counts = chromosome->getPredicateCoverageCounts();
	for (int i = 0; i < targetCFG->getNumberOfPredicates(); ++i) {
		fitness += (counts[i] ? 1 : 0);
	}

	// Simply set scaledFitness to fitness here in case were not using scaling
	//	if scaling is used this will be overwritten by a call to Population::scalePopulationFitness
	scaledFitness = fitness;
}

//============================PRINT FUNCTIONS=======================//

void Organism::printAll() {
	cout << "Fitness: " << fitness << ", Scaled Fitness: " << scaledFitness << endl;
	chromosome->printAll();
}
void Organism::printFitnessAndTestSuiteCoverage() const {
	cout << "Fitness: " << fitness << ", Scaled Fitness: " << scaledFitness << endl;
	chromosome->printTestSuiteCoverage();
}
void Organism::printFitnessAndTestSuiteCoverageAndTestCaseInputs() const {
	cout << "Fitness: " << fitness << ", Scaled Fitness: " << scaledFitness << endl;
	chromosome->printTestSuiteCoverage();
	chromosome->printTestCaseInputsOnly();
}

//============================GETTER FUNCTIONS=======================//

int Organism::getUncoveredEdge() const {
	int uncoveredEdge { -1 };

	auto edges = chromosome->getEdgeCoverageCounts();

	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
		if( edges[i] == 0 ) {
			uncoveredEdge = i;
			break;
		}
	}
	return uncoveredEdge;
}

int Organism::getUncoveredPredicate() const {
	int uncoveredPred { -1 };

	auto preds = chromosome->getPredicateCoverageCounts();

	for (int i = 0; i < targetCFG->getNumberOfPredicates(); ++i) {
		if( preds[i] == 0 ) {
			uncoveredPred = i;
			break;
		}
	}
	return uncoveredPred;
}

TestSuite* Organism::getChromosome() const{
	return  chromosome;
}

int Organism::getFitness() const{
	return fitness;
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
assert(false);

//	//They could be the same objects in memory or just have the same
//	// test cases and coverage
//	if( this->chromosome == right->chromosome ) {
//		return true;
//	}
//	else if ( *this->chromosome == *right->chromosome ) {
//		return true;
//	} else {
//		return false;
//	}
}

Organism& Organism::operator=(const Organism& org) {
	if (this != &org) {
		*chromosome = *org.chromosome;

		assert(chromosome != NULL);
		fitness = org.fitness;
	}
	return *this;
}
