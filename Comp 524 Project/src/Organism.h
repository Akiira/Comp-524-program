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

	Organism(int numOfTestCases);
	Organism(int numOfTestCases, TestCase** testCases);

	void initializeRandomChromosome(int numberOfTestCases);
	void initializeChromosomeFromTestCases(int numberOfTestCases, TestCase** testCases);

	TestSuite* getChromosome() const;
	void mutate(double mutationProb);

	void setFitness();
	int getFitness() const;
	int getNumberOfTestCases() const;

	bool operator<=(const Organism& right);
	bool operator==(const Organism& right);
	Organism& operator=(const Organism& org);

	void printSimple();
	void printFitnessAndCoverage();

	friend class Population;

private:
	TestSuite* chromosome;
	bool initialized, evaluated;
	int fitness;
};
#endif // !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
