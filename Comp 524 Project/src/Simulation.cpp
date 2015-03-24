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

Simulation::Simulation(int populationSize, int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations){

	this->populationSize = populationSize;
	this->testSuiteSize = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	this->numberOfGenerations = numberOfGenerations;
	this->numberOfCutPoints = numberOfCutPoints;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;

	population = new Population { populationSize, testSuiteSize, testSuiteSize };

	bestOrganismSeen = new Organism { 1, testSuiteSize };
	*bestOrganismSeen = *population->getBestOrganism();
}

void Simulation::run(){
	int i { 0 };
	Organism *child1 { }, *child2 { };
	//TODO possibly add some periodic adaptation of parameters like mutation
	do{
		//population->printPopulationFitness();
		//population->printPopulationCoverage();
		auto parent1Index = population->fitnessProportionalSelect();
		auto parent2Index = population->fitnessProportionalSelect();
		auto parent1 = population->getOrganismByIndex(parent1Index);
		auto parent2 = population->getOrganismByIndex(parent2Index);

		cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;

		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);
		double newProb;
		newProb = adaptMutationBasedOnCoverageRatio(probabilityForMutation);
		child1->mutate(newProb);
		child2->mutate(newProb);

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

//		//TODO: there are many different ways we could call/use this. Think about the most appropiate.
//		if( i % 100 == 0 || population->getCoverageRatio() > 0.95 ) {
//			tryLocalOptimization();
//		}

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

	TestSuite* final = constructFinalTestSuite();
}

double Simulation::adaptMutationBasedOnCoverageRatio(double pM) {
	auto cr = population->getCoverageRatio();
	if( cr < 0.25 ) {
		return pM;
	}
	else if( cr < 0.50 ) {
		return pM * 0.75;
	}
	else if( cr < 0.75 ) {
		return pM * 0.60;
	}
	else if ( cr < 0.90 ) {
		return pM * 0.50;
	}
	else {
		return pM * 0.40;
	}
}

double Simulation::adaptMutationBasedOnOrganismsCoverage(Organism* org) {
	auto edges = population->getEdgesCovered();
	auto preds = population->getPredicatesCovered();
	auto orgsEdges = org->getChromosome()->getDuplicateEdgesCovered();
	auto orgsPreds = org->getChromosome()->getDuplicatePredicatesCovered();
	double change = 1.0;

	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
		if( edges[i] > 100 && orgsEdges[i] > 0 ) {
			change += 0.02;
		}
		else if( edges[i] < 10 && orgsEdges[i] > 0 ) { //If this organism is one of the few to cover a
			return 0.0;								// certain edge, then save it.
		}
	}

	for (int i = 0; i < targetCFG->getNumberOfPredicates(); ++i) {
		if( preds[i] > 100 && orgsPreds[i] > 0 ) {
			change += 0.02;
		}
		else if( preds[i] < 10 && orgsPreds[i] > 0 ) { //If this organism is one of the few to cover a
			return 0.0;								// certain edge, then save it.
		}
	}

	return change;
}

TestSuite* Simulation::constructFinalTestSuite() {

	TestSuite* finalSuite = NULL;
	double populationCoverageRatio = population->getCoverageRatio();

	if (bestOrganismSeen->getChromosome()->getCoverageRatio() < populationCoverageRatio) {
		TestSuite* bestTestSuiteSeen = bestOrganismSeen->getChromosome();
		TestCase** bestTestCasesSeen = bestTestSuiteSeen->getAllTestCases();
		int* bestTestSuitePredicates = bestTestSuiteSeen->getDuplicateEdgesCovered();

		TestCase** finalTestCases = new TestCase*[testSuiteSize * 2] { };

		// Start with a copy of bestOrganismSeen
		int finalTestCasesSize = bestOrganismSeen->getNumberOfTestCases();
		for (int i = 0; i < finalTestCasesSize; i++) {
			finalTestCases[i] = new TestCase { *bestTestCasesSeen[i] };
		}

		// GO through predicates, find a missing test case for each predicate that was missing.
		// SHould only need to go through predicates, since predicates imply edges
		for (int predicateNum = 0; predicateNum < targetCFG->getNumberOfPredicates(); predicateNum++) {
			if (bestTestSuitePredicates[predicateNum] == 0) {
				for (int j = 0; j < populationSize; j++) {
					TestCase* missingTestCase = population->getOrganismByIndex(j)->getChromosome()->getTestCaseThatCoversPredicate(predicateNum);
					if (missingTestCase != NULL) {
						// TODO: THis will go over the max number of test cases, figure out what makes the most sense
						finalTestCases[finalTestCasesSize] = new TestCase(*missingTestCase);
						finalTestCasesSize++;
						assert(finalTestCasesSize < testSuiteSize * 2);
						break; // Done with this predicate, find the next
					}
				}
			}
		}

		finalSuite = new TestSuite(finalTestCasesSize, finalTestCasesSize, finalTestCases);
	}
	else {
		finalSuite = new TestSuite(*population->getBestOrganism()->getChromosome());
	}

	finalSuite->calculateTestSuiteCoverage();
	// TODO: COmparing doubles may be a bad idea.
	assert(finalSuite->getCoverageRatio() == populationCoverageRatio);

	finalSuite->printTestSuiteCoverage();
	finalSuite->printTestCaseInputsOnly();

	return finalSuite;
}


//This first version always tries to optimize best organism, we could try other versions as well.
void Simulation::tryLocalOptimization() {
	Organism* bestOrganism = population->getBestOrganism();
	auto tc = callRandomLocalOpt();

	if (tc != NULL) {
		Organism* temp = new Organism { *bestOrganism };
		temp->getChromosome()->replaceRandomTestCase(tc);
		temp->evaluateBaseFitness();

		population->replaceOrganismAtIndexWithChild(0, temp);
	}
}

TestCase* Simulation::callRandomLocalOpt() {
	TestCase* tc = NULL;
	TestCase* oldTC = NULL;
	bool edgeOrPredicate { true };
	Organism* bestOrganism = population->getBestOrganism();
	int uncovered = bestOrganism->getUncoveredEdge();

	if( uncovered == -1 ) {
		edgeOrPredicate = false;
		uncovered = bestOrganism->getUncoveredPredicate();
	}

	switch (uniformInRange(0, 2)) {
		case 0:
			oldTC = bestOrganism->getChromosome()->getTestCase(uniformInRange(0, bestOrganism->getNumberOfTestCases() - 1));
			tc = localOptFromGivenParams(oldTC, uncovered, edgeOrPredicate);
			break;
		case 1:
			tc = localOptFromZero(uncovered, edgeOrPredicate);
			break;
		case 2:
			tc = localOptFromMiddle(uncovered, edgeOrPredicate);//Same as zero local opt in some cases
			break;
		default:
			assert(false);
			break;
	}

	return tc;
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

	for(int i = 0; i < 250; ++i) {

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
	int* middles = new int[targetCFG->getNumberOfParameters()] { };
	int NeighborhoodSize { 1 };

	tc->setInputParametersWithReference(&parameters);

	for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
		int middle { (targetCFG->getLowerBoundForParameter(var)
				+ targetCFG->getUpperBoundForParameter(var)) / 2 };
		middles[var] = middle;
	}

	for(int i = 0; i < 100; ++i) {

		for (int j = 0; j < (10 + NeighborhoodSize); ++j) {
			for (int var = 0; var < 3; ++var) {
				parameters[var] = middles[var] + uniformInRange(-NeighborhoodSize, NeighborhoodSize);
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







