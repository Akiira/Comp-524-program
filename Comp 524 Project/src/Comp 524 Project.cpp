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
#include "ParameterTestCFG.h"
#include "Simulation.h"
#include "RandomSearcher.h"
#include "HardCFG.h"
#include "Range.h"
#include "RangeSet.h"

#include <iostream>
#include <cstdlib> 
#include <fstream>
#include <chrono>
#include <ctime>

#include "HardLeft.h"
#include "HardRight.h"

using namespace std;

void runWithoutGA();
void setTarget(int);
void runTest(int graph, int test);
void runTestsOnAllGraphs();
void testCutPointsToMutationProb();
void testPopulationSizeToCutPoints();
void testPopulationSizeToMutationProb();
void printFileHeader();
void printFileDataEntry(int, short, double, double);
void runTests(int, int, short, short, double, double);

void printTableOfFinalResults(int type, int population, short cutPoints, double mutation);
void printTableOfFinalResultsForRandomSearcher();

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
const short POPULATION_START = 10;
const short POPULATION_END = 100;
const short POPULATION_STEP = 10;

short TEST_RUNS = 30;	// Overridden to 50 by the final tests.
const short GENERATIONS = 10000;

/*
 * First argument sets target CFG as follows:
 * 	0. SimpleIfElse
 * 	1. HiLo
 * 	2. Hard Left
 * 	3. Triangle
 * 	4. Hard-Right
 * 	5. Hard
 * 	6. ParameterTest
 *
 * Second argument sets test as follows:
 * 	1. Cut points vs Mutation probability
 * 	2. Population size vs Cut points
 * 	3. Population size vs Mutation probability
 * 	4. All of the above
 * 	5. Random search on all graphs for max time taken by the GA on those graphs
 * 	6. Run without GA, i.e. no crossover, mutation, or selection
 * 	7. Run with final test on entire algorithm and print table of results
 * 	8. Run with final test without GA and print table of results
 * 	9. Run with final test without Local Opt and print table of results
 * 	10. ParameterTest
 */

int main(int argc, char* argv[]) {
	assert(argc >= 2);

    auto start = chrono::system_clock::now();
    runTest(atoi(argv[1]), atoi(argv[2]));
    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
    cout << "finished computation at " << std::ctime(&end_time);

	return 0;
}

void runTest(int graph, int test) {
	setTarget(graph);
	Simulation* sim;
	int gens;
	switch (test) {
		case 1:
			cout << "Running CP to MP on: " << testProgram << "\n\n";
			testCutPointsToMutationProb();
			break;
		case 2:
			cout << "Running PS to MP on: " << testProgram << "\n\n";
			testPopulationSizeToMutationProb();
			break;
		case 3:
			cout << "Running PS to CP on: " << testProgram << "\n\n";
			testPopulationSizeToCutPoints();
			break;
		case 4:
			cout << "Running all tests on all graphs" << "\n\n";
			runTestsOnAllGraphs();
			break;
		case 5:
			cout << "Running final results table for random searcher on all graphs " << "\n\n";
			printTableOfFinalResultsForRandomSearcher();
			break;
		case 6:
			cout << "Running simulation without GA on: " << testProgram << "\n\n";
			runWithoutGA();
			break;
		case 7:
			cout << "Running final results table test on all graphs" << "\n\n";
			TEST_RUNS = 50;
			printTableOfFinalResults(0, 25, 2, .5);
			break;
		case 8:
			cout << "Running final results table test on all graphs without GA" << "\n\n";
			TEST_RUNS = 50;
			printTableOfFinalResults(1, 25, 2, .5);
			break;
		case 9:
			cout << "Running final results table test on all graphs without Local Opt" << "\n\n";
			TEST_RUNS = 50;
			printTableOfFinalResults(2, 25, 2, .5);
			break;
		case 10:
			cout << "Running simulation on ParameterTestCFG\n\n";
			sim = new Simulation(25);
			gens = sim->run(GENERATIONS, 2, 0.02);
			cout << "Ran for: " << gens << ", and got coverage ratio: " << sim->getCoverageRatio() << endl;
			break;
		default:
			cerr << "Unrecognized test number in runTest: " << test << endl;
			break;
	}
}

void runWithoutGA() {
	outputFile = new ofstream("HardCFG_NoGA" + testProgram + ".txt", ios::out | ios::trunc);

	double avgGens = 0.0;
	double avgCov  = 0.0;

	for (int n = 0; n < TEST_RUNS; ++n) {
		cout << "Test Num: " << n << " " << endl;
		Simulation* sim = new Simulation(20);
		avgGens += sim->runWithoutGA(GENERATIONS);
		avgCov += sim->getCoverageRatio();
	}

	*outputFile << "Average generations: " << avgGens / TEST_RUNS << "\n";
	*outputFile << "Average Coverage: " << avgCov / TEST_RUNS << "\n";

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
			testProgram = "Hard-Left";
			targetCFG = new HardLeft();		
			break;
		case 3:
			testProgram = "Triangle";
			targetCFG = new TriangleProblemGraph();
			break;
		case 4:
			testProgram = "Hard-Right";
			targetCFG = new HardRight();
			break;
		case 5:
			testProgram = "Hard";
			targetCFG = new HardCFG();
			break;
		case 6:
			testProgram = "ParameterTest";
			targetCFG = new ParameterTestCFG();
			break;
		default:
			cerr << "Unrecognized target number in setTarget: " << i << endl;
			break;
	}
}

void runTestsOnAllGraphs() {

	for (int graph = 0; graph <= 5; ++graph) {
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
			    auto start = chrono::system_clock::now();
				printf("Testing population: %d, cutPoints: %d, mutation: %f, on: %s\n", population, cutPoints, mutation, testProgram.c_str());
				for(int i = 0; i < TEST_RUNS; i++)
				{
					cout << "\tTest Num: " << i << " " << endl;

					Simulation* sim = new Simulation(population);
					int temp = sim->run(GENERATIONS, cutPoints, mutation);

					sumOfCoverageRatios += rangeSet->getFinalTestSuite()->getCoverageRatio();
					sumOfGenerations += temp;
					cout << "\t# of gen: " << temp << " cov: " << rangeSet->getFinalTestSuite()->getCoverageRatio() << endl;

					delete sim;
				}
			    auto end = chrono::system_clock::now();
			    chrono::duration<double> elapsed_seconds = end-start;
			    chrono::duration<double> average_elapsed_seconds = elapsed_seconds / TEST_RUNS;
			    cout << "elapsed time: " << elapsed_seconds.count() << endl;
			    cout << "averaged elapsed time: " << average_elapsed_seconds.count() << endl;

				double average = ((double)sumOfGenerations) / ((double)TEST_RUNS);
				printFileDataEntry(population, cutPoints, mutation, average);
				*outputFile << "\t\t(* Average Coverage Ratio: " << ((double)sumOfCoverageRatios) / ((double)TEST_RUNS) <<  " *)" << endl;
				*outputFile << "\t\t(* Average Seconds Per Run " << average_elapsed_seconds.count()  <<  " *)" << endl;

				outputFile->flush();

				cout << "Average number of generations: " << average << endl;
			}
		}
	}
}

/*
 * Type:
 * 	0 - Entire algorithm
 * 	1 - WIthout GA (Only initial population, local opt)
 * 	2 - Without Local Opt (only initial population, crossover, mutation, and range adaptation)
 * 	3 - Random Searcher
 */
void printTableOfFinalResults(int type, int population, short cutPoints, double mutation)
{
	switch(type) {
		case 0:
			outputFile = new ofstream("finalResultsTableEntireAlgorithm.txt", ios::out | ios::trunc);
			break;
		case 1:
			outputFile = new ofstream("finalResultsTableWithoutGA.txt", ios::out | ios::trunc);
			break;
		case 2:
			outputFile = new ofstream("finalResultsTableWithoutLocalOpt.txt", ios::out | ios::trunc);
			break;
	}

	printFileHeader();
	*outputFile << "% {Population Size: " << population << ", Cut Points: " << cutPoints << ", Mutation Prob: " << mutation << "} " << endl;
	*outputFile <<  "\\begin{center}" << endl;
	*outputFile <<  "\\begin{tabular}{| l | c | c | c | c |}"<< endl;
	*outputFile <<  "\\hline"<< endl;
	*outputFile <<  "\t"    << "CFG" << " & "
							<< "Branch Coverage" << " & "
							<< "MCC Coverage" << " & "
							<< "Avg. Generations" << " & "
							<< "Avg. Seconds" << " \\\\ "
							<< "\\hline" << endl;
	for (int graph = 0; graph <= 6; ++graph) {
		setTarget(graph);

		int sumOfGenerations = 0;
		double sumOfCoverageRatios = 0;
		double* avgCountsTestCaseSources = new double[5] {};
	    auto start = chrono::system_clock::now();
		printf("Testing population: %d, cutPoints: %d, mutation: %f, on: %s\n", population, cutPoints, mutation, testProgram.c_str());
		for(int i = 0; i < TEST_RUNS; i++)
		{
			cout << "\tTest Num: " << i << " " << endl;

			Simulation* sim = new Simulation(population);
			int temp;
			switch(type) {
				case 0:
					temp = sim->run(GENERATIONS, cutPoints, mutation);
					break;
				case 1:
					temp = sim->runWithoutGA(GENERATIONS);
					break;
				case 2:
					temp = sim->runWithoutLocalOpt(GENERATIONS, cutPoints, mutation);
					break;
			}


			sumOfCoverageRatios += rangeSet->getFinalTestSuite()->getCoverageRatio();
			sumOfGenerations += temp;
			cout << "\t# of gen: " << temp << " cov: " << rangeSet->getFinalTestSuite()->getCoverageRatio() << endl;

			int* testCaseSources = rangeSet->getTestCaseSourceCounts();
			for (int i = 0; i < 5; i++) {
				avgCountsTestCaseSources[i] += testCaseSources[i];
			}
			delete sim;
		}

	    auto end = chrono::system_clock::now();
	    chrono::duration<double> elapsed_seconds = end-start;
	    chrono::duration<double> average_elapsed_seconds = elapsed_seconds / TEST_RUNS;


		double averageGen = ((double)sumOfGenerations) / ((double)TEST_RUNS);

		*outputFile <<  "\t"
				<< testProgram.c_str() << " & "
				<< rangeSet->getFinalTestSuite()->getBranchCoverageRatio() << " & "
				<< rangeSet->getFinalTestSuite()->getMCCCoverageRatio() << " & "
				<< averageGen << " & "
				<< average_elapsed_seconds.count()
				<< " \\\\ \\hline" << endl;
		for (int i = 0; i < 5; i++) {
			avgCountsTestCaseSources[i] /= (double)TEST_RUNS;
		}
		*outputFile << rangeSet->getAvgTestCaseSourceReport(avgCountsTestCaseSources);
		outputFile->flush();

		cout << "Average number of generations: " << averageGen << endl;
	    cout << "elapsed time for " << TEST_RUNS << " runs: " << elapsed_seconds.count() << endl;
	    cout << "averaged elapsed time: " << average_elapsed_seconds.count() << endl;
	}
	*outputFile <<  " \\end{tabular}"<< endl;
	*outputFile <<  "\\end{center}"<< endl;
	outputFile->flush();
}

void printTableOfFinalResultsForRandomSearcher()
{
	outputFile = new ofstream("finalResultsTableRandomSearcher.txt", ios::out | ios::trunc);
	*outputFile << "% Random Searcher" << endl;
	*outputFile <<  "\\begin{center}" << endl;
	*outputFile <<  "\\begin{tabular}{| l | c | c | c | c |}"<< endl;
	*outputFile <<  "\\hline"<< endl;
	*outputFile <<  "\t"    << "CFG" << " & "
							<< "Branch Coverage" << " & "
							<< "MCC Coverage" << " & "
							<< "Seconds" << " \\\\ "
							<< "\\hline" << endl;

	// SimpleIfElse (Ignored), HiLo, Hard-Left, Triangle, Hard-Right, Hard, param(ignored)
	double secondsForEachGraph[7] =  {0, 1.14958, 26.5636, 6.08553, 34.7523, 95.5332, 0};

	for (int graph = 0; graph <= 6; ++graph) {
		setTarget(graph);

		double sumOfBranchCoverageRatios = 0;
		double sumOfMCCCoverageRatios = 0;
		cout << "Testing Random Searcher on " << testProgram.c_str();
		for(int i = 0; i < TEST_RUNS; i++)
		{
			TestSuite* randomSuite = RandomSearcher::searchAndReturnSuite(secondsForEachGraph[graph]);
			sumOfBranchCoverageRatios += randomSuite->getBranchCoverageRatio();
			sumOfMCCCoverageRatios += randomSuite->getMCCCoverageRatio();
			delete randomSuite;

			cout << "\tTest Num: " << i << " Branch: " <<randomSuite->getBranchCoverageRatio() << " MCC: " <<  randomSuite->getMCCCoverageRatio() << endl;
		}

		*outputFile <<  "\t"
				<< testProgram.c_str() << " & "
				<< sumOfBranchCoverageRatios / (double)TEST_RUNS << " & "
				<< sumOfMCCCoverageRatios / (double)TEST_RUNS << " & "
				<< secondsForEachGraph[graph]
				<< " \\\\ \\hline" << endl;
		outputFile->flush();
		cout << secondsForEachGraph[graph] << endl;
	}
	*outputFile <<  " \\end{tabular}"<< endl;
	*outputFile <<  "\\end{center}"<< endl;
	outputFile->flush();
}




