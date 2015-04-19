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
#include <thread>
using std::cout;
using std::endl;

Simulation::~Simulation(){
	delete population;
}

Simulation::Simulation(int populationSize, int numberOfCutPoints, double mutationProb) {
	this->populationSize = populationSize;
	population = new Population { populationSize };
	this->numberOfCutPoints = numberOfCutPoints;
	this->mutationProb = mutationProb;
}

//TODO move these to header file
void Simulation::run(int numberOfGenerations) {
	this->run(numberOfGenerations, this->numberOfCutPoints, this->mutationProb);
}

void Simulation::run(int numberOfGenerations, int numberOfCutPoints) {
	this->run(numberOfGenerations, numberOfCutPoints, this->mutationProb);
}

void Simulation::run(int numberOfGenerations, double mutationProb) {
	this->run(numberOfGenerations, this->numberOfCutPoints, mutationProb);
}

void Simulation::run(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
		int currentGen { 0 };


		do{
			TestSuiteCrossover(currentGen);
			//TODO Needs range set stuff
			//TODO needs to do tc crossover

			currentGen++;
		} while( currentGen < numberOfGenerations );
}

void Simulation::TestSuiteCrossover(int currentGen) {
	Organism *child1 { }, *child2 { };
	auto parent1Index = population->fitnessProportionalSelect();
	auto parent2Index = population->fitnessProportionalSelect();
	auto parent1 = population->getOrganismByIndex(parent1Index);
	auto parent2 = population->getOrganismByIndex(parent2Index);

	population->crossover(*parent1, *parent2, child1, child2,
			numberOfCutPoints);
	if (currentGen % 5 == 4) {
		population->crossover(child1);
		population->crossover(child2);
	}

	double newProb;
	newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
	child1->mutate(newProb);
	child2->mutate(newProb);

	// Attempt to replace the worst of the two parents
	auto parentToReplace = (parent1 <= parent2 ? parent1Index : parent2Index);

	if (child1 <= child2) {
		if (currentGen % 5 == 0 || population->getCoverageRatio() > 0.95) {
			tryLocalOptimization (child2);
		}
		population->replaceParentThenReplaceWorst(parentToReplace, child2);
		delete child1;
		child1 = NULL;
	} else {
		if (currentGen % 5 == 0 || population->getCoverageRatio() > 0.95) {
			tryLocalOptimization (child1);
		}
		population->replaceParentThenReplaceWorst(parentToReplace, child1);
		delete child2;
		child2 = NULL;
	}
}
//void Simulation::run(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
//	int i { 0 };
//	Organism *child1 { }, *child2 { };
//
//	do{
//		//population->printPopulationFitness();
//		//population->printPopulationCoverage();
//		auto parent1Index = population->fitnessProportionalSelect();
//		auto parent2Index = population->fitnessProportionalSelect();
//		auto parent1 = population->getOrganismByIndex(parent1Index);
//		auto parent2 = population->getOrganismByIndex(parent2Index);
//
//		cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;
//
//		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);
//
//		if(i % 5 == 4 ) {
//			population->crossover(child1);
//			population->crossover(child2);
//		}
//
//		double newProb;
//		newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
//		child1->mutate(newProb);
//		child2->mutate(newProb);
//
//		// Attempt to replace the worst of the two parents
//		auto parentToReplace = ( parent1 <= parent2 ? parent1Index : parent2Index );
//
//		if(child1 <= child2){
//			if( i % 5 == 0 || population->getCoverageRatio() > 0.95 ) {
//				tryLocalOptimization(child2);
//			}
//			population->replaceParentThenReplaceWorst(parentToReplace, child2);
//			delete child1;
//			child1 = NULL;
//		}
//		else {
//			if( i % 5 == 0 || population->getCoverageRatio() > 0.95 ) {
//				tryLocalOptimization(child1);
//			}
//			population->replaceParentThenReplaceWorst(parentToReplace, child1);
//			delete child2;
//			child2 = NULL;
//		}
//		if (i % 100) {
//			//rangeSet->adaptRangesBasedOnUsefulness();
//		}
//
//		i++;
//
//	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);
//
//	population->getBestOrganism()->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
//	population->printPopulationFitness();
//	population->printPopulationCoverage();
//
//	//Organism* final = constructFinalOrganism();
//}

void getUserInput() {
	const int GEN_AND_RATIO = 0, PRINT_FITNESS = 1, PRINT_FITNESS_ONCE = 2,
			COV_RATIO_TS_CROSSOVER = 3, COV_RATIO_TS_CROSSOVER_ONCE = 4,
			REPLACEMENT = 5, CHANGE_LOCAL_OPT = 6, PAUSE = -1, EXIT = -5;
	do {
		int choice;
		std::cin >> choice;
		cout << "\t\tYour choice: " << choice << endl;
		if(choice == EXIT){
			break;
		}

		switch (choice) {
			case GEN_AND_RATIO:
				printGenerationAndRatio = !printGenerationAndRatio;
				break;
			case PRINT_FITNESS:
				printPopFitness = !printPopFitness;
				break;
			case PRINT_FITNESS_ONCE:
				printPopFitnessOnce = !printPopFitnessOnce;
				break;
			case COV_RATIO_TS_CROSSOVER:
				printCoverageRatioForTScrossover = !printCoverageRatioForTScrossover;
				break;
			case COV_RATIO_TS_CROSSOVER_ONCE:
				printCoverageRatioForTScrossoverOnce = !printCoverageRatioForTScrossoverOnce;
				break;
			case REPLACEMENT:
				printReplacement = !printReplacement;
				break;
			case CHANGE_LOCAL_OPT:
				changeLocalOpt = !changeLocalOpt;
				break;
			case PAUSE:
				pause = !pause;
				break;
			default:
				break;
		}
	} while(true);
}

void Simulation::runWithFlags(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
	int i { 0 };
	Organism *child1 { }, *child2 { };

	//std::thread t(getUserInput);


	do{
		while(pause){

		}

		//population->printPopulationFitness();
		//population->printPopulationCoverage();
		auto parent1Index = population->fitnessProportionalSelect();
		auto parent2Index = population->fitnessProportionalSelect();
		auto parent1 = population->getOrganismByIndex(parent1Index);
		auto parent2 = population->getOrganismByIndex(parent2Index);

		if( printGenerationAndRatio )
			cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;

		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);

		if(i % 5 == 4 ) {
			population->crossover(child1);
			population->crossover(child2);
		}

		double newProb;
		newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
		child1->mutate(newProb);
		child2->mutate(newProb);

		// Attempt to replace the worst of the two parents
		auto parentToReplace = ( parent1 <= parent2 ? parent1Index : parent2Index );

		if(child1 <= child2){

			if( changeLocalOpt && ( i % 5 == 0 || population->getCoverageRatio() > 0.95 ) ) {
				tryLocalOptimization(child2);
			}

			population->replaceParentThenReplaceWorst(parentToReplace, child2);
			delete child1;
			child1 = NULL;
		}
		else {

			if(  changeLocalOpt && ( i % 5 == 0 || population->getCoverageRatio() > 0.95) ) {
				tryLocalOptimization(child1);
			}

			population->replaceParentThenReplaceWorst(parentToReplace, child1);
			delete child2;
			child2 = NULL;
		}

		i++;

	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);
	//t.join();
	population->getBestOrganism()->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	population->printPopulationFitness();
	population->printPopulationCoverage();

	//Organism* final = constructFinalOrganism();
}

void Simulation::runWithTournamentSelectAndCrossoverWithDominance(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
	int i { 0 };
	Organism *child1 { };

	do{
		auto parent1Index = population->tournamentSelect();
		auto parent2Index = population->tournamentSelect();
		auto parent1 = population->getOrganismByIndex(parent1Index);
		auto parent2 = population->getOrganismByIndex(parent2Index);

		cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;

		population->crossoverWithDominance(*parent1, *parent2, child1);

		double newProb;
		newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
		child1->mutate(newProb);

		// Attempt to replace the worst of the two parents
		auto parentToReplace = ( parent1 <= parent2 ? parent1Index : parent2Index );

		if( i % 25 == 0 || population->getCoverageRatio() > 0.95 ) {
			tryLocalOptimization(child1);
		}
		population->replaceParentThenReplaceWorst(parentToReplace, child1);

		i++;

	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);

	population->getBestOrganism()->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	population->printPopulationFitness();
	population->printPopulationCoverage();

	Organism* final = constructFinalOrganism();
}

void Simulation::tryLocalOptimization(Organism* child) {
	auto tc = callRandomLocalOpt(child);

	if( tc ) {
		child->getChromosome()->replaceDuplicateTestCase(tc);
		population->evaluateOrganismsFitness(child);
		population->updatePopulationsFitness();
	}
}

TestCase* Simulation::callRandomLocalOpt(Organism* child){
	TestCase* oldTC = child->getChromosome()->getDuplicateTestCase();
	bool edgeOrPredicate { true };

	if( child->getUncoveredEdge() == -1 )
		edgeOrPredicate = false;

	//TODO change probability based on how succesful they are, or try all each time
	switch (uniformInRange(0, 2)) {
		case 0:
			return localOptFromGivenParams(edgeOrPredicate, oldTC);
			break;
		case 1:
			return localOptFromZero(edgeOrPredicate, oldTC);
			break;
		case 2:
			return localOptFromZero(edgeOrPredicate, oldTC);
			//return localOptFromMiddle(uncovered, edgeOrPredicate);
			break;
		default:
			assert(false);
			break;
	}
}

TestCase* Simulation::localOptFromZero (bool edgeOrPredicate, TestCase* oldTC) {
	int NeighborhoodSize { 1 };

	for(int i = 0; i < 1000; ++i) {
		TestCase* tc = new TestCase { };
		tc->setInputParameters(oldTC->getInputParameters());
		for (int j = 0; j < 150; ++j) { //Try searching this particular neighborhood X times


			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {

				auto choice = uniformInRange(1, 100);

				if( choice >= 5 ) { // set the value to somthing in the neighborhood
					int newVal = uniformInRange(-NeighborhoodSize, NeighborhoodSize);
					tc->setInputParameterAtIndex(var, newVal);
				} else {//Move the value closer to zero
					int newVal { tc->getInputParameterAtIndex(var) / (2 * (j + 1)) };
					tc->setInputParameterAtIndex(var, newVal);
				}
			}
			targetCFG->setCoverageOfTestCase(tc);

			auto coverage = ( edgeOrPredicate ? tc->getEdgesCovered() : tc->getPredicatesCovered() );

			if( coveredAnyNewForPopulation(coverage, edgeOrPredicate) ) {
				cout << "Took about " << i * 100 << " tries. " << endl;
				return tc;
			}
		}
		delete tc;
		NeighborhoodSize += 5;
	}

	cout << "Failed to cover edge " << endl;
	return NULL;
}

TestCase* Simulation::localOptFromGivenParams (bool edgeOrPredicate, TestCase* oldTC)  {
	TestCase* tc = new TestCase { };
	int* parameters = new int[targetCFG->getNumberOfParameters()] { };
	int NeighborhoodSize { 1 };

	tc->setInputParametersWithReference(&parameters);

	for(int i = 0; i < 500; ++i) {

		for (int j = 0; j < (15 + NeighborhoodSize/15); ++j) {
			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
				long newValue = oldTC->getInputParameterAtIndex(var) + uniformInRange(-NeighborhoodSize, NeighborhoodSize);

				if( newValue > targetCFG->getUpperBoundForParameter(var) ) {
					newValue = targetCFG->getUpperBoundForParameter(var);
				}

				if( newValue < targetCFG->getLowerBoundForParameter(var) ) {
					newValue = targetCFG->getLowerBoundForParameter(var);
				}


				parameters[var] = newValue;
			}
			targetCFG->setCoverageOfTestCase(tc);

			auto coverage = ( edgeOrPredicate ? tc->getEdgesCovered() : tc->getPredicatesCovered() );
			if( coveredAnyNewForPopulation(coverage, edgeOrPredicate) ) {
				cout << "\tTook about " << i * (10 + NeighborhoodSize/15) << " tries. " << endl;
				return tc;
			}
		}

		NeighborhoodSize += 20;
	}

	cout << "\tFailed to cover edge " << endl;
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
			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
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

bool Simulation::coveredAnyNew(bool* uncovered, bool* covered, bool edge) {
	int num = ( edge ? targetCFG->getNumberOfEdges() : targetCFG->getNumberOfPredicates() );

	for (int i = 0; i < num; ++i) {
		if( uncovered[i] && covered[i] ){
			return true;
		}
	}

	return false;
}

bool Simulation::coveredAnyNewForPopulation(bool* covered, bool edge) {
	int num = ( edge ? targetCFG->getNumberOfEdges() : targetCFG->getNumberOfPredicates() );
	auto popsCover = ( edge ? population->getEdgesCovered() : population->getPredicatesCovered() );
	for (int i = 0; i < num; ++i) {
		if( !popsCover[i] && covered[i] ){
			return true;
		}
	}

	return false;
}

void Simulation::minimizeOrganism(Organism* orgToMinimize) {
	TestSuite* suite = orgToMinimize->getChromosome();
	suite->sortTestSuiteByCoverageCounts();
	for (int i = orgToMinimize->getNumberOfTestCases()-1; i >= 0; i--) {
		if (suite->canRemoveTestCaseWithoutChangingCoverage(i)) {
			suite->removeTestCase(i);
			suite->calculateTestSuiteCoverage();
		}
	}
}

Organism* Simulation::constructFinalOrganism() {
	Organism* finalOrg;

	if (hasEquivalentCoverageToPopulation(population->getBestOrganism())) {
		cout << "Already has all the coverage" << endl;
		finalOrg = new Organism(*population->getBestOrganism());
	}
	else {
		cout << "Doesn't have all the coverage" << endl;
		// Start with a copy of bestOrganismSeen, with room to grow
		TestSuite* bestChromosome = population->getBestOrganism()->getChromosome();
		TestCase** bestTestCasesSeen = bestChromosome->getAllTestCases();
		finalOrg = new Organism(0, bestChromosome->getNumberOfTestCases() * 2, new TestCase*[bestChromosome->getNumberOfTestCases() * 2] { });

		TestSuite* finalSuite = finalOrg->getChromosome();
		for (int i = 0; i < bestChromosome->getNumberOfTestCases(); i++) {
			finalSuite->addTestCase(new TestCase { *bestTestCasesSeen[i] });
		}
		finalSuite->calculateTestSuiteCoverage();

		int* populationEdges = population->getEdgesCovered();
		int* populationPredicates = population->getPredicatesCovered();

		for (int predicateNum = 0; predicateNum < targetCFG->getNumberOfPredicates(); predicateNum++) {
			bool* uncoveredPreds = finalOrg->getChromosome()->getAllUncoveredPredicates();
			if (populationPredicates[predicateNum] > 0 && uncoveredPreds[predicateNum]) {
				for (int j = 0; j < populationSize; j++) {
					TestCase* missingTestCase = population->getOrganismByIndex(j)->getChromosome()->getTestCaseThatCoversPredicate(predicateNum);
					if (missingTestCase != NULL) {
						finalSuite->addTestCase(new TestCase(*missingTestCase));
						finalSuite->calculateTestSuiteCoverage();
						break; // Done with this predicate, find the next
					}
				}
			}
		}

		// I don't believe this is actually necessary since hitting all the predicates should always imply hitting all the edges,
		//	but I included it anyway just in case.
		for (int edgeNum = 0; edgeNum < targetCFG->getNumberOfEdges(); edgeNum++) {
			bool* uncoveredEdges = finalOrg->getChromosome()->getAllUncoveredEdges();
			if (populationEdges[edgeNum] > 0 && uncoveredEdges[edgeNum]) {
				for (int j = 0; j < populationSize; j++) {
					TestCase* missingTestCase = population->getOrganismByIndex(j)->getChromosome()->getTestCaseThatCoversEdge(edgeNum);
					if (missingTestCase != NULL) {
						finalSuite->addTestCase(new TestCase(*missingTestCase));
						finalSuite->calculateTestSuiteCoverage();
						break; // Done with this predicate, find the next
					}
				}
			}
		}
		finalOrg->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
		assert(hasEquivalentCoverageToPopulation(finalOrg));
	}

	minimizeOrganism(finalOrg);
	finalOrg->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	return finalOrg;
}

bool Simulation::hasEquivalentCoverageToPopulation(Organism* organism) {
	int* populationEdges = population->getEdgesCovered();
	int* populationPredicates = population->getPredicatesCovered();
	int* testSuiteEdges = organism->getChromosome()->getEdgeCoverageCounts();
	int* testSuitePredicates = organism->getChromosome()->getPredicateCoverageCounts();

	for (int i = 0; i < targetCFG->getNumberOfEdges(); i++) {
		if (testSuiteEdges[i] == 0 && populationEdges[i] > 0) {
			return false;
		}
	}

	for (int i = 0; i < targetCFG->getNumberOfPredicates(); i++) {
		if (testSuitePredicates[i] == 0 && populationPredicates[i] > 0) {
			return false;
		}
	}
	return true;
}

double Simulation::adaptMutationBasedOnOrganismsCoverage(Organism* org) {
	auto edges = population->getEdgesCovered();
	auto preds = population->getPredicatesCovered();
	auto orgsEdges = org->getChromosome()->getEdgeCoverageCounts();
	auto orgsPreds = org->getChromosome()->getPredicateCoverageCounts();
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

