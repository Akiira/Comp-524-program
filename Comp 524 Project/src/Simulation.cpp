///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "Simulation.h"
#include "ControlFlowGraph.h"
#include "TestSuite.h"
#include "GlobalVariables.h"
#include "Population.h"
#include "Random.h"
#include "Range.h"
#include "RangeSet.h"
#include "Organism.h"
#include <iostream>
#include <cassert>
#include <thread>
using std::cout;
using std::endl;


Simulation::~Simulation(){
	delete population;
	delete rangeSet;
}

Simulation::Simulation(int populationSize, int numberOfCutPoints, double mutationProb) {
	findPromisingRangesAndCreateTheGlobalRangeSet();

	this->populationSize    = populationSize;
	this->numberOfCutPoints = numberOfCutPoints;
	this->mutationProb      = mutationProb;

	population = new Population { populationSize };
}

int Simulation::run(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
	this->numberOfCutPoints = numberOfCutPoints;
	this->mutationProb      = mutationProb;
	int currentGen { 0 };

	double lastGensCoverage = population->getCoverageRatio();
	int gensOfNoImprov = 0;

	do {
		cout << "\tGeneration # " << currentGen << " CoverageRatio: " << population->getCoverageRatio() << endl;

		if( gensOfNoImprov == 20 || currentGen % 30 == 0 ){
			testSuiteCrossover(currentGen);
		}

		testCaseCrossover();

		if( lastGensCoverage <= population->getCoverageRatio() ) {
			lastGensCoverage = population->getCoverageRatio();
			gensOfNoImprov = 0;
		}

		//TODO Needs range set stuff
		if (gensOfNoImprov == 50) {
			rangeSet->adaptRangesBasedOnUsefulness();
		}

		gensOfNoImprov++;
		currentGen++;
	} while (currentGen <= numberOfGenerations && population->getCoverageRatio() < 1);

	return currentGen;
}

void Simulation::testCaseCrossover() {
	if( targetCFG->getNumberOfParameters() <= 1 ) {
		return;
	}

	TestCase *child1 { }, *child2 { };

	auto parent = population->getOrganism(population->fitnessProportionalSelect());
	auto tc1    = parent->getChromosome()->getRandomTestCase();
	auto tc2    = parent->getChromosome()->getRandomTestCase();

	for (int i = 0; i < 100; ++i) {
		do {
			if (tc1 != tc2) {
				break;
			}

			tc1 = parent->getChromosome()->getRandomTestCase();
			tc2 = parent->getChromosome()->getRandomTestCase();
		} while (true);

		population->crossover(*tc1, *tc2, child1, child2, 2);
		tc1->mutate();
		tc2->mutate();

		if( parent->getChromosome()->isCoveringNew(child1) ) {
			delete child2;
			parent->getChromosome()->replaceDuplicateTestCase(child1);
			parent->evaluateBaseFitness();
			population->updatePopulationsFitness();
			break;
		}
		else if( parent->getChromosome()->isCoveringNew(child2) ) {
			delete child1;
			parent->getChromosome()->replaceDuplicateTestCase(child2);
			parent->evaluateBaseFitness();
			population->updatePopulationsFitness();
			break;
		} else {
			delete child1;
			delete child2;
		}
	}
}

void Simulation::testSuiteCrossover(int currentGen) {
	Organism *child1 { }, *child2 { };

	auto parent1Index = population->fitnessProportionalSelect();
	auto parent2Index = population->fitnessProportionalSelect();

	auto parent1 = population->getOrganism(parent1Index);
	auto parent2 = population->getOrganism(parent2Index);

	population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);

	double newProb;
	newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
	child1->mutate(newProb);
	child2->mutate(newProb);

	// Attempt to replace the worst of the two parents
	auto parentToReplace = (parent1 <= parent2 ? parent1Index : parent2Index);

	//TODO when should local opt be done? does it make sense to leave it here?
	if (child1 <= child2) {
		if (currentGen % 10 == 0 || population->getCoverageRatio() > 0.95) {
			tryLocalOptimization (child2);
		}
		child2->evaluateBaseFitness();
		population->replaceParentThenReplaceWorst(parentToReplace, child2);
		delete child1;
		child1 = NULL;
	} else {
		if (currentGen % 10 == 0 || population->getCoverageRatio() > 0.95) {
			tryLocalOptimization (child1);
		}
		child1->evaluateBaseFitness();
		population->replaceParentThenReplaceWorst(parentToReplace, child1);
		delete child2;
		child2 = NULL;
	}
}

void Simulation::tryLocalOptimization(Organism* org) {
	auto tc = callRandomLocalOpt(org);

	if( tc ) {
		org->getChromosome()->replaceDuplicateTestCase(tc);
	}
}

TestCase* Simulation::callRandomLocalOpt(Organism* org){
	TestCase* oldTC = org->getChromosome()->getDuplicateTestCase();

	switch (uniformInRange(0, 2)) {
		case 0:
			return localOptFromGivenParams(oldTC);
			break;
		case 1:
			return localOptFromZero(oldTC);
			break;
		case 2:
			return localOptFromZero(oldTC);
			//return localOptFromMiddle(oldTC);
			break;
		default:
			assert(false);
			break;
	}
}

TestCase* Simulation::localOptFromZero (TestCase* oldTC) {
	int NeighborhoodSize { 1 };

	for(int i = 0; i < 1000; ++i) {
		TestCase* tc = new TestCase { };
		tc->setInputParameters(oldTC->getInputParameters());

		for (int j = 0; j < 150; ++j) { //Try searching this particular neighborhood X times
			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {

				// set the value to something in the neighborhood or move it closer to zero
				if( uniformInRange(1, 100) >= 5 ) {
					tc->setInputParameter(var, uniformInRange(-NeighborhoodSize, NeighborhoodSize));
				} else {
					tc->setInputParameter(var, tc->getParameter(var) / (2 * (j + 1)));
				}
			}
			targetCFG->setCoverageOfTestCase(tc);

			if( population->isCoveringNew(tc) ) {
				return tc;
			}
		}
		delete tc;
		NeighborhoodSize += 5;
	}

	return NULL;
}

TestCase* Simulation::localOptFromGivenParams (TestCase* oldTC)  {
	TestCase* tc = new TestCase { };
	int NeighborhoodSize { 1 };

	tc->setInputParameters(oldTC->getInputParameters());

	for(int i = 0; i < 500; ++i) {

		for (int j = 0; j < (15 + NeighborhoodSize/15); ++j) {
			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
				long newValue = oldTC->getParameter(var) + uniformInRange(-NeighborhoodSize, NeighborhoodSize);

				if( newValue > targetCFG->getUpperBoundForParameter(var) ) {
					newValue = targetCFG->getUpperBoundForParameter(var);
				}
				if( newValue < targetCFG->getLowerBoundForParameter(var) ) {
					newValue = targetCFG->getLowerBoundForParameter(var);
				}

				tc->setInputParameter(var, newValue);
			}
			targetCFG->setCoverageOfTestCase(tc);

			if( population->isCoveringNew(tc) ) {
				return tc;
			}
		}
		NeighborhoodSize += 20;
	}

	return NULL;
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
			int* testSuitePreds = finalOrg->getChromosome()->getPredicateCoverageCounts();
			if (populationPredicates[predicateNum] > 0 && testSuitePreds[predicateNum] == 0) {
				for (int j = 0; j < populationSize; j++) {
					TestCase* missingTestCase = population->getOrganism(j)->getChromosome()->getTestCaseThatCoversPredicate(predicateNum);
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
			int* testSuiteEdges = finalOrg->getChromosome()->getEdgeCoverageCounts();
			if (populationEdges[edgeNum] > 0 && testSuiteEdges[edgeNum] == 0) {
				for (int j = 0; j < populationSize; j++) {
					TestCase* missingTestCase = population->getOrganism(j)->getChromosome()->getTestCaseThatCoversEdge(edgeNum);
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

void Simulation::findPromisingRangesAndCreateTheGlobalRangeSet() {

	int edgesPlusPreds = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	rangeSet = new RangeSet(0, edgesPlusPreds);

	int startSize = 1000, currStart = -500;

	Organism* finalOrg = new Organism(0, edgesPlusPreds, new TestCase*[edgesPlusPreds] { });
	TestSuite* finalSuite = finalOrg->getChromosome();

	TestCase* tc1 = rangeSet->getNewTestCaseEntirelyFromRange(currStart, currStart + startSize);
	finalSuite->addTestCase(tc1);
	finalSuite->calculateTestSuiteCoverage();

	Range* startingRange = new Range(currStart, currStart + startSize);
	rangeSet->addRange(startingRange);

	//TODO: Tune this
	for (int tryNum = 1; tryNum <= 1; tryNum++) {
		int size = startSize * tryNum;
		int nextStartPos = currStart;
		int nextStartNeg = currStart;
		int totalIterations = 0;

		cout << "Starting tryNum # " << tryNum << " : Range Size: " << size << endl;
		while(nextStartPos < numeric_limits<int>::max() - size && nextStartNeg > numeric_limits<int>::min() + size) {

			nextStartPos = nextStartPos + size + 1;
			TestCase* tcPos = rangeSet->getNewTestCaseEntirelyFromRange(nextStartPos, nextStartPos + size);

			if (finalSuite->isCoveringNew(tcPos)) {
				finalSuite->addTestCase(tcPos);
				finalSuite->calculateTestSuiteCoverage();
				finalSuite->printTestSuiteCoverageRatio();

				Range* newRange = new Range(nextStartPos, nextStartPos + size);
				rangeSet->addRange(newRange);
			}
			else {
				delete tcPos;
			}

			nextStartNeg = nextStartNeg - size - 1;
			TestCase* tcNeg = rangeSet->getNewTestCaseEntirelyFromRange(nextStartNeg, nextStartNeg + size);
			targetCFG->setCoverageOfTestCase(tcNeg);
			if (finalSuite->isCoveringNew(tcNeg)) {
				finalSuite->addTestCase(tcNeg);
				finalSuite->calculateTestSuiteCoverage();
				finalSuite->printTestSuiteCoverageRatio();

				Range* newRange = new Range(nextStartNeg, nextStartNeg + size);
				rangeSet->addRange(newRange);
			}
			else {
				delete tcNeg;
			}
			totalIterations++;

			if (finalSuite->getCoverageRatio() == 1) {
				break;
			}
		}

		cout << "End of try # " << tryNum << endl;
		rangeSet->printRangesSimple();
	}
	finalOrg->evaluateBaseFitness();
	finalOrg->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	delete finalOrg;
}

//============================ OLD FUNCTIONS =======================//

//TODO while this was cool and would have been helpful, with us running out of time I may just remove this.
//void getUserInput() {
//	const int GEN_AND_RATIO = 0, PRINT_FITNESS = 1, PRINT_FITNESS_ONCE = 2,
//			COV_RATIO_TS_CROSSOVER = 3, COV_RATIO_TS_CROSSOVER_ONCE = 4,
//			REPLACEMENT = 5, CHANGE_LOCAL_OPT = 6, PAUSE = -1, EXIT = -5;
//	do {
//		int choice;
//		std::cin >> choice;
//		cout << "\t\tYour choice: " << choice << endl;
//		if(choice == EXIT){
//			break;
//		}
//
//		switch (choice) {
//			case GEN_AND_RATIO:
//				printGenerationAndRatio = !printGenerationAndRatio;
//				break;
//			case PRINT_FITNESS:
//				printPopFitness = !printPopFitness;
//				break;
//			case PRINT_FITNESS_ONCE:
//				printPopFitnessOnce = !printPopFitnessOnce;
//				break;
//			case COV_RATIO_TS_CROSSOVER:
//				printCoverageRatioForTScrossover = !printCoverageRatioForTScrossover;
//				break;
//			case COV_RATIO_TS_CROSSOVER_ONCE:
//				printCoverageRatioForTScrossoverOnce = !printCoverageRatioForTScrossoverOnce;
//				break;
//			case REPLACEMENT:
//				printReplacement = !printReplacement;
//				break;
//			case CHANGE_LOCAL_OPT:
//				changeLocalOpt = !changeLocalOpt;
//				break;
//			case PAUSE:
//				pause = !pause;
//				break;
//			default:
//				break;
//		}
//	} while(true);
//}

//void Simulation::runWithFlags(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
//	int i { 0 };
//	Organism *child1 { }, *child2 { };
//
//	//std::thread t(getUserInput);
//
//
//	do{
//		while(pause){
//
//		}
//
//		//population->printPopulationFitness();
//		//population->printPopulationCoverage();
//		auto parent1Index = population->fitnessProportionalSelect();
//		auto parent2Index = population->fitnessProportionalSelect();
//		auto parent1 = population->getOrganism(parent1Index);
//		auto parent2 = population->getOrganism(parent2Index);
//
//		if( printGenerationAndRatio )
//			cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;
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
//
//			if( changeLocalOpt && ( i % 5 == 0 || population->getCoverageRatio() > 0.95 ) ) {
//				tryLocalOptimization(child2);
//			}
//
//			population->replaceParentThenReplaceWorst(parentToReplace, child2);
//			delete child1;
//			child1 = NULL;
//		}
//		else {
//
//			if(  changeLocalOpt && ( i % 5 == 0 || population->getCoverageRatio() > 0.95) ) {
//				tryLocalOptimization(child1);
//			}
//
//			population->replaceParentThenReplaceWorst(parentToReplace, child1);
//			delete child2;
//			child2 = NULL;
//		}
//
//		i++;
//
//	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);
//	//t.join();
//	population->getBestOrganism()->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
//	population->printPopulationFitness();
//	population->printPopulationCoverage();
//
//	//Organism* final = constructFinalOrganism();
//}

//void Simulation::runWithTournamentSelectAndCrossoverWithDominance(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
//	int i { 0 };
//	Organism *child1 { };
//
//	do{
//		auto parent1Index = population->tournamentSelect();
//		auto parent2Index = population->tournamentSelect();
//		auto parent1 = population->getOrganism(parent1Index);
//		auto parent2 = population->getOrganism(parent2Index);
//
//		cout << "Generation # " << i << " CoverageRatio: " << population->getCoverageRatio() << endl;
//
//		population->crossoverWithDominance(*parent1, *parent2, child1);
//
//		double newProb;
//		newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
//		child1->mutate(newProb);
//
//		// Attempt to replace the worst of the two parents
//		auto parentToReplace = ( parent1 <= parent2 ? parent1Index : parent2Index );
//
//		if( i % 25 == 0 || population->getCoverageRatio() > 0.95 ) {
//			tryLocalOptimization(child1);
//		}
//		population->replaceParentThenReplaceWorst(parentToReplace, child1);
//
//		i++;
//
//	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);
//
//	population->getBestOrganism()->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
//	population->printPopulationFitness();
//	population->printPopulationCoverage();
//
//	Organism* final = constructFinalOrganism();
//}

//TestCase* Simulation::localOptFromMiddle (TestCase* oldTC)  {
//	TestCase* tc = new TestCase { };
//	int* parameters = new int[targetCFG->getNumberOfParameters()] { };
//	int* middles = new int[targetCFG->getNumberOfParameters()] { };
//	int NeighborhoodSize { 1 };
//
//	tc->setInputParameters(parameters);
//
//	for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
//		int middle { (targetCFG->getLowerBoundForParameter(var)
//				+ targetCFG->getUpperBoundForParameter(var)) / 2 };
//		middles[var] = middle;
//	}
//
//	for(int i = 0; i < 100; ++i) {
//		for (int j = 0; j < (10 + NeighborhoodSize); ++j) {
//			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
//				parameters[var] = middles[var] + uniformInRange(-NeighborhoodSize, NeighborhoodSize);
//			}
//			targetCFG->setCoverageOfTestCase(tc);
//
//			if( population->isCoveringNew(tc) ) {
//				cout << "Took about " << i * 10 << " tries. " << endl;
//				return tc;
//			}
//		}
//
//		NeighborhoodSize += 5;
//	}
//
//	cout << "Failed to cover edge " << endl;
//	return NULL;
//}
