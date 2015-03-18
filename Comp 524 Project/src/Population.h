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
#include "ControlFlowGraph.h"

class Population
{

public:
	virtual ~Population();

	Population(int popSize, int initialTestSuiteSize, int maxTestSuiteSize);
	void crossover(const Organism& parent1, const Organism& parent2, Organism*& offspring1, Organism*& offspring2, int numberOfCutPoints);
	void crossover(const TestCase& parent1, const TestCase& parent2,
			TestCase*& child1, TestCase*& child2, int numberOfCutPoints);
	void replaceParentWithChild(Organism* parent, Organism* child);
	void replaceWorst(Organism* offspring);
	Organism* randomSelect();
	Organism* select();
	Organism* fitnessProportionalSelect();
	void scalePopulationsFitness();
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

private:
	Organism** population;
	int populationSize;
	int totalFitness;
	int* edgesCovered;
	int* predicatesCovered;

	void setPopulationFitness();
	int* selectCutPoints(int numberOfCutPoints, int upperBound);
	void computeCoverage();

	// Clean up some duplicate code, will also be required if we ever add other replacement schemes.
	void updateCoverageBeforeReplacement(int organismToBeReplaced, Organism* child);

	//TODO remove hard coding of value
	const static typeOfScaling SCALING = NONE;

};
#endif // !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
