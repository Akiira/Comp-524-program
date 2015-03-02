/*
 * SimpleIfElseControlFlowGraph.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: amb6470
 */

#include "SimpleIfElseControlFlowGraph.h"

SimpleIfElseControlFlowGraph::SimpleIfElseControlFlowGraph() {
	// TODO Auto-generated constructor stub

}

SimpleIfElseControlFlowGraph::~SimpleIfElseControlFlowGraph() {
	// TODO Auto-generated destructor stub
}

bool * SimpleIfElseControlFlowGraph::block1(TestCase testCase) {
	int* parameters = testCase.getInputParameters();
	bool* coverage;

	if(parameters[0] > 2)
		coverage = block3(testCase);
	else
		coverage = block2(testCase);


	coverage[0] = true;

	return coverage;
}

bool * SimpleIfElseControlFlowGraph::block2(TestCase testCase) {
	bool* coverage = block4(testCase);
	coverage[1] = true;

	return coverage;

}

bool * SimpleIfElseControlFlowGraph::block3(TestCase testCase) {
	bool* coverage = block4(testCase);
	coverage[2] = true;

	return coverage;
}

bool * SimpleIfElseControlFlowGraph::block4(TestCase testCase) {
	bool* coverage = new bool[4];
	coverage[0] = false;
	coverage[1] = false;
	coverage[2] = false;
	coverage[3] = true;
	return coverage;
}

