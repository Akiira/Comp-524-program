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
		if (currentGen % 100 == 0 ) {
			cout << "\t\tGeneration # " << currentGen << " CoverageRatio: " << population->getCoverageRatio() << endl;
		}

		auto parent = population->getOrganism(population->fitnessProportionalSelect());

		testCaseCrossoverAndMutation(parent);

		if( currentGen % 10 == 0 ) {
			tryLocalOptimization (parent);
		}

		population->updatePopulationsFitness();

		if( lastGensCoverage < population->getCoverageRatio() ) {
			lastGensCoverage = population->getCoverageRatio();
			gensOfNoImprov = 0;
		}

		if (gensOfNoImprov >= 20 && currentGen % 100 == 0) {
			rangeSet->adaptRangesBasedOnUsefulness();
		}

		if( gensOfNoImprov >= 20 && currentGen % 30 == 0 ){
			testSuiteCrossoverAndMutation();
		}

		gensOfNoImprov++;
		currentGen++;
	} while (currentGen <= numberOfGenerations && population->getCoverageRatio() < 1 && gensOfNoImprov < 500);
	//rangeSet->getFinalTestSuite()->printAll();
	//cout <<  rangeSet->getTestCaseSourceReport() << endl;
	return currentGen;
}

int Simulation::runWithoutGA(int numberOfGenerations) {
	int currentGen { 0 };

	double lastGensCoverage = population->getCoverageRatio();
	int gensOfNoImprov = 0;

	do {
		if (currentGen % 100 == 0 ) {
			cout << "\tGeneration # " << currentGen << " CoverageRatio: " << population->getCoverageRatio() << endl;
		}

		auto parent = population->getOrganism(population->fitnessProportionalSelect());

		if( currentGen % 10 == 0 ) {
			tryLocalOptimization (parent);
		}

		population->updatePopulationsFitness();

		if( lastGensCoverage < population->getCoverageRatio() ) {
			lastGensCoverage = population->getCoverageRatio();
			gensOfNoImprov = 0;
		}

		gensOfNoImprov++;
		currentGen++;
	} while (currentGen <= numberOfGenerations && population->getCoverageRatio() < 1 && gensOfNoImprov < 500);

	return currentGen;
}

int Simulation::runWithoutLocalOpt(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
	this->numberOfCutPoints = numberOfCutPoints;
	this->mutationProb      = mutationProb;
	int currentGen { 0 };

	double lastGensCoverage = population->getCoverageRatio();
	int gensOfNoImprov = 0;

	do {
		if (currentGen % 100 == 0 ) {
			cout << "\t\tGeneration # " << currentGen << " CoverageRatio: " << population->getCoverageRatio() << endl;
		}

		if( gensOfNoImprov == 20 || currentGen % 30 == 0 ){
			testSuiteCrossoverAndMutation();
		}

		auto parent = population->getOrganism(population->fitnessProportionalSelect());

		testCaseCrossoverAndMutation(parent);

		population->updatePopulationsFitness();

		if( lastGensCoverage < population->getCoverageRatio() ) {
			lastGensCoverage = population->getCoverageRatio();
			gensOfNoImprov = 0;
		}

		if (gensOfNoImprov == 30 || currentGen % 100 == 1) {
			rangeSet->adaptRangesBasedOnUsefulness();
		}

		gensOfNoImprov++;
		currentGen++;
	} while (currentGen <= numberOfGenerations && population->getCoverageRatio() < 1 && gensOfNoImprov < 500);
	//cout <<  rangeSet->getFinalTestSuite()->printTestCaseInputsAndCoverage() << endl;
	//cout <<  rangeSet->getTestCaseSourceReport() << endl;
	return currentGen;
}

void Simulation::testCaseCrossoverAndMutation(Organism* parent) {
	if( targetCFG->getNumberOfParameters() <= 1 ) {
		return;
	}

	TestCase *child1, *child2;
	auto tc1    = parent->getChromosome()->getRandomTestCase();
	auto tc2    = parent->getChromosome()->getRandomTestCase();

	bool done = false;

	for (int i = 0; i < 100; ++i) {
		do {
			if (tc1 != tc2) {
				break;
			}

			tc1 = parent->getChromosome()->getRandomTestCase();
			tc2 = parent->getChromosome()->getRandomTestCase();
		} while (true);

		population->crossover(*tc1, *tc2, child1, child2, 2);
		child1->mutate();
		child2->mutate();

		//update coverage incase it changed because of mutation
		targetCFG->setCoverageOfTestCase(child1);
		targetCFG->setCoverageOfTestCase(child2);

		rangeSet->offerToFinalTestSuite(child1,2);
		rangeSet->offerToFinalTestSuite(child2,2);

		if( parent->getChromosome()->isCoveringNew(child1) ) {
			parent->getChromosome()->replaceDuplicateTestCase(child1);
			done = true;
		}
		else if( parent->getChromosome()->isCoveringNew(child2) ) {
			parent->getChromosome()->replaceDuplicateTestCase(child2);
			done = true;
		}

		delete child1;
		delete child2;

		if (done) {
			break;
		}
	}
}

void Simulation::testSuiteCrossoverAndMutation() {
	Organism *child1 { }, *child2 { };

	auto parent1Index = population->fitnessProportionalSelect();
	auto parent2Index = population->fitnessProportionalSelect();

	auto parent1 = population->getOrganism(parent1Index);
	auto parent2 = population->getOrganism(parent2Index);

	population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);

	child1->mutate(mutationProb);
	child2->mutate(mutationProb);

	population->replaceParentWithUnion(parent2Index, child1);
	population->replaceParentWithUnion(parent1Index, child2);
}

void Simulation::tryLocalOptimization(Organism* org) {
	int* testCaseSource = new int;
	*testCaseSource = -1;
	auto tc = callRandomLocalOpt(org, testCaseSource);

	if( tc ) {
		org->getChromosome()->replaceDuplicateTestCase(tc);
		rangeSet->offerToFinalTestSuite(tc, *testCaseSource);
		delete tc;
	}
	delete testCaseSource;
}

TestCase* Simulation::callRandomLocalOpt(Organism* org, int* testCaseSource){
	TestCase* oldTC = org->getChromosome()->getDuplicateTestCase();

	switch (uniformInRange(0, 1)) {
		case 0:
			*testCaseSource = 3;
			return localOptFromGivenParams(oldTC);
			break;
		case 1:
			*testCaseSource = 4;
			return localOptFromZero(oldTC);
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

	delete tc;

	return NULL;
}

void Simulation::findPromisingRangesAndCreateTheGlobalRangeSet() {

	int edgesPlusPreds = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	rangeSet = new RangeSet(0, edgesPlusPreds);

	int startSize = 5000;

	TestSuite* tmpSuite = new TestSuite(0, edgesPlusPreds, new TestCase*[edgesPlusPreds] { });

	for (int tryNum = 1; tryNum <= 2; tryNum++) {
		int size = startSize / tryNum;
		long nextStartPos = -size/2;
		long nextStartNeg = size/2;

		while(nextStartPos + size < numeric_limits<int>::max() && tmpSuite->getCoverageRatio() != 1) {
			nextStartPos += size;

			int tmpSize = size;
			if (nextStartPos + size > numeric_limits<int>::max()) {
				tmpSize = numeric_limits<int>::max() - nextStartPos - 1;
			}

			TestCase* tcPos = rangeSet->getNewTestCaseEntirelyFromRange(nextStartPos, nextStartPos + tmpSize);

			if (tmpSuite->isCoveringNew(tcPos)) {
				tmpSuite->addTestCase(tcPos);
				tmpSuite->calculateTestSuiteCoverage();
				rangeSet->addRange(new Range(nextStartPos, nextStartPos + tmpSize));
			}
			else {
				delete tcPos;
			}

		}
		while(nextStartNeg > numeric_limits<int>::min() && tmpSuite->getCoverageRatio() != 1) {
			nextStartNeg -= size;
			int tmpSize = size;

			if (nextStartNeg < numeric_limits<int>::min()) {
				tmpSize = numeric_limits<int>::min() - nextStartNeg;
				nextStartNeg = numeric_limits<int>::min();
			}

			TestCase* tcNeg = rangeSet->getNewTestCaseEntirelyFromRange(nextStartNeg, nextStartNeg + tmpSize);
			targetCFG->setCoverageOfTestCase(tcNeg);

			if (tmpSuite->isCoveringNew(tcNeg)) {
				tmpSuite->addTestCase(tcNeg);
				tmpSuite->calculateTestSuiteCoverage();
				rangeSet->addRange(new Range(nextStartNeg, nextStartNeg + tmpSize));
			}
			else {
				delete tcNeg;
			}
		}
	}

	delete tmpSuite;
}

double Simulation::getCoverageRatio() const {
	return population->getCoverageRatio();
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
