/*
 * HiLoControlFlowGraph.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: amb6470
 */
#include "HiLoControlFlowGraph.h"

HiLoControlFlowGraph::HiLoControlFlowGraph() {
	NUM_OF_EDGES = 16;
	NUM_OF_PREDICATES = 14;

	coverage = new bool*[2];
	coverage[0] = new bool[NUM_OF_EDGES];
	coverage[1] = new bool[NUM_OF_PREDICATES];
	testCase = 0;

}

HiLoControlFlowGraph::~HiLoControlFlowGraph() {
	// Just implicitly call ControlFlowGraph destructor
}


void HiLoControlFlowGraph::runTestCase() {
	block1();
}

void HiLoControlFlowGraph::block1() {
	coverage[0][HiLoControlFlowGraph::edges::B1toB2] = true;
	block2();
}


void HiLoControlFlowGraph::block2() {
	if (true /*testCase.guess == testCase.target*/)
	{
		coverage[0][HiLoControlFlowGraph::edges::B2toB3] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B2_T] = true;
		block3();
	}
	else
	{
		coverage[0][HiLoControlFlowGraph::edges::B2toB4] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B2_F] = true;
		block4();
	}
}

void HiLoControlFlowGraph::block3() {
	coverage[0][HiLoControlFlowGraph::edges::B3toB10] = true;
	block10();
}


void HiLoControlFlowGraph::block4() {
	if (true /*testCase.guess > testCase.target*/)
	{
		coverage[0][HiLoControlFlowGraph::edges::B4toB5] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B4_T] = true;
		block5();
	}
	else
	{
		coverage[0][HiLoControlFlowGraph::edges::B4toB6] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B4_F] = true;
		block6();
	}
}
void HiLoControlFlowGraph::block5() {
	coverage[0][HiLoControlFlowGraph::edges::B5toB10] = true;
	block10();
}
void HiLoControlFlowGraph::block6() {
	if (true /*testCase.guess < testCase.num1*/)
	{
		if (true /*testCase.guess < testCase.num2*/)
		{
			coverage[1][HiLoControlFlowGraph::predicates::B6_TT] = true;
		}
		else
		{
			coverage[1][HiLoControlFlowGraph::predicates::B6_TF] = true;
		}
		//Its an 'or' condition, follow true branch since first one was true
		coverage[0][HiLoControlFlowGraph::edges::B6toB7] = true;
		block7();
	}
	else if (true /*testCase.guess < testCase.num2*/)
	{
		// We already know first part of the condition is false
		coverage[1][HiLoControlFlowGraph::predicates::B6_FT] = true;

		coverage[0][HiLoControlFlowGraph::edges::B6toB7] = true;
		block7();
	}
	else {
		// Both parts of the condition are false
		coverage[1][HiLoControlFlowGraph::predicates::B6_FF] = true;

		coverage[0][HiLoControlFlowGraph::edges::B6toB8] = true;
		block8();
	}
}
void HiLoControlFlowGraph::block7() {
	coverage[0][HiLoControlFlowGraph::edges::B7toB10] = true;
	block10();
}
void HiLoControlFlowGraph::block8() {
	if (true /*testCase.guess != 0 */)
	{
		coverage[0][HiLoControlFlowGraph::edges::B8toB9] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B8_T] = true;
		block5();
	}
	else
	{
		coverage[0][HiLoControlFlowGraph::edges::B8toB10] = true;
		coverage[1][HiLoControlFlowGraph::predicates::B8_F] = true;
		block6();
	}
}
void HiLoControlFlowGraph::block9() {
	coverage[0][HiLoControlFlowGraph::edges::B9toB10] = true;
	block10();
}
void HiLoControlFlowGraph::block10() {
	if (true /*testCase.guess != testCase.target*/)
	{
		if (true /* testCase.guess != 0 */)
		{
			coverage[1][HiLoControlFlowGraph::predicates::B10_TT] = true;
			//Its an 'and' condition, this is the only time we'll follow the true branch
			coverage[0][HiLoControlFlowGraph::edges::B10toB2] = true;
			block2();
		}
		else
		{
			coverage[1][HiLoControlFlowGraph::predicates::B10_TF] = true;
		}
	}
	else if (true /* testCase.guess != 0 */)
	{
		// We already know first part of the condition is false
		coverage[1][HiLoControlFlowGraph::predicates::B10_FT] = true;
	}
	else {
		// Both parts of the condition are false
		coverage[1][HiLoControlFlowGraph::predicates::B10_FF] = true;
	}
	// If we made it here we exit the loop and end the program
	return;
}

