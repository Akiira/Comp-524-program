#include <iostream>
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"
#include "Simulation.h"
#include "GlobalVariables.h"

#include <chrono>
#include <ctime>


using namespace std;

ControlFlowGraph* targetCFG;

void simpleIfElseControlFlowGraphTest_testSuite_version() {
	cout << endl << "SimpleIfElse Test Suite Test: " << endl;
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = testCFG;
	TestSuite* testSuite = new TestSuite { 10 };

	testSuite->print();
}

void simpleIfElseControlFlowGraphTest_testCase_version() {
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = testCFG;
	cout << endl << "SimpleIfElse Test Case Test: " << endl;
	TestCase* falseTestCase = new TestCase { testCFG->getNumberOfParameters(),
			testCFG->getNumberOfEdges(), testCFG->getNumberOfPredicates() };
	falseTestCase->setInputParameters(new int[1] {1});

	TestCase* trueTestCase = new TestCase { testCFG->getNumberOfParameters(),
			testCFG->getNumberOfEdges(), testCFG->getNumberOfPredicates() };
	trueTestCase->setInputParameters(new int[1] {3});

	testCFG->setCoverageOfTestCase(falseTestCase);
	testCFG->setCoverageOfTestCase(trueTestCase);

	cout << endl << "False Test Case:" ;
	falseTestCase->print(testCFG);
	cout << endl << "True Test Case:";
	trueTestCase->print(testCFG);
}

void hiLoControlFlowGraphTest_testSuite_version() {
	cout << endl << "HiLo Test Suite Test: " << endl;
	ControlFlowGraph* testCFG = new HiLoControlFlowGraph { };
	targetCFG = testCFG;
	TestSuite* testSuite = new TestSuite { 10 };

	testSuite->print();
}

void hiLoSimulationTest() {
	cout << "HiLo Simulation" << endl;
	ControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };

	targetCFG = hiLoCFG;
	Simulation* hiLoSim = new Simulation(1000, 1000, 2, .01, 1, 10000);
	hiLoSim->run();
}

void simpleIfElseSimulationTest() {
	//Simulation::Simulation(ControlFlowGraph& targetCFG, int populationSize, int initialTestSuiteSize,
	//		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations)

	ControlFlowGraph* simpleIfElseCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = simpleIfElseCFG;
	Simulation* simpleIfElseSim = new Simulation(10, 100, 2, 1, 1, 10);
	simpleIfElseSim->run();
}

int main() {
	//simpleIfElseControlFlowGraphTest_testCase_version();
	//simpleIfElseControlFlowGraphTest_testSuite_version();
	//hiLoControlFlowGraphTest_testSuite_version();

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    hiLoSimulationTest();

    end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";






	return 0;
}



