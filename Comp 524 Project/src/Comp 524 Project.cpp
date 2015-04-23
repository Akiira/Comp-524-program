
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"
#include "TriangleProblemGraph.h"
#include "Simulation.h"
#include "GlobalVariables.h"
#include "RandomSearcher.h"
#include "MediumCFG.h"
#include "MedHardCFG.h"
#include "HardCFG.h"
#include "Range.h"
#include "RangeSet.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;

void setTarget(int);
void runTestsOnAllGraphs();
void testCutPointsToMutationProb();
void testPopulationSizeToCutPoints();
void testPopulationSizeToMutationProb();
void printFileHeader();
void printFileFooter();
void printFileDataEntry(int, short, double, double);
void runTests(int, int, short, short, double, double);

ControlFlowGraph*targetCFG { };
RangeSet*rangeSet { };
Population*globalPopulation { };	// TODO: This is currently used by rangeSet, but I realized it may be a problem for multithreading tests.

ofstream* outputFile;
string testProgram;
short testType;


const float MUTATION_STANDARD = 0.02;
const float MUTATION_START = 0.00;
const float MUTATION_END = 0.75;
const float MUTATION_STEP = 0.05;

const short CUTPOINTS_STANDARD = 2;
const short CUTPOINTS_START = 0;
const short CUTPOINTS_END = 20;//TODO handle upper bound for smaller number of parameters
const short CUTPOINTS_STEP = 1;

const short POPULATION_STANDARD = 150;
const short POPULATION_START = 50;
const short POPULATION_END = 500;
const short POPULATION_STEP = 25;

const short TEST_RUNS = 20;
const short GENERATIONS = 1500;

//Flags to interactivly set printing during simulation
//TODO I may get rid of all these since we dont have time to make use of them.
bool printPopFitnessOnce;
bool printPopFitness;
bool printGenerationAndRatio;
bool printCoverageRatioForTScrossover;
bool printCoverageRatioForTScrossoverOnce;
bool printReplacement;
bool printCoverageRatioForTCcrossover;

bool changeLocalOpt;

bool pauseOnTSCrossover;
bool pauseOnTCcrossover;
bool pauseONlocalOpt;
bool pause;

int main() {
	chrono::time_point<chrono::system_clock> start { }, end { };
    start = chrono::system_clock::now();
    printGenerationAndRatio = true;

    runTestsOnAllGraphs();

    end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
    cout << "finished computation at " << std::ctime(&end_time);

	return 0;
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
			testProgram = "Triangle";
			targetCFG = new TriangleProblemGraph();
			break;
		case 3:
			testProgram = "Medium-Hard-One";
			targetCFG = new MediumCFG();
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

	for (int graph = 3; graph < 6; ++graph) {
		setTarget(graph);
		testCutPointsToMutationProb();
		testPopulationSizeToMutationProb();
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

	*outputFile << "}, AxesLabel -> {Cut Size, Mutation Probability, Generations}, ColorFunction -> \"Rainbow\"]";
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

	*outputFile << "}, AxesLabel -> {Population Size, Mutation Probability, Generations}, ColorFunction -> \"Rainbow\"]";
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

	*outputFile << "}, AxesLabel -> {Population Size, Cut Points, Generations}, ColorFunction -> \"Rainbow\"]";
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
	*outputFile << "ListPlot3D[{";

	for (int population = popStart; population <= popEnd; population += POPULATION_STEP)
	{
		for(short cutPoints = cutPtsStart; cutPoints <= cutPtsEnd; cutPoints += CUTPOINTS_STEP)
		{
			for(double mutation = mutationStart; mutation <= mutationEnd; mutation += MUTATION_STEP)
			{
				int sumOfGenerations = 0;
				printf("Testing population: %d, cutPoints: %d, mutation: %f, on: %s", population, cutPoints, mutation, testProgram.c_str());
				for(int i = 0; i < TEST_RUNS; i++)
				{
					cout << "\tTest Num: " << i << " ";

					Simulation* sim = new Simulation(population);
					int temp = sim->run(GENERATIONS, cutPoints, mutation);
					delete sim;
					sumOfGenerations += temp;

					cout << "\t# of gen: " << temp << endl;
				}

				double average = ((double)sumOfGenerations) / ((double)TEST_RUNS);
				printFileDataEntry(population, cutPoints, mutation, average);
				outputFile->flush();

				cout << "Average number of generations: " << average << endl;
			}
		}
	}
}



