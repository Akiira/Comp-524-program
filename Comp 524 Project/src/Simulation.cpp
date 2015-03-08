///////////////////////////////////////////////////////////
//  Simulation.cpp
//  Implementation of the Class Simulation
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Simulation.h"
#include "GlobalVariables.h"
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

Simulation::~Simulation(){
	delete population;
}

Simulation::Simulation(int populationSize, int initialTestSuiteSize,
		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations){

	this->populationSize = populationSize;
	this->initialTestSuiteSize = initialTestSuiteSize;
	this->numberOfGenerations = numberOfGenerations;
	this->numberOfCutPoints = numberOfCutPoints;
	probabilityForMutation = mutationProb;
	probabilityForCrossover = crossOverProb;
	bestOrganismSeen = 0;

	population = new Population { populationSize, initialTestSuiteSize };


	//once the population is initialized each organism needs to be evaluated
	// once each organism is evaluated we can set bestOrganismSeen
}


TestSuite* Simulation::getBestTestSuite(){
	return bestOrganismSeen->getChromosome();
}


void Simulation::run(){
	int i { 0 };
	Organism *child1 { }, *child2 { };

	do{
		population->printPopulationFitness();
		auto parent1 = population->fitnessProportionalSelect();
		auto parent2 = population->fitnessProportionalSelect();

		cout << "Generation # " << i << endl;
		//cout << "Parents" << endl;
		//parent1->printFitnessAndCoverage();
		//parent2->printFitnessAndCoverage();

		population->crossover(*parent1, *parent2, child1, child2, numberOfCutPoints);
		child1->mutate(probabilityForMutation);
		child1->setFitness();
		child2->mutate(probabilityForMutation);
		child2->setFitness();

		assert(child1);
		assert(child2);
		//cout << "Children" << endl;
		//child1->printFitnessAndCoverage();
		//child2->printFitnessAndCoverage();

		if(child1 <= child2)
		{
			population->replace(*child2);
		}
		else {
			population->replace(*child1);
		}

		i++;

	}while(i < numberOfGenerations);
	population->printPopulationFitness();

}












