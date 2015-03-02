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
	return block1(testCase);
}

bool** HiLoControlFlowGraph::block1(TestCase* testCase) {
	bool** coverage = block2(testCase);
	coverage[0][HiLoControlFlowGraph::B1toB2] = true;
	return coverage;
}


bool** HiLoControlFlowGraph::block2(TestCase* testCase) {
	bool ** coverage;
	if (true /*testCase.guess == testCase.target*/)
	{
		coverage = block3(testCase);
		coverage[0][HiLoControlFlowGraph::B2toB3] = true;
		coverage[1][HiLoControlFlowGraph::B2_T] = true;
	}
	else
	{
		coverage = block4(testCase);
		coverage[0][HiLoControlFlowGraph::B2toB4] = true;
		coverage[1][HiLoControlFlowGraph::B2_F] = true;
	}
	return coverage;
}

bool** HiLoControlFlowGraph::block3(TestCase* testCase) {
	return 0;
}


bool** HiLoControlFlowGraph::block4(TestCase* testCase) {
	return 0;
}
bool** HiLoControlFlowGraph::block5(TestCase* testCase) {
	return 0;
}
bool** HiLoControlFlowGraph::block6(TestCase* testCase) {
	return 0;
}
bool** HiLoControlFlowGraph::block7(TestCase* testCase) {
	return 0;
}
bool** HiLoControlFlowGraph::block8(TestCase* testCase) {
	return 0;
}
bool** HiLoControlFlowGraph::block9(TestCase* testCase) {
	return 0;
}
bool** HiLoControlFlowGraph::block10(TestCase* testCase) {
	return 0;
}

