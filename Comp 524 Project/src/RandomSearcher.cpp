/*
 * RandomSearcher.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: Kitty
 */

#include "RandomSearcher.h"
#include "Random.h"
#include "GlobalVariables.h"
#include "ControlFlowGraph.h"
#include "TestCase.h"
#include <iostream>
using std::cout;
using std::endl;
RandomSearcher::RandomSearcher() {
	// TODO Auto-generated constructor stub

}

RandomSearcher::~RandomSearcher() {
	// TODO Auto-generated destructor stub
}

void RandomSearcher::search() {
	int numOfEdges = targetCFG->getNumberOfEdges();
	auto edges = new bool[numOfEdges];
	auto attempts = 1;
	do {
		auto test = TestCase::getRandomTestCase();
		targetCFG->setCoverageOfTestCase(test);

		auto temp = test->getEdgesCovered();

		for(int i = 0; i < numOfEdges; i++) {
			if ( temp[i] ) {
				edges[i] = true;
			}
		}

		auto t = true;

		for(int i = 0; i < numOfEdges; i++) {
			if(edges[i] == false)
				t = false;
		}

		delete test;

		if(t)
			break;
		attempts++;
	} while( true );

	cout << "All edges covered. It took " << attempts << " random guesses." << endl;
}

void RandomSearcher::searchForPredicates() {
	int numOfPreds = targetCFG->getNumberOfPredicates();
	auto preds = new bool[numOfPreds];
	long attempts { 1 };
	do {
		auto test = TestCase::getRandomTestCase();
		targetCFG->setCoverageOfTestCase(test);

		auto temp = test->getPredicatesCovered();

		for(int i = 0; i < numOfPreds; i++) {
			if ( temp[i] ) {
				preds[i] = true;
			}
		}

		auto t = true;

		for(int i = 0; i < numOfPreds; i++) {
			if(preds[i] == false)
				t = false;
		}

		delete test;

		if(t)
			break;
		attempts++;
	} while( true );

	cout << "All predicates covered. It took " << attempts << " random guesses." << endl;
}
