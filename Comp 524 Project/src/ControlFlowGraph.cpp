///////////////////////////////////////////////////////////
//  ControlFlowGraph.cpp
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#include "ControlFlowGraph.h"


ControlFlowGraph::ControlFlowGraph(){

}



ControlFlowGraph::~ControlFlowGraph(){

}

bool * ControlFlowGraph::block1(TestCase testCase) {
	int parameters = testCase.getInputParameters();
	bool* coverage;

	if(parameters[0] > 2)
		coverage = block3(testCase);
	else
		coverage = block2(testCase);


	coverage[0] = true;

	return coverage;
}

bool * ControlFlowGraph::block2(TestCase testCase) {
	bool coverage = block4(testCase);
	coverage[1] = true;

	return coverage;

}

bool * ControlFlowGraph::block3(TestCase testCase) {
	bool coverage = block4(testCase);
	coverage[2] = true;

	return coverage;
}

bool * ControlFlowGraph::block4(TestCase testCase) {
	bool coverage = {false,false, false, false};
	coverage[3] = true;
	return coverage;
}
