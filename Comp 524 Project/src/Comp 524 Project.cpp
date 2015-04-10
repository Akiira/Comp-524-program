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
#include "HardCFG.h"
#include "Range.h"
#include "RangeSet.h"
#include <chrono>
#include <ctime>


using namespace std;

ControlFlowGraph*targetCFG { };
RangeSet*rangeSet { };

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

void triangleRandomSearchEdges() {
	ControlFlowGraph* hiLoCFG = new TriangleProblemGraph { };
	targetCFG = hiLoCFG;
	RandomSearcher* r = new RandomSearcher();
	r->search();
}

void triangleRandomSearchPredicates() {
	ControlFlowGraph* hiLoCFG = new TriangleProblemGraph { };
	targetCFG = hiLoCFG;
	RandomSearcher* r = new RandomSearcher();
	r->searchForPredicates();
}

void simpleIfElseSimulationTest() {
	//Simulation::Simulation(int populationSize, int initialTestSuiteSize, int maxTestSuiteSize,
	//		int numberOfCutPoints, double mutationProb, double crossOverProb, int numberOfGenerations)

	ControlFlowGraph* simpleIfElseCFG = new SimpleIfElseControlFlowGraph { };
	targetCFG = simpleIfElseCFG;
	Simulation* simpleIfElseSim = new Simulation(100);
	simpleIfElseSim->run(1000, 5, 0.02);
}

void hiLoSimulationTest() {
	cout << "HiLo Simulation" << endl;
	ControlFlowGraph* hiLoCFG = new HiLoControlFlowGraph { };

	targetCFG = hiLoCFG;
	Simulation* hiLoSim = new Simulation(100);
	hiLoSim->run(1000, 5, 0.02);
}

void triangleSimulationTest() {
	ControlFlowGraph* triangleCFG = new TriangleProblemGraph { };

	targetCFG = triangleCFG;
	Simulation* hiLoSim = new Simulation(100);
	hiLoSim->run(1000, 5, 0.02);
}

//void localOptTest3() {
//	ControlFlowGraph* triangleCFG = new TriangleProblemGraph { };
//	targetCFG = triangleCFG;
//	bool PREDICATE { false };//EDGE { true },
//
//	Simulation* hiLoSim = new Simulation(100);
//
//	for (int var = 0; var < 5; ++var) {
//		TestCase* tc = new TestCase{};
//
//	//	cout << "From Random: " ;
//	//	hiLoSim->localOptFromGivenParams(tc, TriangleProblemGraph::edges::B5toB6, EDGE);
//	//	cout << "From middle: " ;
//	//	hiLoSim->localOptFromMiddle(TriangleProblemGraph::edges::B5toB6, EDGE);
//	//	cout << "From zero: " ;
//	//	hiLoSim->localOptFromZero(TriangleProblemGraph::edges::B5toB6, EDGE);
//	//	cout << "\n";
//
//		cout << "From Random: " ;
//		hiLoSim->localOptFromGivenParams(tc, TriangleProblemGraph::predicates::B0_TFT, PREDICATE);
//		cout << "From middle: " ;
//		hiLoSim->localOptFromMiddle(TriangleProblemGraph::predicates::B0_TFT, PREDICATE);
//		cout << "From zero: " ;
//		hiLoSim->localOptFromZero(TriangleProblemGraph::predicates::B0_TFT, PREDICATE);
//		cout << "\n";
//	}
//}

void hardTest(){
	targetCFG = new HardCFG { };
	Simulation* hardSim = new Simulation(100);
	 hardSim->run(100000, 5, 0.02);
}

//-B10toB21
//15toB32, B16toB33, B16toB34, B16toB35,
//B17toB38, B18toB39, B18toB40, B18toB41,
//B19toB44,
//B21toB49, B21toB50, B22toB51, B22toB52, B22toB53,
//B2toB6 B5toB12-B6toB13-B6toB14
//11toB24-B12toB25---B13toB28-B14toB29-B14toB30
//B23toB56, B24toB57, B24toB58, B24toB59, B24toB60,
//B25toB61, B25toB62, B26toB63, B26toB64, B26toB66,
//B27toB68, B28toB69, B28toB70, B28toB71, B28toB72,
//B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78
void testingHardCFGEdges(){
	auto cfg = new HardCFG();
	//targetCFG = new HardCFG();
	targetCFG = cfg;
	auto tc = new TestCase();

	// B9toB20: mu <= 500 sigma > 500 sigma < 1000
	//Covered: B0, B1, B4, B9, B20, B48,
//	int params [] = {600, -600, -600, -600, 600, 600, 600, 600, -600, 600};
//	tc->setInputParameters(params);
//	targetCFG->setCoverageOfTestCase(tc);
//	tc->printInputsAndCoverage();

	//B20toB45: mu <= 500 sigma > 500 sigma < 1000 (sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 )
	//Covered: B0, B1, B4, B9, B20, B45
//	int params [] = {-500, -500, -1150, -1800, -1800, -500, -500, -1150, -1800, -1800};
//	tc->setInputParameters(params);
//	cfg->setCoverageOfTestCase(tc);
//	tc->printInputsAndCoverage();

	//B20toB46: mu <= 500 sigma > 500 sigma < 1000 ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 )
	//Covered: B0, B1, B4, B9, B20, B46
//	int params [] = {-200, -200, 650, 1100, 1100, -200, -200, 650, 1100, 1100};
//	tc->setInputParameters(params);
//	cfg->setCoverageOfTestCase(tc);
//	tc->printInputsAndCoverage();
//	cout << "Mean: " << cfg->Mu << endl;

	//B20toB47: mu <= 500 sigma > 500 sigma < 1000 ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 )
	//Covered: B0, B1, B4, B9, B20, B47
//	int params [] = {-450, -450, -450, 900, 450, -450, -900, 450, 450, 450};
//	tc->setInputParameters(params);
//	cfg->setCoverageOfTestCase(tc);
//	tc->printInputsAndCoverage();


	//B2TOB6, mu > 500 sigma < 1000
	//Covered: B2, B6, B13, B27, B67
//	int params2 [] = {600, 600, 600, 600, 600, 600, 600, 600, 600, 600};
//	tc->setInputParameters(params2);
//	targetCFG->setCoverageOfTestCase(tc);
//	tc->printInputsAndCoverage();

	//B5toB12, //mu > 500 sigma > 1000 sigma < 5000
	//Covered: B2, B5, B12, B26, B65
//	int params3 [] = {500, 500, 500, 500, 500, 2800, 2800, 2800, 2800, 2800};
//	tc->setInputParameters(params3);
//	cfg->setCoverageOfTestCase(tc);
//	tc->printInputsAndCoverage();

	//B29toB73: mu > 500 sigma < 750 sigma > 500 (a1 == a2) && (a2 == a3)
	//Covered: B0, B2, B6, B14, B29, B73
	int params [] = {150, 150, 150, 1500, 1050, 50, -300, 1050, 1050, 1050};
	tc->setInputParameters(params);
	cfg->setCoverageOfTestCase(tc);
	tc->printInputsAndCoverage();
}

void testSomeNewFunctions() {
	targetCFG = new HardCFG { };
	Simulation* hardSim = new Simulation(100);

	auto org = new Organism(258, 258);
	auto ts = org->getChromosome();
	auto tc = ts->getDuplicateTestCase();
	int params [] = {-200, -200, 650, 1100, 1100, -200, -200, 650, 1100, 1100};
	tc->setInputParameters(params);
	targetCFG->setCoverageOfTestCase(tc);
	cout << "Covered something new: " << hardSim->coveredAnyNewForPopulation(tc->getEdgesCovered(), true) << endl;

}

void hardTestWIthTournamentSelectAndCrossoverWithDominance(){
	targetCFG = new HardCFG { };
	Simulation* hardSim = new Simulation(100);
	hardSim->runWithTournamentSelectAndCrossoverWithDominance(1000, 5, 0.02);
}

int main() {
	//simpleIfElseControlFlowGraphTest_testCase_version();
	//simpleIfElseControlFlowGraphTest_testSuite_version();
	//hiLoControlFlowGraphTest_testSuite_version();

	chrono::time_point<chrono::system_clock> start { }, end { };
    start = chrono::system_clock::now();

    //localOptTest();
    //localOptTest2();
    //localOptTest3();
    //hiLoSimulationTest();
    //hiLoRandomSearchEdges();
    //hiLoRandomSearchPredicates();
    //triangleSimulationTest();
    //triangleRandomSearchEdges();
    hardTest();
    //testingHardCFGEdges();
    //hardTestWIthTournamentSelectAndCrossoverWithDominance();

    //testSomeNewFunctions();

    end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
    cout << "finished computation at " << std::ctime(&end_time);


	return 0;
}



