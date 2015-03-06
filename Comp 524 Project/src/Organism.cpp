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

// Note have to explicitly call a initializeChromosome method to create the chromosome.
//	and have to explicitly call setFitness to evaluate the fitness of the chromosome.
Organism::Organism(ControlFlowGraph& target) {
	targetCFG = &target;
	initialized = false;
	evaluated = false;
	fitness = 0;
	chromosome = 0;
}

// Will take an array of pointers to test cases. And call the version of TestSuite constructor
//	that performs a deep copy on them to create a new TestSuite object. Necessary for crossover
void Organism::initializeChromosomeFromTestCases(int numberOfTestCases, TestCase** testCases) {
	chromosome = new TestSuite(numberOfTestCases, testCases, targetCFG);
	initialized = true;
}

void Organism::initializeRandomChromosome() {
	chromosome = new TestSuite(10, targetCFG);
	initialized = true;
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
