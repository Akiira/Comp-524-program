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
	void replaceOrganismAtIndexWithChild(int organismToReplace, Organism* child);

	void evaluateOrganismsFitness(Organism* org);
	void updatePopulationsFitness();
	int fitnessProportionalSelect();

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

	double getCoverageRatio() const {
		return coverageRatio;
	}



private:
	Organism** population;
	int populationSize;
	int totalFitness;
	int* edgesCovered;
	int* predicatesCovered;
	double coverageRatio;
	int lastReplacedFitness;

	//TODO remove hard coding of value
	const static typeOfScaling SCALING = LINEAR;

	int* selectCutPoints(int numberOfCutPoints, int upperBound);
	void computePopulationLevelCoverage();


	void linearScaling();

	void updateCoverageBeforeReplacement(int organismToBeReplaced, Organism* child);

	void sortPopulationByFitness();
	void moveOrganismToSortedPosition(int indexToSort);


};
#endif // !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
