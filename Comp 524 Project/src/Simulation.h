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

	Simulation(int populationSize, int numberOfCutPoints, double mutationProb);
	Simulation(int populationSize, int numberOfCutPoints): Simulation(populationSize, numberOfCutPoints, 0.02){};
	Simulation(int populationSize, double mutationProb): Simulation(populationSize, 2, mutationProb){};
	Simulation(int populationSize): Simulation(populationSize, 2, 0.02){};


	void run(int numberOfGenerations, int numberOfCutPoints, double mutationProb);
	void TestCaseCrossover();
	void TestSuiteCrossover(int currentGen);

	void runWithTournamentSelectAndCrossoverWithDominance(int numberOfGenerations, int numberOfCutPoints, double mutationProb);
	void runWithFlags(int numberOfGenerations, int numberOfCutPoints, double mutationProb);

	void tryLocalOptimization(Organism* child);
	TestCase* callRandomLocalOpt(Organism* child);

	void tryLocalOptimization();
	TestCase* callRandomLocalOpt();
	TestCase* localOptFromZero (TestCase* oldTC);
	TestCase* localOptFromGivenParams (TestCase* oldTC);
	TestCase* localOptFromMiddle (TestCase* oldTC);

	void run(int numberOfGenerations) {
		this->run(numberOfGenerations, this->numberOfCutPoints, this->mutationProb);
	}

	void run(int numberOfGenerations, int numberOfCutPoints) {
		this->run(numberOfGenerations, numberOfCutPoints, this->mutationProb);
	}

	void run(int numberOfGenerations, double mutationProb) {
		this->run(numberOfGenerations, this->numberOfCutPoints, mutationProb);
	}


private:
	Population* population;

	int populationSize;
	int numberOfCutPoints;
	double mutationProb;

	double adaptMutationBasedOnOrganismsCoverage(Organism* org);
	double adaptMutationBasedOnCoverageRatio(double pM);

	Organism* constructFinalOrganism();
	bool hasEquivalentCoverageToPopulation(Organism* organism);
	void minimizeOrganism(Organism* orgToMinimize);

};
#endif // !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
