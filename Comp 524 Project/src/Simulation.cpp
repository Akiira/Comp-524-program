///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Simulation.h"
#include <cassert>

Simulation::~Simulation(){

}

Simulation::Simulation(ControlFlowGraph& targetBranchCFG, int populationSize, double mutationProb,
		double crossOverProb, int numberOfGenerations){

	targetCFG = &targetBranchCFG;
	this->populationSize = populationSize;
	this->numberOfGenerations = numberOfGenerations;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;
	bestOrganismSeen = 0;

	population = new Population(populationSize, targetBranchCFG);

	//once the population is initialized each organism needs to be evaluated
	// once each organism is evaluated we can set bestOrganismSeen
}


int Simulation::evaluateFitness(Organism* organism){

	assert(false);
	return 0;
}


TestSuite* Simulation::getBestTestSuite(){
	return bestOrganismSeen->getChromosome();
}


void Simulation::run(){
	bool done = false;

	do{

	}while(!done);

}
