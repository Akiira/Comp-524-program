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

	Simulation(int populationSize, int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations);
	Simulation(int populationSize);

	void run(int numberOfGenerations, int numberOfCutPoints, double mutationProb);

	void tryLocalOptimization(Organism* child);
	TestCase* callRandomLocalOpt(Organism* child);

	bool coveredAnyNew(bool* uncovered, bool* covered, bool edge);
	bool coveredAnyNewForPopulation(bool* uncovered, bool edge) ;
	void tryLocalOptimization();
	TestCase* callRandomLocalOpt();
	TestCase* localOptFromZero (bool edgeOrPredicate, TestCase* oldTC);
	TestCase* localOptFromGivenParams (bool edgeOrPredicate, TestCase* oldTC);
	TestCase* localOptFromMiddle (int thingToCover, bool edgeOrPredicate);


private:
	Population* population;

	int populationSize;

	double adaptMutationBasedOnOrganismsCoverage(Organism* org);
	double adaptMutationBasedOnCoverageRatio(double pM);

	Organism* constructFinalOrganism();
	bool hasEquivalentCoverageToPopulation(Organism* organism);
	void minimizeOrganism(Organism* orgToMinimize);

};
#endif // !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
