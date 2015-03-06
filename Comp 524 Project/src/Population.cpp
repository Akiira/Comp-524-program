///////////////////////////////////////////////////////////
//  Population.cpp
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Population.h"
#include "Random.h"

Population::~Population(){

}


Population::Population(int popSize, ControlFlowGraph& targetCFG){
	population = new Organism*[popSize];
	populationSize = popSize;
	for(int i = 0; i < popSize; i++){
		population[i] = new Organism(targetCFG);
	}
	totalFitness = 0;
}


void Population::crossover(Organism& offspring1, Organism& offspring2, Organism& child1, Organism& child2){

}


void Population::initializeStartingPopulation(){

}


void Population::replace(Organism& offspring){

}

//TODO implement fitness proportional selection
Organism* Population::select(){
	return population[uniformInRange(0, populationSize)];
}


void Population::setPopulationFitness(){

}
