#include <iostream>
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"

using namespace std;

void simpleIfElseControlFlowGraphTest_testSuite_version() {
	cout << endl << "SimpleIfElse Test Suite Test: " << endl;
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	TestSuite* testSuite = new TestSuite { 10, testCFG };

	testSuite->print();
}

void simpleIfElseControlFlowGraphTest_testCase_version() {
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
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
	TestSuite* testSuite = new TestSuite { 10, testCFG };

	testSuite->print();
}

int main() {
	simpleIfElseControlFlowGraphTest_testCase_version();
	simpleIfElseControlFlowGraphTest_testSuite_version();
	hiLoControlFlowGraphTest_testSuite_version();
	return 0;
}



