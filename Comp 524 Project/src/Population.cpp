///////////////////////////////////////////////////////////
//  Population.cpp
//  Implementation of the Class Population
//  Created on:      27-Feb-2015 12:12:44 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Population.h"

Population::~Population(){

}


Population::Population(int popSize, const ControlFlowGraph& targetCFG){
	population = new Organism*[popSize];

	for(int i = 0; i < popSize; i++){
		population[i] = new Organism(targetCFG);
	}
}


void Population::crossover(Organism* offspring1, Organism* offspring2){

}


void Population::initializeStartingPopulation(){

}


void Population::replace(Organism* offspring){

}


Organism* Population::select(){

	return  0;
}


void Population::setPopulationFitness(){

}
