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
	targetCFG = &target;
	initialized = false;
	evaluated = false;
	fitness = 0;
	chromosome = 0;
	//chromosome = new TestSuite(666, &target); //TODO: remove hardcoded value
	//setFitness(target);
}

void Organism::initializeRandomChromosome() {
	chromosome = new TestSuite(10, targetCFG);

}

TestSuite* Organism::getChromosome() const{
	return  chromosome;
}

int Organism::getFitness() const{
	return fitness;
}


void Organism::mutate(double mutationProb){

}

void Organism::setFitness(){
	int sum = 0;
	for(int i = 0; i < chromosome->getNumberOfTestCases(); i++){

	}

	fitness = sum;
}

bool Organism::operator<=(const Organism& right) { //overloaded operator <=
	return (fitness <= right.fitness) ? true : false;
} //operator<=

bool Organism::operator==(const Organism& right) { //overloaded operator ==
												   //TODO implement function
	return false;
	//return (strcmp(chromosome, right.chromosome) == 0)? true : false;
}	//operator==

Organism& Organism::operator=(const Organism& org) {	//assignment operator
	//TODO: implement this.
//	if (this != &org) {
//		delete[] chromosome;
//		chromosome = new char[org.length + 1];
//		assert(chromosome != NULL);
//		strcpy(chromosome, org.chromosome);
//		fitness = org.fitness;
//		length = org.length;
//	}	//if
	return *this;
}//operator=
