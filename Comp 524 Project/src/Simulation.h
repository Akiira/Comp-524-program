///////////////////////////////////////////////////////////
//  Simulation.h
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
#define EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_

#include "TestSuite.h"
#include "Population.h"
#include "ControlFlowGraph.h"

class Simulation
{

public:
	virtual ~Simulation();

	Simulation(ControlFlowGraph& targetBranchCFG, int populationSize, double mutationProb,
			double crossOverProb, int numberOfGenerations);

	int evaluateFitness(Organism* organism);
	TestSuite* getBestTestSuite();
	void run();

private:
	Population* population;
	Organism* bestOrganismSeen;
	const ControlFlowGraph* targetCFG;

	int numberOfGenerations;
	int populationSize;
	double probabilityForCrossover;
	double probabilityForMutation;

};
#endif // !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
