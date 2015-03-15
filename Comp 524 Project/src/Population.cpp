///////////////////////////////////////////////////////////
//  Population.cpp
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "Population.h"
#include "Random.h"

#include <cassert>

Population::~Population() {
	for (int i = 0; i < populationSize; i++) {
		delete population[i];
	}
	delete[] population;
}

/**
 * Creates a new population of random test suites
 */
Population::Population(int popSize, int initialTestSuiteSize, int maxTestSuiteSize) {
	population = new Organism*[popSize] { };
	populationSize = popSize;
	int numOfGroups { (popSize / (maxTestSuiteSize-4)) };
	int numOfTests { 3 };

	for (int i = 0; i < popSize; i++) {

		population[i] = new Organism { initialTestSuiteSize, maxTestSuiteSize };
		//population[i] = new Organism { numOfTests, maxTestSuiteSize };

		if( i % numOfGroups == 0 ) {
			numOfTests++;
		}
	}
	totalFitness = 0;
	setPopulationFitness();
}

void Population::crossover(const Organism& parent1, const Organism& parent2,
		Organism*& child1, Organism*& child2, int numberOfCutPoints) {

	int parent1NumberOfTestCases = parent1.getNumberOfTestCases();
	int parent2NumberOfTestCases = parent2.getNumberOfTestCases();

	int parent1MaxNumberOfTestCases = parent1.getMaxNumberOfTestCases();
	int parent2MaxNumberOfTestCases = parent2.getMaxNumberOfTestCases();

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

	TestCase** child1TestCases = new TestCase*[parent1MaxNumberOfTestCases] { };
	TestCase** child2TestCases = new TestCase*[parent2MaxNumberOfTestCases] { };

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
	delete[] cutPoints;
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

	child1 = new Organism { parent1NumberOfTestCases, parent1.getMaxNumberOfTestCases(), child1TestCases };
	child2 = new Organism { parent2NumberOfTestCases,	parent2.getMaxNumberOfTestCases(), child2TestCases };
}

//TODO decide if we want this
void Population::crossover(const TestCase& parent1, const TestCase& parent2,
		TestCase*& child1, TestCase*& child2, int numberOfCutPoints) {
	auto cutPoints = selectCutPoints(numberOfCutPoints, parent1.getNumberOfParameters());
	bool alternate { true };
	int current { 0 };

	for (int i = 0; i < numberOfCutPoints; i++) {
		if (alternate) {
			for (int j = current; j <= cutPoints[i]; j++) {
				child1->setInputParameterAtIndex(j, parent1.getInputParameterAtIndex(j));
				child2->setInputParameterAtIndex(j, parent2.getInputParameterAtIndex(j));
			}
		}
		else {
			for (int j = current; j <= cutPoints[i]; j++) {
				child1->setInputParameterAtIndex(j, parent2.getInputParameterAtIndex(j));
				child2->setInputParameterAtIndex(j, parent1.getInputParameterAtIndex(j));
			}
		}
		current = cutPoints[i] + 1;
		alternate = !alternate;
	}

	delete[] cutPoints;

	if (alternate) {
		for (int j = current; j < parent1.getNumberOfParameters(); j++) {
			child1->setInputParameterAtIndex(j, parent1.getInputParameterAtIndex(j));
			child2->setInputParameterAtIndex(j, parent2.getInputParameterAtIndex(j));
		}
	} else {
		for (int j = current; j < parent1.getNumberOfParameters(); j++) {
			child1->setInputParameterAtIndex(j, parent2.getInputParameterAtIndex(j));
			child2->setInputParameterAtIndex(j, parent1.getInputParameterAtIndex(j));
		}
	}
}

void Population::scalePopulationsFitness() {

	if( SCALING == LINEAR ) {
		totalFitness = 0;
		int max { getBestOrganism()->fitness },
			min { population[populationSize - 1]->getFitness() };
		auto a = max;
		auto b = -min / populationSize;

		for(int i = 0; i < populationSize; i++) {
			auto f = population[i]->getFitness();
			population[i]->setScaledFitness(a + (b * f));
			totalFitness += a + (b * f);
		}
	}
	else if ( SCALING == EXPONENTIAL ) {
		totalFitness = 0;
//This kept overflowing, I think the paper i read left out some details
//		auto base = 1.25;
//		auto power = 0;
//		for(int i = populationSize - 1; i > 0; i--) {
//			//population[i]->setFitness(pow(base, power));
//			cout << "Setting :" << i << " to :" << pow(base, power) << endl;
//			population[i]->setScaledFitness(pow(base, power));
//			power++;
//		}
		int start { 1 },
		    delta { 1 };

		for(int i = populationSize - 1; i > 0; i--) {
			population[i]->setScaledFitness(start);
			totalFitness += start;
			assert(totalFitness > 0); //assert we have not overflowed
			if( i % 100 == 99 ) {
				delta += 1;
			}

			start += delta;
		}
	}
	else if ( SCALING == RANKED ) { //Ranking fitness
		totalFitness = 0;
		short rank { 1 };
		for(int i = populationSize - 1; i > 0; i--) {
				population[i]->setScaledFitness(rank);
				totalFitness += rank;
				rank++;
			}
	} else { // no scaling //TODO this is just for testing, can be much more efficient when we dont scale
		for(int i = 0; i < populationSize; i++) {
			population[i]->setScaledFitness(population[i]->getFitness());
		}
	}
}

int* Population::selectCutPoints(int numCutPoints, int upperBound) {
	assert(numCutPoints < upperBound);

	//select numCutPoints randomly in the range [0..orgLength-1]
	//and store their locations in the cutPoints array
	int* cutPoints = new int[numCutPoints];

	int m = 0;   // the number of points selected so far
	int i = 0;  //index for cutPoints array

	for (int t = 0; (t < upperBound) && (m < numCutPoints); t++) {
		if (((upperBound - t) * uniform01()) < (numCutPoints - m)) {
			cutPoints[i] = t;
			i++;
			m++;
		}
	}
	return cutPoints;
}

void Population::replaceParentWithChild(Organism* parent, Organism* child) {

	for(int i = 0; i < populationSize; i++) {

		if( population[i] == parent ) {
			delete population[i];
			population[i] = child;
			break;
		}
	}

	//TODO depending on when we do this, the population array may be out of order and need sorted
}

void Population::replace(Organism* child) {
	int worst { populationSize - 1 };

	if (child->getFitness() >= population[worst]->getFitness()) {
		//totalFitness += child->getFitness() - population[worst]->getFitness();
		delete population[worst];
		population[worst] = child;

		scalePopulationsFitness();

		int i = populationSize - 1;
		Organism* tmp;

		//now move the new child to the correct position in the popArray
		while ((i > 0)
				&& (population[i]->getFitness()
						> population[i - 1]->getFitness())) {
			tmp = population[i];
			population[i] = population[i - 1];
			population[i - 1] = tmp;
			i--;
		}
	} else {
		delete child;
	}
}

// Straight from GA0
Organism* Population::fitnessProportionalSelect() {
//If totalFitness is zero then an organism is selected at random.
	long toss;
	int i = 0;
	int sum;

	if (totalFitness == 0) {
		i = uniformInRange(0, populationSize - 1);
	} else {
		//sum = population[0]->getFitness();
		sum = population[0]->getScaledFitness();
		toss = uniformInRange(0, totalFitness);

		while (sum < toss) {
			i++;
			//sum += population[i]->getFitness();
			sum += population[i]->getScaledFitness();
		}
	}

	return population[i];
}


// Straight from GA0
void Population::setPopulationFitness() {
	int i, j;
//	totalFitness = 0;
//	for (i = 0; i < populationSize; i++) {
//		totalFitness += population[i]->getFitness();
//	}
	scalePopulationsFitness();

	//now sort popArray so that the organisms are in order of fitness
	//from highest to lowest.
	Organism* tmp;
	for (i = populationSize - 1; i > 1; i--) {
		for (j = 0; j < i; j++) {
			if (population[j]->fitness < population[j + 1]->fitness) {
				tmp = population[j];
				population[j] = population[j + 1];
				population[j + 1] = tmp;
			}
		}
	}
}

void Population::printPopulationFitness() {
	int bestFitness = population[0]->getFitness();
	int worstFitness = population[populationSize-1]->getFitness();
	cout << "Best Fitness: " << bestFitness << endl;
	cout << "Worst Fitness: " << worstFitness << endl;
	cout << "Difference between best and worst: " << bestFitness - worstFitness << endl;
}

Organism* Population::getBestOrganism() const {
	return population[0];
}
