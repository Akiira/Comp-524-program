#include <iostream>
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"

using namespace std;

void controlFlowGraphTest_testSuite_version() {
	cout << "Test Suite Test: " << endl;
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	TestSuite* testSuite = new TestSuite { 10, testCFG };

	testSuite->print();
}

void controlFlowGraphTest_testCase_version() {
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph { };
	cout << endl << endl << "Test Case Test: " << endl;
	TestCase* falseTestCase = new TestCase { testCFG->getNumberOfParameters(),
			testCFG->getNumberOfEdges(), testCFG->getNumberOfPredicates() };
	falseTestCase->setInputParameters(new int[1] {1});

	TestCase* trueTestCase = new TestCase { testCFG->getNumberOfParameters(),
			testCFG->getNumberOfEdges(), testCFG->getNumberOfPredicates() };
	trueTestCase->setInputParameters(new int[1] {3});

	testCFG->setCoverageOfTestCase(falseTestCase);
	testCFG->setCoverageOfTestCase(trueTestCase);

	falseTestCase->print();
	trueTestCase->print();
}

int main() {
	controlFlowGraphTest_testSuite_version();
	controlFlowGraphTest_testCase_version();
	return 0;
}



