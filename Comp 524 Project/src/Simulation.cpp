///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "Simulation.h"
#include "GlobalVariables.h"
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

Simulation::~Simulation(){
	delete population;
	delete bestOrganismSeen;
}

Simulation::Simulation(int populationSize, int initialTestSuiteSize, int maxTestSuiteSize,
		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations){

	this->populationSize = populationSize;
	this->initialTestSuiteSize = initialTestSuiteSize;
	this->maxTestSuiteSize = maxTestSuiteSize;
	this->numberOfGenerations = numberOfGenerations;
	this->numberOfCutPoints = numberOfCutPoints;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;

	population = new Population { populationSize, initialTestSuiteSize, maxTestSuiteSize };

	bestOrganismSeen = new Organism { 1, maxTestSuiteSize };
	*bestOrganismSeen = *population->getBestOrganism();
}

void Simulation::run(){
	int i { 0 };
	Organism *child1 { }, *child2 { };

	do{
		population->printPopulationFitness();
		auto parent1 = population->fitnessProportionalSelect();
		auto parent2 = population->fitnessProportionalSelect();

		cout << "Generation # " << i << endl;

		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);
		child1->mutate(probabilityForMutation);
		child2->mutate(probabilityForMutation);

		assert(child1 && child2);

		//TODO add a periodic call to some local optimization in this loop.
		//TODO possibly add some periodic adaptation of parameters like mutation

		if(child1 <= child2){
			population->replace(child2);
			delete child1;
		}
		else {
			population->replace(child1);
			delete child2;
		}

		i++;

		//TODO Check if it looks like the last generation always have the best organism
		//		and if so, we can remove this. We can check this when we are closer to final version
		if( *bestOrganismSeen < *population->getBestOrganism() ) {
			*bestOrganismSeen = *population->getBestOrganism();
		}

	}while(i < numberOfGenerations);

	bestOrganismSeen->printFitnessAndCoverage();
	population->printPopulationFitness();

}












