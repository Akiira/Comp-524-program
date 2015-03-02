///////////////////////////////////////////////////////////
//  Organism.cpp
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Organism.h"
#include "ControlFlowGraph.h"

Organism::~Organism(){
	delete chromosome;
}

Organism::Organism(ControlFlowGraph& target) {
	chromosome = new TestSuite(666, &target); //TODO: remove hardcoded value
	setFitness(target);
}

TestSuite* Organism::getChromosome() const{
	return  chromosome;
}

int Organism::getFitness() const{
	return fitness;
}


void Organism::mutate(){

}


void Organism::setFitness(ControlFlowGraph& target){
	int sum = 0;
	for(int i = 0; i < chromosome->getNumberOfTestCases(); i++){

	}

	fitness = sum;
}
