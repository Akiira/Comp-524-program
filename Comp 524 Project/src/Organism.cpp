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

}

TestSuite* Organism::getChromosome(){
	return  chromosome;
}

int Organism::getFitness(){
	return fitness;
}


void Organism::mutate(){

}


void Organism::setFitness(const ControlFlowGraph& target){
	int sum = 0;
	for(int i = 0; i < chromosome->getNumberOfTestCases(); i++){

	}

	fitness = sum;
}
