///////////////////////////////////////////////////////////
//  Population.h
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
#define EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_

#include "Organism.h"
#include "ControlFlowGraph.h"

class Population
{

public:
	virtual ~Population();

	Population(int popSize, const ControlFlowGraph& targetCFG);
	void crossover(Organism* offspring1, Organism* offspring2);
	void replace(Organism* offspring);
	Organism* select();

private:
	Organism** population;
	int totalFitness;

	void initializeStartingPopulation();
	void setPopulationFitness();

};
#endif // !defined(EA_BE7C8399_4BC5_48f6_93FA_2B1C440AB247__INCLUDED_)
