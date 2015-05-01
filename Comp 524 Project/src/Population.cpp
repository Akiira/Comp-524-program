///////////////////////////////////////////////////////////
//  Population.cpp
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "ControlFlowGraph.h"
#include "Population.h"
#include "Random.h"
#include "Organism.h"
#include "RangeSet.h"

#include <cassert>
#include <iostream>

Population::~Population() {
	for (int i = 0; i < populationSize; i++) {
		delete population[i];
	}
	delete[] population;
	delete[] edgesCovered;
	delete[] predicatesCovered;
}

Population::Population(int popSize) {
	edgesCovered = new int[targetCFG->getNumberOfEdges()] { };
	predicatesCovered = new int[targetCFG->getNumberOfPredicates()] { };

	population = new Organism*[popSize] { };
	populationSize = popSize;
	testSuiteSize = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();

	int numberOfRanges = rangeSet->getNumberOfRanges();
	if (numberOfRanges > 0) {
		for (int i = 0; i < populationSize; i++) {
			int rangeNum = i % numberOfRanges;
			population[i] = new Organism { testSuiteSize, testSuiteSize, rangeSet->getRange(rangeNum) };
		}
	}
	else {
		// Something went wrong because at the very least one range is always added to the rangeset.
		assert(false);
	}

	totalFitness = 0;
	updatePopulationsFitness();
}

// PUBLIC -
void Population::updatePopulationsFitness() {
	updateCoverage();

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
	} else if ( SCALING == SHARING ) {
		totalFitness = 0;

		for(int i = 0; i < populationSize; i++) {
			evaluateSharedFitness(population[i]);
			totalFitness += population[i]->getScaledFitness();

			assert(totalFitness >= 0); //Overflow or other problem
		}

	} else if ( totalFitness == 0 ) { // No scaling, initializing fitness for the first time
		int fitness;
		for (int i = 0; i < populationSize; i++) {
			fitness = population[i]->getFitness();
			population[i]->setScaledFitness(fitness);
			totalFitness += fitness;

			assert(totalFitness >= 0); //Overflow or other problem
		}
	} else { // No scaling, only need to take one new organism into account
		totalFitness += lastReplacedFitness;
	}

	// Move this here, because of the why were using shared fitness we can't simply do incremental
	//	updates of the population's sorting.
	sortPopulationByFitness();
}

// PRIVATE - called at beginning of updatePopulationsFitness
void Population::updateCoverage() {
	// Clear any previous coverage
	for (int j = 0; j < targetCFG->getNumberOfEdges(); ++j) {
		edgesCovered[j] = 0;
	}

	for (int j = 0; j < targetCFG->getNumberOfPredicates(); ++j) {
		predicatesCovered[j] = 0;
	}

	// Calculate new coverage
	for (int i = 0; i < populationSize; ++i) {
		population[i]->evaluateBaseFitness();
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

// PRIVATE - called at end of updatePopulationsFitness
void Population::sortPopulationByFitness() {
	int i, j;
	Organism* tmp;
	for (i = populationSize - 1; i > 1; i--) {
		for (j = 0; j < i; j++) {
			if (population[j]->scaledFitness < population[j + 1]->scaledFitness) {
				tmp = population[j];
				population[j] = population[j + 1];
				population[j + 1] = tmp;
			}
		}
	}
}

// Return index instead to be able to pass it to replaceParent
//	and save unnecessary looping to determine the index of the parent to replace
int Population::fitnessProportionalSelect() const {
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

void Population::crossover(const Organism& parent1, const Organism& parent2,
		Organism*& child1, Organism*& child2, int numberOfCutPoints) {

	// Swap things so that parent1TestCases refers to the array with the most test cases
	TestCase** parent1TestCases = parent1.chromosome->getAllTestCases();
	TestCase** parent2TestCases = parent2.chromosome->getAllTestCases();

	int parent1NumberOfTestCases { parent1.getNumberOfTestCases() };
	int parent2NumberOfTestCases { parent2.getNumberOfTestCases() };

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
		}
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



void Population::crossover(const TestCase& parent1, const TestCase& parent2,
		TestCase*& child1, TestCase*& child2, int numberOfCutPoints) {

	auto cutPoints = selectCutPoints(numberOfCutPoints, targetCFG->getNumberOfParameters());
	bool alternate { true };
	int current { 0 };
	child1 = new TestCase();
	child2 = new TestCase();

	for (int i = 0; i < numberOfCutPoints; i++) {
		if (alternate) {
			for (int j = current; j <= cutPoints[i]; j++) {
				child1->setInputParameter(j, parent1.getParameter(j));
				child2->setInputParameter(j, parent2.getParameter(j));
			}
		}
		else {
			for (int j = current; j <= cutPoints[i]; j++) {
				child1->setInputParameter(j, parent2.getParameter(j));
				child2->setInputParameter(j, parent1.getParameter(j));
			}
		}
		current = cutPoints[i] + 1;
		alternate = !alternate;
	}

	delete[] cutPoints;

	if (alternate) {
		for (int j = current; j < parent1.getNumberOfParameters(); j++) {
			child1->setInputParameter(j, parent1.getParameter(j));
			child2->setInputParameter(j, parent2.getParameter(j));
		}
	} else {
		for (int j = current; j < parent1.getNumberOfParameters(); j++) {
			child1->setInputParameter(j, parent2.getParameter(j));
			child2->setInputParameter(j, parent1.getParameter(j));
		}
	}
}

void Population::replaceParentWithUnion(int parentIndex, Organism* child) {
	TestCase** parentTestCases = population[parentIndex]->getChromosome()->getAllTestCases();
	int parentSize = population[parentIndex]->getChromosome()->getNumberOfTestCases();

	TestSuite* childChromosome = child->getChromosome();

	for (int i = 0; i < parentSize; i++) {
		if (childChromosome->isCoveringNew(parentTestCases[i])) {
			childChromosome->replaceDuplicateTestCase(parentTestCases[i]);
		}
	}
	replaceOrganism(parentIndex, child);
}

void Population::replaceParentThenReplaceWorst(int parentIndex, Organism* child) {
	// Don't use scaled fitness here as the child has not been scaled with the population yet.
	if (child->getFitness() >= population[parentIndex]->getFitness()) {
		replaceOrganism(parentIndex, child);
	}
	else {
		replaceWorst(child);
	}
}

void Population::replaceWorst(Organism* child) {
	int worst { populationSize - 1 };
	// Don't use scaled fitness here as the child has not been scaled with the population yet.
	if (child->getFitness() >= population[worst]->getFitness()) {
		replaceOrganism(worst, child);
	} else {
		delete child;
	}
}

// A private utility function to perform the actual replacement, any checking of whether or not to
//	perform the replacement must be done by the calling function.
void Population::replaceOrganism(int organismIndex, Organism* newOrganism) {

	totalFitness -= population[organismIndex]->getScaledFitness();

	delete population[organismIndex];
	population[organismIndex] = newOrganism;

	lastReplacedFitness = newOrganism->getScaledFitness();
}

//From: http://stackoverflow.com/questions/48087/select-a-random-n-elements-from-listt-in-c-sharp
int* Population::selectCutPoints(int &numCutPoints, int upperBound) {

	if(numCutPoints >= upperBound) {
		numCutPoints = uniformInRange(0, upperBound - 1);
	}
	assert(numCutPoints < upperBound);
	assert(numCutPoints >= 0);

	//select numCutPoints randomly in the range [0..orgLength-1]
	//and store their locations in the cutPoints array
	int* cutPoints = new int[numCutPoints] { };
	int index = 0, numLeft = upperBound, needed = numCutPoints;
	for (int i = 0; i < upperBound; ++i) {
		if(needed == 0 || numLeft == 0) {
			break;
		}
		if( uniform01() <= ((double) needed / (double) numLeft)) {
			cutPoints[index] = i;
			index++;
			needed--;
		}
		numLeft--;
	}

	return cutPoints;
}

bool Population::isCoveringNew(const TestCase * tc) const {

	auto newEdge = isCoveringNewEdge(tc->getEdgesCovered());
	auto newPred = isCoveringNewPred(tc->getPredicatesCovered());

	return newEdge || newPred;
}

bool Population::isCoveringNewEdge(const bool * coverage) const {

	auto popsCover = getEdgesCovered();
	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
		if( !popsCover[i] && coverage[i] ){
			return true;
		}
	}

	return false;
}

bool Population::isCoveringNewPred(const bool * coverage) const {

	auto popsCover = getPredicatesCovered();
	for (int i = 0; i < targetCFG->getNumberOfPredicates(); ++i) {
		if( !popsCover[i] && coverage[i] ){
			return true;
		}
	}

	return false;
}

void Population::printPopulationFitness() const {
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

void Population::printPopulationCoverage() const {
	cout << "Population Coverage:" << endl;
	targetCFG->printPopulationCoverage(edgesCovered, predicatesCovered);
	cout << "Coverage Ratio: " << coverageRatio << endl << endl;
}



void Population::evaluateSharedFitness(Organism* org) {
	double sharedFitness = 0;
	auto edges = org->getChromosome()->getEdgeCoverageCounts();

	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
		if ( edges[i] ) {
			assert(edgesCovered[i] > 0);
			sharedFitness += 100.0 * ((double)edges[i] / (double)edgesCovered[i]);
		}
	}

	auto preds = org->getChromosome()->getPredicateCoverageCounts();
	for (int i = 0; i < targetCFG->getNumberOfPredicates(); ++i) {
		if (preds[i]) {
			assert(predicatesCovered[i] > 0);
			sharedFitness +=  100.0 * ((double)preds[i] / (double)predicatesCovered[i]);
		}
	}

	assert(sharedFitness >= 0);
	org->setScaledFitness(sharedFitness + 0.5);
}



void Population::linearScaling() {
	totalFitness = 0;


	//REFERENCE: http://www.cse.unr.edu/~sushil/class/gas/notes/scaling/index.html
	//This scalingFactor "is a scaling constant that specifies the expected number
	//             of copies of the best individual in the next generation"
	double scalingFactor { 5.0 }, average = 0, a, b, d;
	int max { getBestOrganism()->fitness },
	    min { population[populationSize - 1]->getFitness() };

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

		if ( scaledFitness >= 0) {
			population[i]->setScaledFitness(scaledFitness);
			totalFitness += scaledFitness;
		} else {
			population[i]->setScaledFitness(0);
			totalFitness += 0;
		}
	}
}

//========================== OLD FUNCTIONS =====================================//


// Deleted because it was confusing me.
