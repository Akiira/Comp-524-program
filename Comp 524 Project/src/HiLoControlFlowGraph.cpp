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
	testCase->addEdgeCoverage(edges::B1toB2);
	block2();
}


void HiLoControlFlowGraph::block2()  {
	// Since its a do-while loop, increment counter here.
	programVariables[LOOP_COUNTER]++;

	if (programVariables[GUESS] == programVariables[TARGET])
	{
		testCase->addEdgeCoverage(edges::B2toB3);
		testCase->addPredicateCoverage(predicates::B2_T);
		block3();
	}
	else
	{
		testCase->addEdgeCoverage(edges::B2toB4);
		testCase->addPredicateCoverage(predicates::B2_F);
		block4();
	}
}

void HiLoControlFlowGraph::block3()  {
	testCase->addEdgeCoverage(edges::B3toB10);
	block10();
}


void HiLoControlFlowGraph::block4()  {
	if (programVariables[GUESS] >programVariables[TARGET])
	{
		testCase->addEdgeCoverage(edges::B4toB5);
		testCase->addPredicateCoverage(predicates::B4_T);
		block5();
	}
	else
	{
		testCase->addEdgeCoverage(edges::B4toB6);
		testCase->addPredicateCoverage(predicates::B4_F);
		block6();
	}
}
void HiLoControlFlowGraph::block5()  {
	testCase->addEdgeCoverage(edges::B5toB10);
	block10();
}
void HiLoControlFlowGraph::block6()  {
	if (programVariables[GUESS] < programVariables[NUM1])
	{
		if (programVariables[GUESS] < programVariables[NUM2])
		{
			testCase->addPredicateCoverage(predicates::B6_TT);
		}
		else
		{
			testCase->addPredicateCoverage(predicates::B6_TF);
		}
		//Its an 'or' condition, follow true branch since first one was true
		testCase->addEdgeCoverage(edges::B6toB7);
		block7();
	}
	else if (programVariables[GUESS] < programVariables[NUM2])
	{
		// We already know first part of the condition is false
		testCase->addPredicateCoverage(predicates::B6_FT);
		testCase->addEdgeCoverage(edges::B6toB7);
		block7();
	}
	else {
		// Both parts of the condition are false
		testCase->addPredicateCoverage(predicates::B6_FF);
		testCase->addEdgeCoverage(edges::B6toB8);
		block8();
	}
}
void HiLoControlFlowGraph::block7()  {
	testCase->addEdgeCoverage(edges::B7toB10);
	block10();
}
void HiLoControlFlowGraph::block8() {
	if (programVariables[GUESS] != 0 )
	{
		testCase->addEdgeCoverage(edges::B8toB9);
		testCase->addPredicateCoverage(predicates::B8_T);
		block5();
	}
	else
	{
		testCase->addEdgeCoverage(edges::B8toB10);
		testCase->addPredicateCoverage(predicates::B8_F);
		block6();
	}
}
void HiLoControlFlowGraph::block9() {
	testCase->addEdgeCoverage(edges::B9toB10);
	block10();
}
void HiLoControlFlowGraph::block10() {
	if (programVariables[GUESS] != programVariables[TARGET])
	{
		if (programVariables[GUESS] != 0)
		{
			testCase->addPredicateCoverage(predicates::B10_TT);
			//Its an 'and' condition, this is the only time we'll follow the true branch
			testCase->addEdgeCoverage(edges::B10toB2);

			// Not sure about this but as a first thought allow the loop to run up to 10 times,
			//	each time with a good chance of picking the right guess.
			//	With this scheme it will be quite easy for the algorithm to get LOOP_MANY covered,
			//	not so easy to get LOOP_1, or LOOP_2

			// This program is a pretty bad example to use for loops since it actually takes input within the
			//	loop, meaning the number of input parameters is technically infinite. Using this random generation
			//	makes no sense without storing the sequence of guesses because a test case that got the coverage using
			// this has nothing to do with the human actually entering the parameters.

			// This is where it might make sense to have a subclass of testCase. That in addition to the three input parameters,
			//	will also allow you to store a list of (in this case up to 9) additional guesses as part of the test case.
			// 	That way the generated test case is completely reproducible by a human later.

			// Also each program/CFG will have different input boundaries that we'll want to take into account when generating
			//	the input parameters randomly/ through some local opt scheme. So either all the test case generation
			//	functions need to be in the CFG or there should be test case subclasses with different generation methods.

			/* For now just go through the loop once then quit like el ariss did. Need to figure this out.

			if (programVariables[LOOP_COUNTER] <= 9) {
				programVariables[GUESS] = uniformInRange(programVariables[TARGET]-5, programVariables[TARGET]+5);
				block2();
			}
			else {
				programVariables[GUESS] = programVariables[TARGET];
				block2();
			}
			*/
		}
		else
		{
			testCase->addPredicateCoverage(predicates::B10_TF);
		}
	}
	else if (programVariables[GUESS] != 0 )
	{
		// We already know first part of the condition is false
		testCase->addPredicateCoverage(predicates::B10_FT);
	}
	else {
		// Both parts of the condition are false
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

