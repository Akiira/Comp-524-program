/*
 * HiLoControlFlowGraph.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: amb6470
 */
#include "HiLoControlFlowGraph.h"

bool** HiLoControlFlowGraph::getCoverageOfTestCase(TestCase* testCase) {
	return start(testCase);
}

bool** HiLoControlFlowGraph::start(TestCase* testCase) {

}

bool** HiLoControlFlowGraph::block1(TestCase* testCase) {
	bool** coverage = block2(testCase);
	coverage[0][HiLoControlFlowGraph::branches::B1toB2] = true;
	return coverage;
}


bool** HiLoControlFlowGraph::block2(TestCase* testCase) {
	bool ** coverage;
	if (true /*testCase.guess == testCase.target*/)
	{
		coverage = block3(testCase);
		coverage[0][HiLoControlFlowGraph::branches::B2toB3] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B2_T] = true;
	}
	else
	{
		coverage = block4(testCase);
		coverage[0][HiLoControlFlowGraph::branches::B2toB4] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B2_F] = true;
	}
}

bool** block3(TestCase* testCase) {

}


bool** block4(TestCase* testCase);
bool** block5(TestCase* testCase);
bool** block6(TestCase* testCase);
bool** block7(TestCase* testCase);
bool** block8(TestCase* testCase);
bool** block9(TestCase* testCase);
bool** block10(TestCase* testCase);

