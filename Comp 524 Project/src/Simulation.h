///////////////////////////////////////////////////////////
//  Simulation.h
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_)
#define EA_63CB5297_1E19_44f6_AF1B_16CF01DC9D73__INCLUDED_

class Population;
class Organism;
class TestCase;

class Simulation
{
public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//

	virtual ~Simulation();

	Simulation(int populationSize, int numberOfCutPoints, double mutationProb);
	Simulation(int populationSize, int numberOfCutPoints): Simulation(populationSize, numberOfCutPoints, 0.02){};
	Simulation(int populationSize, double mutationProb): Simulation(populationSize, 2, mutationProb){};
	Simulation(int populationSize): Simulation(populationSize, 2, 0.02){};


	void run(int numberOfGenerations, int numberOfCutPoints, double mutationProb);
	void TestCaseCrossover();
	void TestSuiteCrossover(int currentGen);

	void tryLocalOptimization(Organism* org);
	TestCase* callRandomLocalOpt(Organism* org);
	TestCase* localOptFromZero (TestCase* oldTC);
	TestCase* localOptFromGivenParams (TestCase* oldTC);


	void run(int numberOfGenerations) {
		this->run(numberOfGenerations, this->numberOfCutPoints, this->mutationProb);
	}

	void run(int numberOfGenerations, int numberOfCutPoints) {
		this->run(numberOfGenerations, numberOfCutPoints, this->mutationProb);
	}

	void run(int numberOfGenerations, double mutationProb) {
		this->run(numberOfGenerations, this->numberOfCutPoints, mutationProb);
	}

	//============================ OLD FUNCTIONS =======================//

	TestCase* localOptFromMiddle (TestCase* oldTC);
	void runWithTournamentSelectAndCrossoverWithDominance(int numberOfGenerations, int numberOfCutPoints, double mutationProb);
	void runWithFlags(int numberOfGenerations, int numberOfCutPoints, double mutationProb);

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
