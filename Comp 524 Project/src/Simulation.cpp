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
}

Simulation::Simulation(int populationSize) {

	this->populationSize = populationSize;
	this->testSuiteSize = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();

	population = new Population { populationSize, testSuiteSize, testSuiteSize };
}

void Simulation::run(int numberOfGenerations, int numberOfCutPoints, double mutationProb) {
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

		double newProb;
		newProb = adaptMutationBasedOnCoverageRatio(mutationProb);
		child1->mutate(newProb);
		child2->mutate(newProb);

		// Attempt to replace the worst of the two parents
		auto parentToReplace = ( parent1 <= parent2 ? parent1Index : parent2Index );

		if(child1 <= child2){
			population->evaluateOrganismsFitness(child2);
			tryLocalOptimization(child2);
			population->replaceParentThenReplaceWorst(parentToReplace, child2);
			delete child1;
			child1 = NULL;
		}
		else {
			population->evaluateOrganismsFitness(child2);
			tryLocalOptimization(child1);
			population->replaceParentThenReplaceWorst(parentToReplace, child1);
			delete child2;
			child2 = NULL;
		}

//		if( i % 100 == 0 || population->getCoverageRatio() > 0.95 ) {
//			tryLocalOptimization();
//		}

		i++;

	}while(i < numberOfGenerations && population->getCoverageRatio() < 1);

	population->getBestOrganism()->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	population->printPopulationFitness();
	population->printPopulationCoverage();

	Organism* final = constructFinalOrganism();
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

Organism* Simulation::constructFinalOrganism() {
	Organism* finalOrg;

	if (hasEquivalentCoverageToPopulation(population->getBestOrganism())) {
		cout << "Already has all the coverage" << endl;
		finalOrg = new Organism(*population->getBestOrganism());
	}
	else {
		cout << "Doesn't have all the coverage" << endl;
		// Start with a copy of bestOrganismSeen, with room to grow
		TestCase** bestTestCasesSeen = population->getBestOrganism()->getChromosome()->getAllTestCases();
		finalOrg = new Organism(0, testSuiteSize * 2, new TestCase*[testSuiteSize * 2] { });

		TestSuite* finalSuite = finalOrg->getChromosome();
		for (int i = 0; i < testSuiteSize; i++) {
			finalSuite->addTestCase(new TestCase { *bestTestCasesSeen[i] });
		}
		finalSuite->calculateTestSuiteCoverage();

		int predicateNum = -1;
		while((predicateNum = finalOrg->getUncoveredPredicate()) != -1) {
			for (int j = 0; j < populationSize; j++) {
				TestCase* missingTestCase = population->getOrganismByIndex(j)->getChromosome()->getTestCaseThatCoversPredicate(predicateNum);
				if (missingTestCase != NULL) {
					finalSuite->addTestCase(new TestCase(*missingTestCase));
					finalSuite->calculateTestSuiteCoverage();
					break; // Done with this predicate, find the next
				}
			}
		}

		// I don't believe this is actually necessary since hitting all the predicates should always imply hitting all the edges,
		//	but I included it anyway just in case.
		int edgeNum = -1;
		while((edgeNum = finalOrg->getUncoveredEdge()) != -1) {
			for (int j = 0; j < populationSize; j++) {
				TestCase* missingTestCase = population->getOrganismByIndex(j)->getChromosome()->getTestCaseThatCoversEdge(edgeNum);
				if (missingTestCase != NULL) {
					finalSuite->addTestCase(new TestCase(*missingTestCase));
					finalSuite->calculateTestSuiteCoverage();
					break; // Done with this edge, find the next
				}
			}
		}

		assert(hasEquivalentCoverageToPopulation(finalOrg));
	}

	minimizeOrganism(finalOrg);
	finalOrg->printFitnessAndTestSuiteCoverageAndTestCaseInputs();
	return finalOrg;
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

void Simulation::tryLocalOptimization(Organism* child) {
	cout << "\tFitness before1: " << child->getFitness() << endl;
	auto tc = callRandomLocalOpt(child);

	if( tc ) {
		tc->printInputsAndCoverage();
		cout << "---------------------------------------------" << endl;
		cout << "\tFitness before2: " << child->getFitness() << "\n";
		child->printFitnessAndTestSuiteCoverage();
		cout << "---------------------------------------------" << endl;
		child->getChromosome()->replaceDuplicateTestCase(tc);
		child->getChromosome()->calculateTestSuiteCoverage();
		child->printFitnessAndTestSuiteCoverage();
		child->evaluateBaseFitness();
		cout << "\tFitness before3: " << child->getFitness() << "\n";
		population->evaluateOrganismsFitness(child);
		cout << "\tFitness after: " << child->getFitness() << "\n";
		population->updatePopulationsFitness();
	}
}

TestCase* Simulation::callRandomLocalOpt(Organism* child){
	TestCase* oldTC = child->getChromosome()->getDuplicateTestCase();
	bool edgeOrPredicate { true };
	int uncovered = child->getUncoveredEdge();
	bool* allUncovered = NULL;

	if( child->getUncoveredEdge() == -1 ) {
		edgeOrPredicate = false;
		uncovered = child->getUncoveredPredicate();
		allUncovered = child->getChromosome()->getAllUncoveredPredicates();
	} else {
		allUncovered = child->getChromosome()->getAllUncoveredEdges();
	}
//	cout << "\n";
//	for (int i = 0; i < targetCFG->getNumberOfEdges(); ++i) {
//		cout << "\t" << allUncovered[i] << "\t" << oldTC->getEdgesCovered()[i] << "\n";
//	}

	switch (uniformInRange(0, 2)) {
		case 0:
			return localOptFromZero(allUncovered, edgeOrPredicate, oldTC);
			//return localOptFromGivenParams(oldTC, uncovered, edgeOrPredicate);
			break;
		case 1:
			return localOptFromZero(allUncovered, edgeOrPredicate, oldTC);
			break;
		case 2:
			return localOptFromZero(allUncovered, edgeOrPredicate, oldTC);
			//return localOptFromMiddle(uncovered, edgeOrPredicate);
			break;
		default:
			assert(false);
			break;
	}
}

TestCase* Simulation::localOptFromZero (bool* uncovered, bool edgeOrPredicate, TestCase* oldTC) {
	int NeighborhoodSize { 1 };
	int startingProbability = 30;
	//oldTC->printInputsAndCoverage();
	for(int i = 0; i < 1000; ++i) {
		TestCase* tc = new TestCase { };
		tc->setInputParameters(oldTC->getInputParameters());
		for (int j = 0; j < 200; ++j) { //Try searching this particular neighborhood X times


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
				cout << "\tTook about " << i * 200 << " tries. " << endl;
				return tc;
			}

//			if( coveredAnyNew(uncovered, coverage, edgeOrPredicate) ) {
//				//tc->printInputsAndCoverage();
//
//				if( population->getEdgesCovered() )
//
//				cout << "\tTook about " << i * 50 << " tries. " << endl;
//				return tc;
//			}


		}
		delete tc;
		NeighborhoodSize += 5;

//		if( i % 5 == 0 ) {
//			startingProbability--;
//		}
	}

	cout << "\tFailed to cover edge " << endl;
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

TestCase* Simulation::localOptFromZero (int thingToCover, bool edgeOrPredicate) {
	TestCase* tc = new TestCase { };
	int* parameters = new int[targetCFG->getNumberOfParameters()] { };
	int NeighborhoodSize { 1 };

	tc->setInputParametersWithReference(&parameters);

	for(int i = 0; i < 100; ++i) {

		for (int j = 0; j < 10; ++j) {
			for (int var = 0; var < targetCFG->getNumberOfParameters(); ++var) {
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
