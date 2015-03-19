///////////////////////////////////////////////////////////
//  Population.h
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
#define EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_

#include "Organism.h"
#include "GlobalVariables.h"

class Population
{

public:
	virtual ~Population();

	Population(int popSize, int initialTestSuiteSize, int maxTestSuiteSize);
	void crossover(const Organism& parent1, const Organism& parent2, Organism*& offspring1, Organism*& offspring2, int numberOfCutPoints);
	void crossover(const TestCase& parent1, const TestCase& parent2,
			TestCase*& child1, TestCase*& child2, int numberOfCutPoints);

	void replaceParentThenReplaceWorst(int parentIndex, Organism* child);
	void replaceWorst(Organism* offspring);

	int fitnessProportionalSelect();
	void scalePopulationsFitness(Organism* newOrganism, int replacedOrganismFitness);
	void printPopulationFitness();

	void printPopulationCoverage();

	int* getEdgesCovered() const {
		return edgesCovered;
	}

	int* getPredicatesCovered() const {
		return predicatesCovered;
	}

	Organism* getBestOrganism() const {
		return population[0];
	}

	Organism* getOrganismByIndex(int index) const {
		return population[index];
	}

	int getTotalFitness() const {
		return totalFitness;
	}

	int getProportionCovered() const {
		return 0;
	}

private:
	Organism** population;
	int populationSize;
	int totalFitness;
	int* edgesCovered;
	int* predicatesCovered;
	int totalProportionCovered;

	int* selectCutPoints(int numberOfCutPoints, int upperBound);
	void computePopulationLevelCoverage();

	// Clean up some duplicate code, will also be required if we ever add other replacement schemes.
	void updateCoverageBeforeReplacement(int organismToBeReplaced, Organism* child);

	void sortPopulationByFitness();
	void moveOrganismToSortedPosition(int indexToSort);

	void replaceOrganismAtIndexWithChild(int organismToReplace, Organism* child);


	//TODO remove hard coding of value
	const static typeOfScaling SCALING = NONE;

};
#endif // !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
