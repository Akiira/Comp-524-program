/*
 * RandomSearcher.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Kitty
 */

#ifndef RANDOMSEARCHER_H_
#define RANDOMSEARCHER_H_

#include <chrono>
class TestSuite;

class RandomSearcher {
public:
	RandomSearcher();
	virtual ~RandomSearcher();
	static void search(double seconds);
	static void updatePredicateCoverage(bool* preds, bool* newCoverage);
	static void updateEdgeCoverage(bool* edges, bool* newCoverage);
	static int getPredCount(bool* preds);
	static int getEdgeCount(bool* edges);

	static TestSuite* searchAndReturnSuite(double seconds);
};



#endif /* RANDOMSEARCHER_H_ */
