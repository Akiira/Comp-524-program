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
	TestSuite* getChromosome();
	void mutate();

	void setFitness(const ControlFlowGraph& target);
	int getFitness();

private:
	TestSuite* chromosome;
	int fitness;

};
#endif // !defined(EA_FD308F31_83AB_4efc_8F0F_05730A9FF804__INCLUDED_)
