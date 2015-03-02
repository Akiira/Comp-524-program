/*
 * HiLoControlFlowGraph.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: amb6470
 */
#include "HiLoControlFlowGraph.h"

HiLoControlFlowGraph::HiLoControlFlowGraph() {
	numberOfEdges = 16;
	numberOfPredicates = 14;

	testCase = 0;

}

int HiLoControlFlowGraph::getNumberOfEdges() {
	return numberOfEdges;
}
int HiLoControlFlowGraph::getNumberOfPredicates() {
	return numberOfPredicates;
}
int HiLoControlFlowGraph::getNumberOfParameters() {
	return numberOfParameters;
}

HiLoControlFlowGraph::~HiLoControlFlowGraph() {
	// Just implicitly call ControlFlowGraph destructor
}


void HiLoControlFlowGraph::runTestCase() {
	testCase->clearCoverage();
	block1();
}

void HiLoControlFlowGraph::block1() {
	testCase->edgesCovered[HiLoControlFlowGraph::edges::B1toB2] = true;
	block2();
}


void HiLoControlFlowGraph::block2() {
	if (true /*testCase.guess == testCase.target*/)
	{
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B2toB3] = true;
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B2_T] = true;
		block3();
	}
	else
	{
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B2toB4] = true;
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B2_F] = true;
		block4();
	}
}

void HiLoControlFlowGraph::block3() {
	testCase->edgesCovered[HiLoControlFlowGraph::edges::B3toB10] = true;
	block10();
}


void HiLoControlFlowGraph::block4() {
	if (true /*testCase.guess > testCase.target*/)
	{
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B4toB5] = true;
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B4_T] = true;
		block5();
	}
	else
	{
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B4toB6] = true;
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B4_F] = true;
		block6();
	}
}
void HiLoControlFlowGraph::block5() {
	testCase->edgesCovered[HiLoControlFlowGraph::edges::B5toB10] = true;
	block10();
}
void HiLoControlFlowGraph::block6() {
	if (true /*testCase.guess < testCase.num1*/)
	{
		if (true /*testCase.guess < testCase.num2*/)
		{
			testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_TT] = true;
		}
		else
		{
			testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_TF] = true;
		}
		//Its an 'or' condition, follow true branch since first one was true
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B6toB7] = true;
		block7();
	}
	else if (true /*testCase.guess < testCase.num2*/)
	{
		// We already know first part of the condition is false
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_FT] = true;

		testCase->edgesCovered[HiLoControlFlowGraph::edges::B6toB7] = true;
		block7();
	}
	else {
		// Both parts of the condition are false
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_FF] = true;

		testCase->edgesCovered[HiLoControlFlowGraph::edges::B6toB8] = true;
		block8();
	}
}
void HiLoControlFlowGraph::block7() {
	testCase->edgesCovered[HiLoControlFlowGraph::edges::B7toB10] = true;
	block10();
}
void HiLoControlFlowGraph::block8() {
	if (true /*testCase.guess != 0 */)
	{
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B8toB9] = true;
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B8_T] = true;
		block5();
	}
	else
	{
		testCase->edgesCovered[HiLoControlFlowGraph::edges::B8toB10] = true;
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B8_F] = true;
		block6();
	}
}
void HiLoControlFlowGraph::block9() {
	testCase->edgesCovered[HiLoControlFlowGraph::edges::B9toB10] = true;
	block10();
}
void HiLoControlFlowGraph::block10() {
	if (true /*testCase.guess != testCase.target*/)
	{
		if (true /* testCase.guess != 0 */)
		{
			testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_TT] = true;
			//Its an 'and' condition, this is the only time we'll follow the true branch
			testCase->edgesCovered[HiLoControlFlowGraph::edges::B10toB2] = true;
			block2();
		}
		else
		{
			testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_TF] = true;
		}
	}
	else if (true /* testCase.guess != 0 */)
	{
		// We already know first part of the condition is false
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_FT] = true;
	}
	else {
		// Both parts of the condition are false
		testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_FF] = true;
	}
	// If we made it here we exit the loop and end the program
	return;
}

