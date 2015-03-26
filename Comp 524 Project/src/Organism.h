///////////////////////////////////////////////////////////
//  Organism.h
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
#define EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_

#include "TestSuite.h"

class ControlFlowGraph;

class Organism
{

public:
	virtual ~Organism();

	Organism(const Organism& org);
	Organism(int numOfTestCases, int maxNumberOfTestCases);
	Organism(int numOfTestCases, int maxNumberOfTestCases, TestCase** testCases);

	TestSuite* getChromosome() const;
	void mutate(double mutationProb);

	void evaluateBaseFitness();

	int getUncoveredEdge() const;
	int getUncoveredPredicate() const;
	int getFitness() const;
	int getNumberOfTestCases() const;
	int getMaxNumberOfTestCases() const;

	bool operator<=(const Organism& right);
	bool operator<(const Organism& right);
	bool operator==(const Organism* right);
	Organism& operator=(const Organism& org);

	void printAll();
	void printFitnessAndTestSuiteCoverage();
	void printFitnessAndTestSuiteCoverageAndTestCaseInputs();

	void setFitness(int fitness) {
		this->fitness = fitness;
	}

	int getScaledFitness() const {
		return scaledFitness;
	}

	void setScaledFitness(int scaledFitness) {
		this->scaledFitness = scaledFitness;
	}

	friend class Population;

private:
	TestSuite* chromosome;
	int fitness;
	int scaledFitness;

	int fitnessFunction01();
	int fitnessFunction02();
	int fitnessFunction03();
};
#endif // !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
