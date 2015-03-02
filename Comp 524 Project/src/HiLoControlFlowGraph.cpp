/*
 * HiLoControlFlowGraph.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: amb6470
 */
#include "HiLoControlFlowGraph.h"

//using namespace HiLoControlFlowGraph;
//using HiLoControlFlowGraph::predicates;

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
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B1toB2] = true;
	testCase->addEdgeCoverage(edges::B1toB2);
	block2();
}


void HiLoControlFlowGraph::block2() {
	if (true /*testCase.guess == testCase.target*/)
	{
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B2toB3] = true;
		testCase->addEdgeCoverage(edges::B2toB3);
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B2_T] = true;
		testCase->addPredicateCoverage(predicates::B2_T);
		block3();
	}
	else
	{
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B2toB4] = true;
		testCase->addEdgeCoverage(edges::B2toB4);
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B2_F] = true;
		testCase->addPredicateCoverage(predicates::B2_F);
		block4();
	}
}

void HiLoControlFlowGraph::block3() {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B3toB10] = true;
	testCase->addEdgeCoverage(edges::B3toB10);
	block10();
}


void HiLoControlFlowGraph::block4() {
	if (true /*testCase.guess > testCase.target*/)
	{
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B4toB5] = true;
		testCase->addEdgeCoverage(edges::B4toB5);
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B4_T] = true;
		testCase->addPredicateCoverage(predicates::B4_T);
		block5();
	}
	else
	{
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B4toB6] = true;
		testCase->addEdgeCoverage(edges::B4toB6);
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B4_F] = true;
		testCase->addPredicateCoverage(predicates::B4_F);
		block6();
	}
}
void HiLoControlFlowGraph::block5() {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B5toB10] = true;
	testCase->addEdgeCoverage(edges::B5toB10);
	block10();
}
void HiLoControlFlowGraph::block6() {
	if (true /*testCase.guess < testCase.num1*/)
	{
		if (true /*testCase.guess < testCase.num2*/)
		{
			//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_TT] = true;
			testCase->addPredicateCoverage(predicates::B6_TT);
		}
		else
		{
			//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_TF] = true;
			testCase->addPredicateCoverage(predicates::B6_TF);
		}
		//Its an 'or' condition, follow true branch since first one was true
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B6toB7] = true;
		testCase->addEdgeCoverage(edges::B6toB7);
		block7();
	}
	else if (true /*testCase.guess < testCase.num2*/)
	{
		// We already know first part of the condition is false
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_FT] = true;
		testCase->addPredicateCoverage(predicates::B6_FT);

		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B6toB7] = true;
		testCase->addEdgeCoverage(edges::B6toB7);
		block7();
	}
	else {
		// Both parts of the condition are false
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B6_FF] = true;
		testCase->addPredicateCoverage(predicates::B6_FF);

		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B6toB8] = true;
		testCase->addEdgeCoverage(edges::B6toB8);
		block8();
	}
}
void HiLoControlFlowGraph::block7() {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B7toB10] = true;
	testCase->addEdgeCoverage(edges::B7toB10);
	block10();
}
void HiLoControlFlowGraph::block8() {
	if (true /*testCase.guess != 0 */)
	{
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B8toB9] = true;
		testCase->addEdgeCoverage(edges::B8toB9);
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B8_T] = true;
		testCase->addPredicateCoverage(predicates::B8_T);
		block5();
	}
	else
	{
		//testCase->edgesCovered[HiLoControlFlowGraph::edges::B8toB10] = true;
		testCase->addEdgeCoverage(edges::B8toB10);
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B8_F] = true;
		testCase->addPredicateCoverage(predicates::B8_F);
		block6();
	}
}
void HiLoControlFlowGraph::block9() {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B9toB10] = true;
	testCase->addEdgeCoverage(edges::B9toB10);
	block10();
}
void HiLoControlFlowGraph::block10() {
	if (true /*testCase.guess != testCase.target*/)
	{
		if (true /* testCase.guess != 0 */)
		{
			//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_TT] = true;
			testCase->addPredicateCoverage(predicates::B10_TT);
			//Its an 'and' condition, this is the only time we'll follow the true branch
			//testCase->edgesCovered[HiLoControlFlowGraph::edges::B10toB2] = true;
			testCase->addEdgeCoverage(edges::B10toB2);
			block2();
		}
		else
		{
			//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_TF] = true;
			testCase->addPredicateCoverage(predicates::B10_TF);
		}
	}
	else if (true /* testCase.guess != 0 */)
	{
		// We already know first part of the condition is false
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_FT] = true;
		testCase->addPredicateCoverage(predicates::B10_FT);
	}
	else {
		// Both parts of the condition are false
		//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_FF] = true;
		testCase->addPredicateCoverage(HiLoControlFlowGraph::predicates::B10_FF);
	}
	// If we made it here we exit the loop and end the program
	return;
}

