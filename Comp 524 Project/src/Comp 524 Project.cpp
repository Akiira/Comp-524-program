

#include <iostream>
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
#include "HiLoControlFlowGraph.h"
#include "SimpleIfElseControlFlowGraph.h"

using namespace std;

void controlFlowGraphTest() {
	ControlFlowGraph* testCFG = new SimpleIfElseControlFlowGraph();

	TestCase* falseTestCase = new TestCase(testCFG->getNumberOfEdges(), testCFG->getNumberOfPredicates(), 1);
	falseTestCase->setInputParameters(new int[1] {1});

	TestCase* trueTestCase = new TestCase(testCFG->getNumberOfEdges(), testCFG->getNumberOfPredicates(), 1);
	trueTestCase->setInputParameters(new int[1] {3});

	bool** falseCoverage = testCFG->getCoverageOfTestCase(falseTestCase);

	bool** trueCoverage = testCFG->getCoverageOfTestCase(trueTestCase);

	//cout << &falseCoverage << " " << &trueCoverage << endl;

	cout << "x == 1 testCase Edge Coverage:" << endl;
	for (int i = 0; i < testCFG->getNumberOfEdges(); i++) {
		cout << falseCoverage[0][i] << " | ";
	}
	cout << endl << "x == 1 testCase Predicate Coverage:" << endl;
	for (int i = 0; i < testCFG->getNumberOfPredicates(); i++) {
		cout << falseCoverage[1][i] << " | ";
	}

	cout << endl << "x == 3 testCase Edge Coverage:" << endl;
	for (int i = 0; i < testCFG->getNumberOfEdges(); i++) {
		cout << trueCoverage[0][i] << " | ";
	}
	cout << endl << "x == 3 testCase Predicate Coverage:" << endl;
	for (int i = 0; i < testCFG->getNumberOfPredicates(); i++) {
		cout << trueCoverage[1][i] << " | ";
	}
}

int main() {
	cout << "Run" << endl;
	controlFlowGraphTest();

	return 0;
}



