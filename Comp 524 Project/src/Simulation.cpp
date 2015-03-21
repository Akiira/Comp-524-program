///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "Simulation.h"
#include "Random.h"
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

Simulation::~Simulation(){
	delete population;
	delete bestOrganismSeen;
}

Simulation::Simulation(int populationSize, int initialTestSuiteSize, int maxTestSuiteSize,
		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations){

	this->populationSize = populationSize;
	this->initialTestSuiteSize = initialTestSuiteSize;
	this->maxTestSuiteSize = maxTestSuiteSize;
	this->numberOfGenerations = numberOfGenerations;
	this->numberOfCutPoints = numberOfCutPoints;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;

	population = new Population { populationSize, initialTestSuiteSize, maxTestSuiteSize };

	bestOrganismSeen = new Organism { 1, maxTestSuiteSize };
	*bestOrganismSeen = *population->getBestOrganism();
}

void Simulation::run(){
	int i { 0 };
	Organism *child1 { }, *child2 { };

	do{
		//population->printPopulationFitness();
		//population->printPopulationCoverage();
		auto parent1Index = population->fitnessProportionalSelect();
		auto parent2Index = population->fitnessProportionalSelect();
		auto parent1 = population->getOrganismByIndex(parent1Index);
		auto parent2 = population->getOrganismByIndex(parent2Index);

		cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;

		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);
		child1->mutate(probabilityForMutation);
		child2->mutate(probabilityForMutation);

		assert(child1 && child2);

		//TODO possibly add some periodic adaptation of parameters like mutation

		// Attempt to replace the worst of the two parents
		auto parentToReplace = parent2Index;
		if (parent1 <= parent2) {
			parentToReplace = parent1Index;
		}

		if(child1 <= child2){
			population->replaceParentThenReplaceWorst(parentToReplace, child2);
			delete child1;
		}
		else {
			population->replaceParentThenReplaceWorst(parentToReplace, child1);
			delete child2;
		}

		//TODO: there are many different ways we could call/use this. Think about the most appropiate.
		if( i % 9 == 0 || population->getCoverageRatio() > 0.95 ) {
			tryLocalOptimization();
		}

		i++;

		//TODO Check if it looks like the last generation always have the best organism
		//		and if so, we can remove this. We can check this when we are closer to final version
		if( *bestOrganismSeen < *population->getBestOrganism() ) {
			*bestOrganismSeen = *population->getBestOrganism();
		}

	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);

	bestOrganismSeen->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	population->printPopulationFitness();
	population->printPopulationCoverage();

}

//This first version always tries to optimize best organism, we could try other versions as well.
void Simulation::tryLocalOptimization() {
	bool edgeOrPredicate { true };
	Organism* bestOrganism = population->getBestOrganism();
	int uncovered = bestOrganism->getUncoveredEdge();

	if( uncovered == -1 ) {
		edgeOrPredicate = false;
		uncovered = bestOrganism->getUncoveredPredicate();
	}
	auto oldTC = bestOrganism->getChromosome()->getTestCase(uniformInRange(0, bestOrganism->getNumberOfTestCases() - 1));

	TestCase* tc = localOptFromGivenParams(oldTC, uncovered, edgeOrPredicate);
	//TestCase* tc = localOptVersion1(uncovered, edgeOrPredicate);

	if (tc != NULL) {
		Organism* temp = new Organism { *bestOrganism };
		temp->getChromosome()->replaceRandomTestCase(tc);
		temp->evaluateBaseFitness();

		population->replaceOrganismAtIndexWithChild(0, temp);
	}
}


TestCase* Simulation::localOptFromZero (int thingToCover, bool edgeOrPredicate) {
	TestCase* tc = new TestCase { };
	int* parameters = new int[3] { };
	int NeighborhoodSize { 1 };

	tc->setInputParametersWithReference(&parameters);

	for(int i = 0; i < 100; ++i) {

		for (int j = 0; j < 10; ++j) {
			for (int var = 0; var < 3; ++var) {
				parameters[var] = uniformInRange(-NeighborhoodSize, NeighborhoodSize);
			}
			targetCFG->setCoverageOfTestCase(tc);

			auto coverage = ( edgeOrPredicate ? tc->getEdgesCovered() : tc->getPredicatesCovered() );
			if( coverage[thingToCover] ) {
				cout << "Took about " << i * 10 << " tries. " << endl;
				return tc;
			}
		}

		NeighborhoodSize += 5;
	}

	cout << "Failed to cover edge " << endl;
	return NULL;
}

TestCase* Simulation::localOptFromGivenParams (TestCase* orig, int thingToCover, bool edgeOrPredicate) {
	TestCase* tc = new TestCase { };
	int* parameters = new int[targetCFG->getNumberOfParameters()] { };
	int NeighborhoodSize { 1 };

	tc->setInputParametersWithReference(&parameters);

	for(int i = 0; i < 500; ++i) {

		for (int j = 0; j < (10 + NeighborhoodSize/15); ++j) {
			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
				parameters[var] = orig->getInputParameterAtIndex(var) + uniformInRange(-NeighborhoodSize, NeighborhoodSize);
			}
			targetCFG->setCoverageOfTestCase(tc);

			auto coverage = ( edgeOrPredicate ? tc->getEdgesCovered() : tc->getPredicatesCovered() );
			if( coverage[thingToCover] ) {
				cout << "Took about " << i * 10 << " tries. " << endl;
				return tc;
			}
		}

		NeighborhoodSize += 20;
	}

	cout << "Failed to cover edge " << endl;
	return NULL;
}


TestCase* Simulation::localOptFromMiddle (int thingToCover, bool edgeOrPredicate) {
	TestCase* tc = new TestCase { };
	int* parameters = new int[targetCFG->getNumberOfParameters()] { };
	int NeighborhoodSize { 1 };

	tc->setInputParametersWithReference(&parameters);

	for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
		int middle { (targetCFG->getLowerBoundForParameter(var)
				+ targetCFG->getUpperBoundForParameter(var)) / 2 };
		parameters[var] = middle;
	}

	for(int i = 0; i < 100; ++i) {

		for (int j = 0; j < (10 + NeighborhoodSize); ++j) {
			for (int var = 0; var < 3; ++var) {
				parameters[var] += uniformInRange(-NeighborhoodSize, NeighborhoodSize);
			}
			targetCFG->setCoverageOfTestCase(tc);

			auto coverage = ( edgeOrPredicate ? tc->getEdgesCovered() : tc->getPredicatesCovered() );
			if( coverage[thingToCover] ) {
				cout << "Took about " << i * 10 << " tries. " << endl;
				return tc;
			}
		}

		NeighborhoodSize += 5;
	}

	cout << "Failed to cover edge " << endl;
	return NULL;
}







