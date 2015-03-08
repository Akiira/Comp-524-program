///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Simulation.h"
#include <iostream>
using std::cout;
using std::endl;

Simulation::~Simulation(){
	delete population;
}

Simulation::Simulation(ControlFlowGraph& targetCFG, int populationSize, int initialTestSuiteSize,
		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations){

	this->targetCFG = &targetCFG;
	this->populationSize = populationSize;
	this->initialTestSuiteSize = initialTestSuiteSize;
	this->numberOfGenerations = numberOfGenerations;
	this->numberOfCutPoints = numberOfCutPoints;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;
	bestOrganismSeen = 0;

	population = new Population { populationSize, initialTestSuiteSize, targetCFG };


	//once the population is initialized each organism needs to be evaluated
	// once each organism is evaluated we can set bestOrganismSeen
}


TestSuite* Simulation::getBestTestSuite(){
	return bestOrganismSeen->getChromosome();
}


void Simulation::run(){
	int i { 0 };

	// Create two shells of organism to use, their chromosomes are not set by this constructor
	Organism child1(*targetCFG), child2(*targetCFG);

	do{
		auto parent1 = population->fitnessProportionalSelect();
		auto parent2 = population->fitnessProportionalSelect();

		cout << "Generation # " << i << endl;
		cout << "Parents" << endl;
		parent1->print();
		parent2->print();

		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);
		child1.mutate(probabilityForMutation);
		child1.setFitness();
		child2.mutate(probabilityForMutation);
		child2.setFitness();

		cout << "Children" << endl;
		child1.print();
		child2.print();

		if(child1 <= child2)
		{
			population->replace(child2);
		}
		else {
			population->replace(child1);
		}



		i++;

	}while(i < numberOfGenerations);
	population->printPopulationFitness();

}












