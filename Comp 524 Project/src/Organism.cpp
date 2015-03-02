///////////////////////////////////////////////////////////
//  Organism.cpp
//  Implementation of the Class Organism
//  Created on:      27-Feb-2015 12:13:22 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "Organism.h"


Organism::~Organism(){

}


Organism::Organism(){

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
	for(int i = 0; i < chromosome->getNumberOfTestCases(); i++){

	}
}
