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

	// Target isn't an input since its always num1 * num2
	numberOfParameters = 3;

	testCase = 0;

	numberOfProgramVariables = 5;
	programVariables = new int[numberOfProgramVariables];
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
	int* inputParameters = testCase->getInputParameters();
	programVariables[NUM1] = inputParameters[NUM1];
	programVariables[NUM2] = inputParameters[NUM2];
	programVariables[GUESS] = inputParameters[GUESS];
	programVariables[TARGET] = programVariables[NUM1] * programVariables[NUM2];
	programVariables[LOOP_COUNTER] = 0;
	block1();
}

void HiLoControlFlowGraph::block1() {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B1toB2] = true;
	testCase->addEdgeCoverage(edges::B1toB2);
	block2();
}


void HiLoControlFlowGraph::block2()  {
	// Since its a do-while loop, increment counter here.
	programVariables[LOOP_COUNTER]++;

	if (programVariables[GUESS] == programVariables[TARGET])
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

void HiLoControlFlowGraph::block3()  {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B3toB10] = true;
	testCase->addEdgeCoverage(edges::B3toB10);
	block10();
}


void HiLoControlFlowGraph::block4()  {
	if (programVariables[GUESS] >programVariables[TARGET])
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
void HiLoControlFlowGraph::block5()  {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B5toB10] = true;
	testCase->addEdgeCoverage(edges::B5toB10);
	block10();
}
void HiLoControlFlowGraph::block6()  {
	if (programVariables[GUESS] < programVariables[NUM1])
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
	else if (programVariables[GUESS] < programVariables[NUM2])
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
void HiLoControlFlowGraph::block7()  {
	//testCase->edgesCovered[HiLoControlFlowGraph::edges::B7toB10] = true;
	testCase->addEdgeCoverage(edges::B7toB10);
	block10();
}
void HiLoControlFlowGraph::block8() {
	if (programVariables[GUESS] != 0 )
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
	if (programVariables[GUESS] != programVariables[TARGET])
	{
		if (programVariables[GUESS] != 0)
		{
			//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_TT] = true;
			testCase->addPredicateCoverage(predicates::B10_TT);
			//Its an 'and' condition, this is the only time we'll follow the true branch
			//testCase->edgesCovered[HiLoControlFlowGraph::edges::B10toB2] = true;
			testCase->addEdgeCoverage(edges::B10toB2);


			// Not sure about this but as a first thought allow the loop to run up to 10 times,
			//	each time with a good chance of picking the right guess.
			//	With this scheme it will be quite easy for the algorithm to get LOOP_MANY covered,
			//	not so easy to get LOOP_1, or LOOP_2

			// This program is a pretty bad example to use for loops since it actually takes input within the
			//	loop, meaning the number of input parameters is technically infinite. Using this random generation
			//	makes no sense because a test case that got the coverage using this has nothing to do with the
			//	human actually entering the parameters.

			// This is where it might make sense to have a subclass of testCase. That in addition to the three input parameters,
			//	will also allow you to store a list of (in this case up to 9) additional guesses as part of the test case.
			// 	That way the generated test case is completely reproducible by a human later.
			if (programVariables[LOOP_COUNTER] < 10) {
				programVariables[GUESS] = uniformInRange(programVariables[TARGET]-5, programVariables[TARGET]+5);
				block2();
			}

		}
		else
		{
			//testCase->predicatesCovered[HiLoControlFlowGraph::predicates::B10_TF] = true;
			testCase->addPredicateCoverage(predicates::B10_TF);
		}
	}
	else if (programVariables[GUESS] != 0 )
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

	// Set loop coverage based on how many times the loop was executed
	//TODO Add a loop coverage array to the test case class.
	if (programVariables[LOOP_COUNTER] == 1) {
		//testCase->addLoopCoverage(loops::LOOP_1);
	} else if (programVariables[LOOP_COUNTER] == 2) {
		//testCase->addLoopCoverage(loops::LOOP_2);
	}
	else {
		//testCase->addLoopCoverage(loops::LOOP_MANY);
	}

	// If we made it here we exited the loop and end the program
	return;
}

