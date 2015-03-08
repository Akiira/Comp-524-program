///////////////////////////////////////////////////////////
//  Population.cpp
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Population.h"
#include "Random.h"

#include <cassert>
#include "GlobalVariables.h"

Population::~Population() {

}

/**
 * Creates a new population of random test suites
 */
Population::Population(int popSize, int initialTestSuiteSize) {
	assert(targetCFG);
	population = new Organism*[popSize];
	populationSize = popSize;
	this->initialTestSuiteSize = initialTestSuiteSize;
	for (int i = 0; i < popSize; i++) {
		population[i] = new Organism(initialTestSuiteSize);
	}
	totalFitness = 0;
}

void Population::crossover(const Organism& parent1, const Organism& parent2,
		Organism*& offspring1, Organism*& offspring2, int numberOfCutPoints) {

	int parent1NumberOfTestCases = parent1.getNumberOfTestCases();
	int parent2NumberOfTestCases = parent2.getNumberOfTestCases();

	TestCase**parent1TestCases { };
	TestCase**parent2TestCases { };

	// Swap things so that parent1TestCases refers to the array with the most test cases
	if (parent1NumberOfTestCases >= parent2NumberOfTestCases) {
		parent1TestCases = parent1.chromosome->getAllTestCases();
		parent2TestCases = parent2.chromosome->getAllTestCases();
	} else {
		parent1TestCases = parent2.chromosome->getAllTestCases();
		parent2TestCases = parent1.chromosome->getAllTestCases();
		int tmp = parent1NumberOfTestCases;
		parent1NumberOfTestCases = parent2NumberOfTestCases;
		parent2NumberOfTestCases = tmp;
	}

	// Use parent2NumberOfTestCases as the upperBound of cutpoints since parent2 must be <= parent1
	int* cutPoints = selectCutPoints(numberOfCutPoints,	parent2NumberOfTestCases);

	TestCase** child1TestCases = new TestCase*[parent1NumberOfTestCases] { };
	TestCase** child2TestCases = new TestCase*[parent2NumberOfTestCases] { };

	bool alternate { true };
	int current { 0 }; //the overall finger through all chromosomes (parents & offspring)
	for (int i = 0; i < numberOfCutPoints; i++) {
		if (alternate) {
			for (int j = current; j <= cutPoints[i]; j++) {
				child1TestCases[j] = new TestCase { *parent1TestCases[j] };
				child2TestCases[j] = new TestCase { *parent2TestCases[j] };
			}
		}  //if
		else {
			for (int j = current; j <= cutPoints[i]; j++) {
				child1TestCases[j] = new TestCase { *parent2TestCases[j] };
				child2TestCases[j] = new TestCase { *parent1TestCases[j] };
			}
		}
		current = cutPoints[i] + 1;
		alternate = !alternate;
	}

	//now take care of the last segments, if any
	if (alternate) {
		for (int j = current; j < parent2NumberOfTestCases; j++) {
			child1TestCases[j] = new TestCase { *parent1TestCases[j] };
			child2TestCases[j] = new TestCase { *parent2TestCases[j] };
		}
	} else {
		for (int j = current; j < parent2NumberOfTestCases; j++) {
			child1TestCases[j] = new TestCase { *parent2TestCases[j] };
			child2TestCases[j] = new TestCase { *parent1TestCases[j] };
		}
	}

	// Now fill in remaining test cases of offspring1 from parent1 (if parent1 had more than parent2)
	for (int j = parent2NumberOfTestCases; j < parent1NumberOfTestCases; j++) {
		child1TestCases[j] = new TestCase { *parent1TestCases[j] };
	}

	offspring1 = new Organism { parent1.getNumberOfTestCases(), child1TestCases };
	offspring2 = new Organism { parent2.getNumberOfTestCases(),	child2TestCases };
}

int* Population::selectCutPoints(int numCutPoints, int upperBound) {
	//select numCutPoints randomly in the range [0..orgLength-1]
	//and store their locations in the cutPoints array
	int* cutPoints = new int[numCutPoints];

	int m = 0;   // the number of points selected so far
	int i = 0;  //index for cutPoints array

	// GA0 also had (t <= orgLength) here, pretty sure it should be <, otherwise cutpoint could be orgLength
	for (int t = 0; (t < upperBound) && (m < numCutPoints); t++) {
		if (((upperBound - t) * uniform01()) < (numCutPoints - m)) {
			cutPoints[i] = t;
			i++;
			m++;
		}
	}  //for
	return cutPoints;
}  //selectCutPoints

void Population::replace(Organism& offspring) {

}

//TODO implement fitness proportional selection
Organism* Population::select() {
	return population[uniformInRange(0, populationSize - 1)];
}

void Population::setPopulationFitness() {

}
