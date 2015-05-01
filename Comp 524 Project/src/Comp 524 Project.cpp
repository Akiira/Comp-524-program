///////////////////////////////////////////////////////////
//  Comp 524 Project.cpp
//  Main
//  Created on:      27-Feb-2015 12:11:44 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"
#include "TriangleProblemGraph.h"
#include "Simulation.h"
#include "RandomSearcher.h"
#include "MediumCFG.h"
#include "MedHardCFG.h"
#include "HardCFG.h"
#include "Range.h"
#include "RangeSet.h"

#include <iostream>
#include <cstdlib> 
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;

void runWithoutGA();
void setTarget(int);
void runTestsOnAllGraphs();
void testCutPointsToMutationProb();
void testPopulationSizeToCutPoints();
void testPopulationSizeToMutationProb();
void printFileHeader();
void printFileDataEntry(int, short, double, double);
void runTests(int, int, short, short, double, double);

ControlFlowGraph*targetCFG { };
RangeSet*rangeSet { };

ofstream* outputFile;
string testProgram;
short testType;


const float MUTATION_STANDARD = 0.02;
const float MUTATION_START = 0.00;
const float MUTATION_END = 1.00;
const float MUTATION_STEP = 0.05;

const short CUTPOINTS_STANDARD = 2;
const short CUTPOINTS_START = 0;
const short CUTPOINTS_END = 4;
const short CUTPOINTS_STEP = 1;

const short POPULATION_STANDARD = 20;
const short POPULATION_START = 5;
const short POPULATION_END = 100;
const short POPULATION_STEP = 5;

const short TEST_RUNS = 30;
const short GENERATIONS = 10000;

int main(int argc, char* argv[]) {
    auto start = chrono::system_clock::now();

    //runTestsOnAllGraphs();
    //runWithoutGA();
    targetCFG = new HardCFG();
    RandomSearcher::search(60 * 60 * atoi(argv[1]));

    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
    cout << "finished computation at " << std::ctime(&end_time);

	return 0;
}

void runWithoutGA() {
	targetCFG = new HardCFG();
	outputFile = new ofstream("HardCFG_NoGA" + testProgram + ".txt", ios::out | ios::trunc);

	runTests(50, 50, 0, 0, 0, 0);

	*outputFile << "}\n ListPlot3D[Transpose@Partition[Last /@ data, 15], DataRange -> {{" << CUTPOINTS_START << ", " << CUTPOINTS_END << "}, {" << MUTATION_START << ", " << MUTATION_END << "},{" << 1 << ", " << GENERATIONS << "}}, PlotRange -> All, AxesLabel -> {Cut Size, Mutation Probability, Generations}, ColorFunction -> \"Rainbow\"]";
	outputFile->close();
	cout << "Data written to file" << endl;
}

void setTarget(int i) {
	switch (i) {
		case 0:
			testProgram = "SimpleIfElse";
			targetCFG = new SimpleIfElseControlFlowGraph();
			break;
		case 1:
			testProgram = "HiLo";
			targetCFG = new HiLoControlFlowGraph();
			break;
		case 2:
			testProgram = "Medium-Hard-One";
			targetCFG = new MediumCFG();		
			break;
		case 3:
			testProgram = "Triangle";
			targetCFG = new TriangleProblemGraph();
			break;
		case 4:
			testProgram = "Medium-Hard-Two";
			targetCFG = new MedHardCFG();
			break;
		case 5:
			testProgram = "Hard";
			targetCFG = new HardCFG();
			break;
		default:
			break;
	}
}

void runTestsOnAllGraphs() {

	for (int graph = 2; graph == 2; ++graph) {
		setTarget(graph);
		//testCutPointsToMutationProb();
		//testPopulationSizeToMutationProb();
		testPopulationSizeToCutPoints();
	}
}

void testCutPointsToMutationProb(){
	testType   = 0;
	outputFile = new ofstream("cuts_To_Mutation_" + testProgram + ".txt", ios::out | ios::trunc);

	printFileHeader();
	*outputFile << "(* {Cut Points, Mutation Probability, Generations} *)" << endl;
	*outputFile << "(* Standard Population Size: " << POPULATION_STANDARD << " *)" <<  endl;

	runTests(POPULATION_STANDARD, POPULATION_STANDARD, CUTPOINTS_START,	CUTPOINTS_END,
									MUTATION_START, MUTATION_END);

*outputFile << "}\n ListPlot3D[Transpose@Partition[Last /@ data, 15], DataRange -> {{" << CUTPOINTS_START << ", " << CUTPOINTS_END << "}, {" << MUTATION_START << ", " << MUTATION_END << "},{" << 1 << ", " << GENERATIONS << "}}, PlotRange -> All, AxesLabel -> {Cut Size, Mutation Probability, Generations}, ColorFunction -> \"Rainbow\"]";
	outputFile->close();
	cout << "Data written to file" << endl;
}

void testPopulationSizeToMutationProb(){
	testType   = 1;
	outputFile = new ofstream("pop_To_Mutation_" + testProgram + ".txt", ios::out | ios::trunc);

	printFileHeader();
	*outputFile << "(* {Population Size, Mutation Probability, Generations} *)" << endl;
	*outputFile << "(* Standard Cut Points: " << CUTPOINTS_STANDARD << " *)" <<  endl;

	runTests(POPULATION_START, POPULATION_END, CUTPOINTS_STANDARD, CUTPOINTS_STANDARD,
									MUTATION_START, MUTATION_END);

	*outputFile << "}\n ListPlot3D[Transpose@Partition[Last /@ data, 15], DataRange -> {{" << POPULATION_START << ", " << POPULATION_END << "}, {" << MUTATION_START << ", " << MUTATION_END << "},{" << 1 << ", " << GENERATIONS << "}}, PlotRange -> All, AxesLabel -> {Population Size, Mutation Probability, Generations}, ColorFunction -> \"Rainbow\"]";
	outputFile->close();
	cout << "Data written to file" << endl;

}

void testPopulationSizeToCutPoints()
{
	testType   = 2;
	outputFile = new ofstream("pop_To_CutPoints_" + testProgram + ".txt", ios::out | ios::trunc);

	printFileHeader();
	*outputFile << "(* {Population Size, Cut Points, Generations} *)" << endl;
	*outputFile << "(* Standard Mutation Probability: " << MUTATION_STANDARD << " *)" << endl;

	runTests(POPULATION_START, POPULATION_END, CUTPOINTS_START,	CUTPOINTS_END,
									MUTATION_STANDARD, MUTATION_STANDARD);

	*outputFile << "}\n ListPlot3D[Transpose@Partition[Last /@ data, 15], DataRange -> {{" << POPULATION_START << ", " << POPULATION_END << "}, {" << CUTPOINTS_START<< ", " << CUTPOINTS_END << "},{" << 1 << ", " << GENERATIONS << "}}, PlotRange -> All, AxesLabel -> {Population Size, Cut Points, Generations}, ColorFunction -> \"Rainbow\"]";
	outputFile->close();
	cout << "Data written to file" << endl;
}

void printFileHeader() {
	*outputFile << "(* Number of tests per point: " << TEST_RUNS <<  " *)" << endl;
	*outputFile << "(* Maximum Generations: " << GENERATIONS <<  " *)" << endl;
	*outputFile << "(* Test Program: " << testProgram <<  " *)" << endl;
}

void printFileDataEntry(int population, short cutpoints, double mutation, double average) {
	switch (testType)
	{
		case 0: // Mapping Cut Points to Mutation Probability
			*outputFile << "{" << cutpoints << ", " << mutation << ", " << average << "}, ";
			break;
		case 1: // Mapping Population Size to Mutation Probability
			*outputFile << "{" << population << ", " << mutation << ", " << average << "}, ";
			break;
		case 2: // Mapping Population Size to Cut Points
			*outputFile << "{" << population << ", " << cutpoints << ", " << average << "}, ";
			break;
		default:
			*outputFile << "---ERROR---";
			cout << "ERROR, invalid test configuration";
			break;
	}
}

void runTests(int popStart, int popEnd, short cutPtsStart,
			  short cutPtsEnd, double mutationStart, double mutationEnd)
{
	*outputFile << "data := {";

	for (int population = popStart; population <= popEnd; population += POPULATION_STEP)
	{
		for(short cutPoints = cutPtsStart; cutPoints <= cutPtsEnd; cutPoints += CUTPOINTS_STEP)
		{
			for(double mutation = mutationStart; mutation <= mutationEnd; mutation += MUTATION_STEP)
			{
				int sumOfGenerations = 0;
				double sumOfCoverageRatios = 0;
				printf("Testing population: %d, cutPoints: %d, mutation: %f, on: %s\n", population, cutPoints, mutation, testProgram.c_str());
				for(int i = 0; i < TEST_RUNS; i++)
				{
					cout << "\tTest Num: " << i << " " << endl;

					Simulation* sim = new Simulation(population);
					int temp;
					if(mutation <= 0.01 && cutPoints == 0) {
						cout << "\tWithout GA" << endl;
						temp = sim->runWithoutGA(GENERATIONS);
					} else {
						temp = sim->run(GENERATIONS, cutPoints, mutation);
					}


					sumOfCoverageRatios += rangeSet->getFinalTestSuite()->getCoverageRatio();
					sumOfGenerations += temp;
					cout << "\t# of gen: " << temp << " cov: " << rangeSet->getFinalTestSuite()->getCoverageRatio() << endl;

					delete sim;
				}

				double average = ((double)sumOfGenerations) / ((double)TEST_RUNS);
				printFileDataEntry(population, cutPoints, mutation, average);
				*outputFile << "\t\t(* Average Coverage Ratio: " << ((double)sumOfCoverageRatios) / ((double)TEST_RUNS) <<  " *)" << endl;
				outputFile->flush();

				cout << "Average number of generations: " << average << endl;
			}
		}
	}
}



