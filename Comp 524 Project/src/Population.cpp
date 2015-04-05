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
	edgesCovered = new int[targetCFG->getNumberOfEdges()] { };
	predicatesCovered = new int[targetCFG->getNumberOfPredicates()] { };
	population = new Organism*[popSize] { };
	populationSize = popSize;

	for (int i = 0; i < popSize; i++) {
		population[i] = new Organism { initialTestSuiteSize, maxTestSuiteSize };
	}

	totalFitness = 0;

	computePopulationLevelCoverage();

	for (int i = 0; i < popSize; i++) {
		evaluateOrganismsFitness(population[i]);
	}

	updatePopulationsFitness();
	sortPopulationByFitness();
}

void Population::evaluateOrganismsFitness(Organism* org) {
	double base = 1.0;
	auto edges = org->getChromosome()->getEdgeCoverageCounts();
	org->evaluateBaseFitness();

	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
		if ( edges[i] ) {
			int timesCoveredByPopulation = edgesCovered[i];
			if ( timesCoveredByPopulation < 5 && base < 5.0 ) {
				base *= 3.0;
			} else if ( timesCoveredByPopulation < 10 && base < 5.0 ) {
				base *= 2.0;
			} else if ( timesCoveredByPopulation < 20 && base < 5.0 ) {
				base *= 1.5;
			} else if ( timesCoveredByPopulation < 50 && base < 5.0 ) {
				base *= 1.0;
			} else if ( timesCoveredByPopulation < 100 && base > 0.5  ) {
				base *= 0.95;
			} else if ( timesCoveredByPopulation < 200 && base > 0.5  ) {
				base *= 0.90;
			} else if ( base > 0.5 ) {
				base *= 0.85;
			}
		}
	}

	auto preds = org->getChromosome()->getPredicateCoverageCounts();

	for (int i = 0; i < targetCFG->getNumberOfPredicates(); ++i) {
		if (preds[i]) {
			int timesCoveredByPopulation = preds[i];
			if (timesCoveredByPopulation < 5 && base < 5.0) {
				base *= 3.0;
			} else if (timesCoveredByPopulation < 10 && base < 5.0) {
				base *= 2.0;
			} else if (timesCoveredByPopulation < 20 && base < 5.0) {
				base *= 1.5;
			} else if (timesCoveredByPopulation < 50 && base < 5.0) {
				base *= 1.0;
			} else if (timesCoveredByPopulation < 100 && base > 0.5 ) {
				base *= 0.95;
			} else if (timesCoveredByPopulation < 200 && base > 0.5 ) {
				base *= 0.90;
			} else if ( base > 0.5 ) {
				base *= 0.85;
			}
		}
	}

	org->setFitness(org->getFitness() * base);
}

void Population::updatePopulationsFitness() {
	if( SCALING == LINEAR ) {
		linearScaling();
	}
	else if ( SCALING == EXPONENTIAL ) {
		totalFitness = 0;

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
	} else if ( totalFitness == 0 ) { // No scaling, initializing fitness for the first time
		int fitness;
		for (int i = 0; i < populationSize; i++) {
			fitness = population[i]->getFitness();
			population[i]->setScaledFitness(fitness);
			totalFitness += fitness;
		}
	} else { // No scaling, only need to take one new organism into account
		totalFitness += lastReplacedFitness;
	}
}

// TODO: Looking at this I realized that Population is a friend of Organism
//	so fitness and chromosome and coverage can be accessed without getters.
//	How much more efficient is this and should we go through and change all to
//	be this way?
//
void Population::sortPopulationByFitness() {
	int i, j;
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

void Population::computePopulationLevelCoverage() {
	// Clear any previous coverage
	for (int j = 0; j < targetCFG->getNumberOfEdges(); ++j) {
		edgesCovered[j] = 0;
	}

	for (int j = 0; j < targetCFG->getNumberOfPredicates(); ++j) {
		predicatesCovered[j] = 0;
	}

	// Calculate new coverage
	for (int i = 0; i < populationSize; ++i) {
		auto edgeCov = population[i]->chromosome->getEdgeCoverageCounts();
		auto predCov = population[i]->chromosome->getPredicateCoverageCounts();

		for (int j = 0; j < targetCFG->getNumberOfEdges(); ++j) {
			edgesCovered[j] += edgeCov[j];
		}

		for (int j = 0; j < targetCFG->getNumberOfPredicates(); ++j) {
			predicatesCovered[j] += predCov[j];
		}
	}
	// Calculate the coverage ratio
	double numCovered = 0;
	for (int j = 0; j < targetCFG->getNumberOfEdges(); ++j) {
		if (edgesCovered[j] > 0) {
			numCovered++;
		}
	}
	for (int j = 0; j < targetCFG->getNumberOfPredicates(); ++j) {
		if (predicatesCovered[j] > 0) {
			numCovered++;
		}
	}
	coverageRatio = numCovered / (targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates());
}

// Return index instead to ultimately be able to pass it to replaceParent
//	and save unnecessary looping to determine the index of the parent to replace
int Population::fitnessProportionalSelect() {
//If totalFitness is zero then an organism is selected at random.
	long toss;
	int i = 0;
	int sum;

	if (totalFitness == 0) {
		i = uniformInRange(0, populationSize - 1);
	} else {
		sum = population[0]->getScaledFitness();
		toss = uniformInRange(0, totalFitness);

		while (sum < toss) {
			i++;
			sum += population[i]->getScaledFitness();
		}
	}

	return i;
}


int Population::randomSelect() {
	return uniformInRange(0, populationSize-1);
}

int Population::tournamentSelect() {
	int numSelected = .10 * populationSize;

	int bestIndex, bestFitness;
	for (int i = 0; i < numSelected; i++) {
		int next = randomSelect();
		if (population[next]->getScaledFitness() > bestFitness) {
			bestIndex = i;
			bestFitness = population[i]->getScaledFitness();
		}
	}

	return bestIndex;
}

void Population::crossoverWithDominance(const Organism& parent1, const Organism& parent2, Organism*& child1) {
	// This crossover assumes same sized parents
	assert(parent1.getNumberOfTestCases() == parent2.getNumberOfTestCases());

	TestCase** betterTestCases;
	TestCase** worseTestCases;

	int betterParent { ( parent1.getScaledFitness() >= parent2.getScaledFitness() ? 1 : 2 ) };
	int tossBoundary = 0;
	switch(betterParent) {
		case 1:
			if (parent1.getScaledFitness() == 0) {
				tossBoundary = 0;
			}
			else {
				tossBoundary = parent2.getScaledFitness() / parent1.getScaledFitness() * 50;
			}
			betterTestCases = parent1.chromosome->getAllTestCases();
			worseTestCases = parent2.chromosome->getAllTestCases();
			break;
		case 2:
			if (parent1.getScaledFitness() == 0) {
				tossBoundary = 0;
			}
			else {
				tossBoundary = parent2.getScaledFitness() / parent1.getScaledFitness() * 50;
			}
			tossBoundary = parent1.getScaledFitness() / parent2.getScaledFitness() * 50;
			betterTestCases = parent2.chromosome->getAllTestCases();
			worseTestCases = parent1.chromosome->getAllTestCases();
			break;
	}

	int testSuiteSize = parent1.getNumberOfTestCases();


	TestCase** child1TestCases = new TestCase*[testSuiteSize] { };

	for (int i = 0; i < parent1.getNumberOfTestCases(); i++) {
		int toss = uniformInRange(1, 100);
		if (toss < tossBoundary) {
			child1TestCases[i] = new TestCase { *worseTestCases[i] };
		}
		else {
			child1TestCases[i] = new TestCase { *betterTestCases[i] };
		}
	}

	child1 = new Organism { testSuiteSize, testSuiteSize, child1TestCases };
}

void Population::crossover(const Organism& parent1, const Organism& parent2,
		Organism*& child1, Organism*& child2, int numberOfCutPoints) {

	// Swap things so that parent1TestCases refers to the array with the most test cases
	TestCase** parent1TestCases = (parent1.getNumberOfTestCases() >= parent2.getNumberOfTestCases() ? parent1.chromosome->getAllTestCases() : parent2.chromosome->getAllTestCases());
	TestCase** parent2TestCases = (parent1.getNumberOfTestCases() >= parent2.getNumberOfTestCases() ? parent2.chromosome->getAllTestCases() : parent1.chromosome->getAllTestCases());

	int parent1NumberOfTestCases { ( parent1.getNumberOfTestCases() >= parent2.getNumberOfTestCases() ? parent1.getNumberOfTestCases() : parent2.getNumberOfTestCases()) };
	int parent2NumberOfTestCases { ( parent1.getNumberOfTestCases() >= parent2.getNumberOfTestCases() ? parent2.getNumberOfTestCases() : parent1.getNumberOfTestCases()) };

	// Use parent2NumberOfTestCases as the upperBound of cutpoints since parent2 must be <= parent1
	int* cutPoints = selectCutPoints(numberOfCutPoints,	parent2NumberOfTestCases);

	TestCase** child1TestCases = new TestCase*[parent1.getMaxNumberOfTestCases()] { };
	TestCase** child2TestCases = new TestCase*[parent2.getMaxNumberOfTestCases()] { };

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
	child2 = new Organism { parent2NumberOfTestCases, parent2.getMaxNumberOfTestCases(), child2TestCases };
}

//TODO decide when we use this
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

int* Population::selectCutPoints(int numCutPoints, int upperBound) {
	assert(numCutPoints < upperBound);

	//select numCutPoints randomly in the range [0..orgLength-1]
	//and store their locations in the cutPoints array
	int* cutPoints = new int[numCutPoints] { };

	int m { 0 };   // the number of points selected so far
	int i { 0 };  //index for cutPoints array

	for (int t = 0; (t < upperBound) && (m < numCutPoints); t++) {
		if (((upperBound - t) * uniform01()) < (numCutPoints - m)) {
			cutPoints[i] = t;
			i++;
			m++;
		}
	}
	return cutPoints;
}

void Population::replaceParentThenReplaceWorst(int parentIndex, Organism* child) {
	if (child->getFitness() >= population[parentIndex]->getFitness()) {
		replaceOrganismAtIndexWithChild(parentIndex, child);
	}
	else {
		replaceWorst(child);
	}
}

void Population::replaceWorst(Organism* child) {
	int worst { populationSize - 1 };

	if (child->getFitness() >= population[worst]->getFitness()) {
		replaceOrganismAtIndexWithChild(worst, child);
	} else {
		delete child;
		child = NULL;
	}
}

// A private utility function to perform the actual replacement, any checking of whether or not to
//	perform the replacement must be done by the calling function.
void Population::replaceOrganismAtIndexWithChild(int organismToReplace, Organism* child) {

	updateCoverageBeforeReplacement(organismToReplace, child);

	totalFitness -= population[organismToReplace]->getScaledFitness();

	delete population[organismToReplace];
	population[organismToReplace] = child;

	lastReplacedFitness = child->getFitness();
	updatePopulationsFitness();

	moveOrganismToSortedPosition(organismToReplace);
}

// Shared by all replacement schemes, should be called before the organismToBeReplaced is deleted
void Population::updateCoverageBeforeReplacement(int organismToBeReplaced, Organism* child) {
	auto replacedEdgeCov = population[organismToBeReplaced]->getChromosome()->getEdgeCoverageCounts();
	auto replacedPredCov = population[organismToBeReplaced]->getChromosome()->getPredicateCoverageCounts();
	auto childEdgeCov = child->chromosome->getEdgeCoverageCounts();
	auto childPredCov = child->chromosome->getPredicateCoverageCounts();

	int numCovered = 0;
	for (int j = 0; j < targetCFG->getNumberOfEdges(); ++j) {

		edgesCovered[j] += childEdgeCov[j] - replacedEdgeCov[j];
		if (edgesCovered[j] > 0) {
			numCovered++;
		}
	}
	for (int j = 0; j < targetCFG->getNumberOfPredicates(); ++j) {
		predicatesCovered[j] += childPredCov[j] - replacedPredCov[j];
		if (predicatesCovered[j] > 0) {
			numCovered++;
		}
	}
	coverageRatio = ((double) numCovered) / (targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates());

}

// Note this method assumes the organism at indexToSort is >= the organism as indexToSort+1
//	this is valid since all of our replacement schemes only occur if the replacement is better
//	than the Organism that was replaced.
void Population::moveOrganismToSortedPosition(int indexToSort) {
	Organism* tmp;
	int i = indexToSort;
	// Move the child left while its fitness is greater than it's left neighbor
	while ((i > 0) && (population[i]->getFitness() > population[i - 1]->getFitness())) {
		tmp = population[i];
		population[i] = population[i - 1];
		population[i - 1] = tmp;
		i--;
	}
}


void Population::printPopulationFitness() {
	int bestFitness = population[0]->getFitness();
	int bestFitnessScaled = population[0]->getScaledFitness();
	int worstFitness = population[populationSize-1]->getFitness();
	int worstFitnessScaled = population[populationSize-1]->getScaledFitness();
	cout << "Best Fitness: " << bestFitness << endl;
	cout << "Best Fitness Scaled: " << bestFitnessScaled << endl;
	cout << "Worst Fitness: " << worstFitness << endl;
	cout << "Worst Fitness Scaled: " << worstFitnessScaled << endl;
	cout << "Difference between best and worst: " << bestFitness - worstFitness << endl;
	cout << "Difference between best and worst Scaled: " << bestFitnessScaled - worstFitnessScaled << endl;
}

void Population::printPopulationCoverage() {
	cout << "Population Coverage:" << endl;
	targetCFG->printPopulationCoverage(edgesCovered, predicatesCovered);
	cout << "Coverage Ratio: " << coverageRatio << endl << endl;
}



void Population::linearScaling() {
	totalFitness = 0;

	//TODO scaling factor needs to be updated dynamicly based on the current population

	//REFERENCE: http://www.cse.unr.edu/~sushil/class/gas/notes/scaling/index.html
	//This scalingFactor "is a scaling constant that specifies the expected number
	//             of copies of the best individual in the next generation"
	double scalingFactor { 5.0 }, average = 0, a, b, d;
	int max { getBestOrganism()->fitness },
	    min { population[populationSize - 1]->getFitness() };

	//TODO: if we keep this scaling we should have a separate field for scaledTotalFitness so we
	//		can avoid this extra loop
	for (int i = 0; i < populationSize; ++i) {
		average += population[i]->fitness;
	}

	average /= populationSize;

	// Find values for the scaling coefficients
	if (min > ((scalingFactor * average - max) / (scalingFactor - 1.0))) {
		d = max - average;
		a = (scalingFactor - 1.0) * (average / d);
		b = average * (max - (scalingFactor * average)) / d;
	} else {
		d = average - min;
		a = average / d;
		b = -min * (average / d);
	}
	if (d < 0.00001 && d > -0.00001) {
		a = 1.0;
		b = 0.0;
	}

	for (int i = 0; i < populationSize; i++) {
		auto f = population[i]->getFitness();
		auto scaledFitness = b + (a * f);
		//cout << "Fitness: " << f << ", scaled: " << (b + (a*f)) << endl;
		if ( scaledFitness >= 0) {
			population[i]->setScaledFitness(scaledFitness);
			totalFitness += scaledFitness;
		} else {
			population[i]->setScaledFitness(0);
			totalFitness += 0;
		}
	}
}
