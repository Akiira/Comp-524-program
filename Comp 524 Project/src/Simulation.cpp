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





Simulation::Simulation(ControlFlowGraph* targetBranchCFG,
		ControlFlowGraph* targetMCCCFG, int populationSize, double mutationProb,
		double crossOverProb, int numberOfGenerations){

	BranchCoverageGraph = targetBranchCFG;
	MultiCondCoverageGraph = targetMCCCFG;
	this->populationSize = populationSize;
	this->numberOfGenerations = numberOfGenerations;

	population = new Population(populationSize, targetBranchCFG);
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
