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
#include "TestSuite.h"
#include <chrono>

using std::cout;
using std::endl;

void RandomSearcher::search(double seconds) {
	int numOfEdges = targetCFG->getNumberOfEdges();
	auto edges = new bool[numOfEdges];
	int numOfPreds = targetCFG->getNumberOfPredicates();
	auto preds = new bool[numOfPreds];

	auto start = chrono::system_clock::now();

	do {
		auto test = TestCase::getRandomTestCase();
		targetCFG->setCoverageOfTestCase(test);

		RandomSearcher::updateEdgeCoverage(edges, test->getEdgesCovered());
		RandomSearcher::updatePredicateCoverage(preds, test->getPredicatesCovered());

		delete test;

	    chrono::duration<double> diff = chrono::system_clock::now()-start;

		if ( diff.count() - seconds >= 0 ) {
			break;
		}
	} while( true );

	cout << "\nEdges: " << getEdgeCount(edges) << "/" << targetCFG->getNumberOfEdges() << "     ";
	cout << "Edge Coverage Ratio: " << ((double)getEdgeCount(edges) / (double)targetCFG->getNumberOfEdges()) << endl;
	cout << "Preds: " << getPredCount(preds) << "/" << targetCFG->getNumberOfPredicates() << "     ";
	cout << "Preds Coverage Ratio: " << ((double)getPredCount(preds) / (double)targetCFG->getNumberOfPredicates()) << endl;
	cout << "Overall Coverage Ratio: " << (double)(getEdgeCount(edges) + getPredCount(preds)) / (double)(targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates()) << endl;
	cout << "\n\n";
}


TestSuite* RandomSearcher::searchAndReturnSuite(double seconds) {
	// Start as blank test suite. Grow whenever rangeSet generates a test case that covers something new.
	int edgesPlusPred = targetCFG->getNumberOfEdges() + targetCFG->getNumberOfPredicates();
	TestSuite* finalTestSuite = new TestSuite(0, edgesPlusPred, new TestCase*[edgesPlusPred]);

	auto start = chrono::system_clock::now();

	do {
		auto test = TestCase::getRandomTestCase();

		if (finalTestSuite->isCoveringNew(test)) {
			finalTestSuite->addTestCase(new TestCase(*test));
			finalTestSuite->calculateTestSuiteCoverage();
		}

		delete test;

		chrono::duration<double> diff = chrono::system_clock::now()-start;

		if ( diff.count() - seconds >= 0 ) {
			break;
		}
	} while( true );

	return finalTestSuite;
}

void RandomSearcher::updatePredicateCoverage(bool* preds, bool* newCoverage) {
	for(int i = 0; i < targetCFG->getNumberOfPredicates(); i++) {
		if ( newCoverage[i] ) {
			preds[i] = true;
		}
	}
}

void RandomSearcher::updateEdgeCoverage(bool* edges, bool* newCoverage) {
	for(int i = 0; i < targetCFG->getNumberOfEdges(); i++) {
		if ( newCoverage[i] ) {
			edges[i] = true;
		}
	}
}
int RandomSearcher::getPredCount(bool* preds) {
	int count = 0;
	for(int i = 0; i < targetCFG->getNumberOfEdges(); i++) {
		if ( preds[i] ) {
			count++;
		}
	}
	return count;
}
int RandomSearcher::getEdgeCount(bool* edges) {
	int count = 0;
	for(int i = 0; i < targetCFG->getNumberOfEdges(); i++) {
		if ( edges[i] ) {
			count++;
		}
	}
	return count;
}
