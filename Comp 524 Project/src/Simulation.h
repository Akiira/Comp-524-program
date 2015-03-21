///////////////////////////////////////////////////////////
//  Simulation.h
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
#define EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_

#include "TestSuite.h"
#include "GlobalVariables.h"
#include "Population.h"

class Simulation
{
public:
	virtual ~Simulation();

	Simulation(int populationSize, int initalTestSuiteSize, int maxTestSuiteSize,
			int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations);
	void run();
	void tryLocalOptimization();
	TestCase* callRandomLocalOpt();
	TestCase* localOptFromZero(int thingToCover, bool edgeOrPredicate);
	TestCase* localOptFromGivenParams (TestCase* orig, int thingToCover, bool edgeOrPredicate);
	TestCase* localOptFromMiddle (int thingToCover, bool edgeOrPredicate);

	TestSuite* getBestTestSuite(){
		return bestOrganismSeen->getChromosome();
	}

private:
	Population* population;
	Organism* bestOrganismSeen;

	int populationSize;
	int initialTestSuiteSize;
	int maxTestSuiteSize;

	//TODO: I was think we could maybe just pass these into the run function since they are never
	// 		used anywhere else.
	int numberOfCutPoints;
	int numberOfGenerations;
	double probabilityForCrossover;
	double probabilityForMutation;

};
#endif // !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
