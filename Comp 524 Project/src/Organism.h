///////////////////////////////////////////////////////////
//  Organism.h
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
#define EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_

#include "TestSuite.h"

class ControlFlowGraph;

class Organism
{

public:
	virtual ~Organism();

	Organism(ControlFlowGraph& target);

	void initializeRandomChromosome();
	void initializeChromosomeFromTestCases(int numberOfTestCases, TestCase** testCases);

	TestSuite* getChromosome() const;
	void mutate(double mutationProb);

	void setFitness();
	int getFitness() const;

	bool operator<=(const Organism& right);
	bool operator==(const Organism& right);
	Organism& operator=(const Organism& org);

	void print();

	friend class Population;

private:
	TestSuite* chromosome;
	ControlFlowGraph* targetCFG;
	// Initialized means it has a full test suite
	// Evaluated means it's fitness is up to date
	bool initialized, evaluated;
	int fitness;

};
#endif // !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
