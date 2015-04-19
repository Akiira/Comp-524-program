#include <iostream>
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

#include <chrono>
#include <ctime>


using namespace std;

ControlFlowGraph*targetCFG { };

//Flags to interactivly set printing during simulation

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

    targetCFG = new MedHardCFG { };
    Simulation* medSim = new Simulation(100);
    medSim->run(100000, 5, 0.02);

    end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
    cout << "finished computation at " << std::ctime(&end_time);

	return 0;
}





