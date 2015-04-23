///////////////////////////////////////////////////////////
//  Organism.h
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
#define EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_

#include <cassert>
#include "TestSuite.h"

class ControlFlowGraph;

class Organism
{

public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//

	virtual ~Organism();

	Organism(const Organism& org);
	Organism(int numOfTestCases, int maxNumberOfTestCases);
	Organism(int numOfTestCases, int maxNumberOfTestCases, TestCase** testCases);
	Organism(int numOfTestCases, int maxNumberOfTestCases, int rangeNum);
	Organism(int numberOfTestCases, int maxNumberOfTestCases, Range *range);

	//========================== MUTATOR FUNCTIONS =================================//

	void mutate(double mutationProb);
	void evaluateBaseFitness();

	void setFitness(int fitness) {
		this->fitness = fitness;
		this->scaledFitness = fitness;
	}

	void setScaledFitness(int scaledFitness) {
		assert(scaledFitness >= 0);
		this->scaledFitness = scaledFitness;
	}

	//========================== GETTER FUNCTIONS ==================================//

	TestSuite* getChromosome() const;
	int getUncoveredEdge() const;
	int getUncoveredPredicate() const;
	int getFitness() const;
	int getNumberOfTestCases() const;
	int getMaxNumberOfTestCases() const;

	int getScaledFitness() const {
		return scaledFitness;
	}

	//========================== PRINT FUNCTIONS ===================================//

	void printAll();
	void printFitnessAndTestSuiteCoverage() const;
	void printFitnessAndTestSuiteCoverageAndTestCaseInputs() const;

	//========================== OVERLOADED OPERATORS ==============================//

	bool operator<=(const Organism& right);
	bool operator<(const Organism& right);
	bool operator==(const Organism* right);
	Organism& operator=(const Organism& org);

	friend class Population;

private:
	TestSuite* chromosome;
	int fitness, scaledFitness;

	int fitnessFunction01();
	int fitnessFunction02();
	int fitnessFunction03();
};
#endif // !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
