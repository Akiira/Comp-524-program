///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Simulation.h"

Simulation::~Simulation(){
	delete population;
}

Simulation::Simulation(ControlFlowGraph& targetCFG, int populationSize, int numberOfCutPoints, double mutationProb,
		double crossOverProb, int numberOfGenerations){

	this->targetCFG = &targetCFG;
	this->populationSize = populationSize;
	this->numberOfGenerations = numberOfGenerations;
	this->numberOfCutPoints = numberOfCutPoints;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;
	bestOrganismSeen = 0;

	population = new Population { populationSize, targetCFG };

	//once the population is initialized each organism needs to be evaluated
	// once each organism is evaluated we can set bestOrganismSeen
}


TestSuite* Simulation::getBestTestSuite(){
	return bestOrganismSeen->getChromosome();
}


void Simulation::run(){
	int i { 0 };
	Organism child1(*targetCFG), child2(*targetCFG);

	do{
		auto parent1 = population->select();
		auto parent2 = population->select();

		population->crossover(*parent1, *parent2, child1, child2);

		child1.mutate(probabilityForMutation);
		child1.setFitness(*targetCFG);
		child2.mutate(probabilityForMutation);
		child2.setFitness(*targetCFG);

		if(child1 <= child2)
			population->replace(child2);
		else
			population->replace(child1);

		i++;

	}while(i < numberOfGenerations);

}












