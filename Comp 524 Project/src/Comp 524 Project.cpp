#include <iostream>
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"
#include "Simulation.h"
#include "GlobalVariables.h"
#include "RandomSearcher.h"

#include <chrono>
#include <ctime>


using namespace std;

ControlFlowGraph*targetCFG { };

void simpleIfElseControlFlowGraphTest_testSuite_version() {
	cout << endl << "SimpleIfElse Test Suite Test: " << endl;
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = testCFG;
	TestSuite* testSuite = new TestSuite { 10, 10 };

	testSuite->printAll();
}

void simpleIfElseControlFlowGraphTest_testCase_version() {
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = testCFG;
	cout << endl << "SimpleIfElse Test Case Test: " << endl;
	TestCase* falseTestCase = new TestCase { };
	falseTestCase->setInputParameters(new int[1] {1});

	TestCase* trueTestCase = new TestCase { };
	trueTestCase->setInputParameters(new int[1] {3});

	testCFG->setCoverageOfTestCase(falseTestCase);
	testCFG->setCoverageOfTestCase(trueTestCase);

	cout << endl << "False Test Case:" ;
	falseTestCase->printInputsAndCoverage();
	cout << endl << "True Test Case:";
	trueTestCase->printInputsAndCoverage();
}

void hiLoControlFlowGraphTest_testSuite_version() {
	cout << endl << "HiLo Test Suite Test: " << endl;
	ControlFlowGraph* testCFG = new HiLoControlFlowGraph { };
	targetCFG = testCFG;
	//TestSuite* testSuite = new TestSuite { 10 };

	//testSuite->print();
}

void hiLoRandomSearchEdges() {
	ControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };
	targetCFG = hiLoCFG;
	RandomSearcher* r = new RandomSearcher();
	r->search();
}

void hiLoRandomSearchPredicates() {
	ControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };
	targetCFG = hiLoCFG;
	RandomSearcher* r = new RandomSearcher();
	r->searchForPredicates();
}

void simpleIfElseSimulationTest() {
	//Simulation::Simulation(int populationSize, int initialTestSuiteSize, int maxTestSuiteSize,
	//		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations)

	ControlFlowGraph* simpleIfElseCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = simpleIfElseCFG;
	Simulation* simpleIfElseSim = new Simulation(1000, 10, 15, 2, 1, 1, 10);
	simpleIfElseSim->run();
}

void hiLoSimulationTest() {
	cout << "HiLo Simulation" << endl;
	ControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };

	targetCFG = hiLoCFG;
	Simulation* hiLoSim = new Simulation(1000, 5, 15, 2, .02, 1, 1000000);
	hiLoSim->run();
}

void localOptTest() {
	HiLoControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };
	bool EDGE { true }, PREDICATE { false };
	targetCFG = hiLoCFG;

	Simulation* hiLoSim = new Simulation(1000, 5, 15, 2, .02, 1, 1000000);
	hiLoSim->localOptFromZero(HiLoControlFlowGraph::edges::B8toB10, EDGE);

	hiLoSim->localOptFromZero(HiLoControlFlowGraph::predicates::B10_FF, PREDICATE);
	hiLoSim->localOptFromZero(HiLoControlFlowGraph::predicates::B10_FT, PREDICATE);
	hiLoSim->localOptFromZero(HiLoControlFlowGraph::predicates::B10_TF, PREDICATE);
	hiLoSim->localOptFromZero(HiLoControlFlowGraph::predicates::B10_TT, PREDICATE);
}
void localOptTest2() {
	HiLoControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };
	bool EDGE { true }, PREDICATE { false };
	targetCFG = hiLoCFG;

	Simulation* hiLoSim = new Simulation(1000, 5, 15, 2, .02, 1, 1000000);

for (int var = 0; var < 50; ++var) {
	TestCase* tc = new TestCase{};

	hiLoSim->localOptFromGivenParams(tc, HiLoControlFlowGraph::edges::B8toB10, EDGE);
}


//	hiLoSim->localOptVersion1(HiLoControlFlowGraph::predicates::B10_FF, PREDICATE);
//	hiLoSim->localOptVersion1(HiLoControlFlowGraph::predicates::B10_FT, PREDICATE);
//	hiLoSim->localOptVersion1(HiLoControlFlowGraph::predicates::B10_TF, PREDICATE);
//	hiLoSim->localOptVersion1(HiLoControlFlowGraph::predicates::B10_TT, PREDICATE);
}
int main() {
	//simpleIfElseControlFlowGraphTest_testCase_version();
	//simpleIfElseControlFlowGraphTest_testSuite_version();
	//hiLoControlFlowGraphTest_testSuite_version();

	chrono::time_point<chrono::system_clock> start { }, end { };
    start = chrono::system_clock::now();

    //localOptTest();
    //localOptTest2();
    hiLoSimulationTest();
    //hiLoRandomSearchEdges();
    //hiLoRandomSearchPredicates();

    end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
    cout << "finished computation at " << std::ctime(&end_time);


	return 0;
}



