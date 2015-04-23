///////////////////////////////////////////////////////////
//  Population.h
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
#define EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_

class Organism;
class TestCase;
#include "GlobalVariables.h"

class Population
{
public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//

	virtual ~Population();

	Population(int popSize);
	Population(int popSize, int initialTestSuiteSize, int maxTestSuiteSize);

	//========================== CORE FUNCTIONS ====================================//

	void crossover(const Organism& parent1, const Organism& parent2,
			Organism*& offspring1, Organism*& offspring2, int numberOfCutPoints);
	void crossover(const TestCase& parent1, const TestCase& parent2,
			TestCase*& child1, TestCase*& child2, int numberOfCutPoints);

	void replaceParentWithUnion(int parentIndex, Organism* child);
	void replaceParentThenReplaceWorst(int parentIndex, Organism* child);
	void replaceWorst(Organism* offspring);
	void replaceOrganism(int organismToReplace, Organism* child);


	void updatePopulationsFitness();
	int fitnessProportionalSelect() const;

	void printPopulationFitness();
	void printPopulationCoverage();

	//========================== GETTER FUNCTIONS===================================//


	int* getEdgesCovered() const {
		return edgesCovered;
	}

	int* getPredicatesCovered() const {
		return predicatesCovered;
	}

	Organism* getBestOrganism() const {
		return population[0];
	}

	Organism* getOrganism(int index) const {
		return population[index];
	}

	int getTotalFitness() const {
		return totalFitness;
	}

	double getCoverageRatio() const {
		return coverageRatio;
	}


	int getTestSuiteSize() const {
		return testSuiteSize;
	}

	int getPopulationSize() const {
		return populationSize;
	}

	//========================== PREDICATE FUNCTIONS================================//


	bool isCoveringNew(const TestCase * tc) const;
	bool isCoveringNewEdge(const bool * coverage) const;
	bool isCoveringNewPred(const bool * coverage) const;

	//========================== PRINT FUNCTIONS====================================//

	void printPopulationFitness() const;
	void printPopulationCoverage() const;

	//========================== OLD FUNCTIONS =====================================//

	// Deleted because it was confusing me.

private:
	Organism** population;
	int populationSize;
	int testSuiteSize;
	long long totalFitness;

	int* edgesCovered;
	int* predicatesCovered;
	double coverageRatio;
	int lastReplacedFitness;

	//TODO remove hard coding of value
	const static typeOfScaling SCALING = SHARING;

	int* selectCutPoints(int& numberOfCutPoints, int upperBound);

	void updateCoverage();

	void evaluateSharedFitness(Organism* org);

	void linearScaling();

	void sortPopulationByFitness();
};
#endif // !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
