/*
 * TriangleProblemGraph.cpp
 *
 *  Created on: Mar 21, 2015
 *      Author: Kitty
 */

#include "TriangleProblemGraph.h"
#include <cstring>
#include <cassert>
using std::endl;
using std::cout;

TriangleProblemGraph::~TriangleProblemGraph() {
	// TODO Auto-generated destructor stub
}

TriangleProblemGraph::TriangleProblemGraph() {
	numberOfEdges = 12;
	numberOfPredicates = 24;

	// Target isn't an input since its always num1 * num2
	numberOfParameters = 3;

	testCase = NULL;
	A = -666;
	B = -666;
	C = -666;
	rangeForEachParameter[0][0] = -100;
	rangeForEachParameter[0][1] = 10000;
	rangeForEachParameter[1][0] = -100;
	rangeForEachParameter[1][1] = 10000;
	rangeForEachParameter[2][0] = -100;
	rangeForEachParameter[2][1] = 10000;
}


int TriangleProblemGraph::getLowerBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][0];
}
int TriangleProblemGraph::getUpperBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][1];
}

void TriangleProblemGraph::runTestCase() {
	testCase->clearCoverage();

	A = testCase->getInputParameterAtIndex(0);
	B = testCase->getInputParameterAtIndex(1);
	C = testCase->getInputParameterAtIndex(2);

	if( A > 0 && B > 0 && C >0 ) {
		testCase->addEdgeCoverage(edges::B0toB2);
		testCase->addPredicateCoverage(predicates::B0_TTT);
		block2();
	} else {
		testCase->addEdgeCoverage(edges::B0toB1);

		if( A > 0 && B > 0 ) {
			testCase->addPredicateCoverage(predicates::B0_TTF);
		}
		else if( B > 0 && C >0 ) {
			testCase->addPredicateCoverage(predicates::B0_FTT);
		}
		else if( A > 0 && C >0 ) {
			testCase->addPredicateCoverage(predicates::B0_TFT);
		}
		else if( A > 0 ) {
			testCase->addPredicateCoverage(predicates::B0_TFF);
		}
		else if( B > 0 ) {
			testCase->addPredicateCoverage(predicates::B0_FTF);
		}
		else if( C > 0 ) {
			testCase->addPredicateCoverage(predicates::B0_FFT);
		} else {
			testCase->addPredicateCoverage(predicates::B0_FFF);
		}
	}
}

void TriangleProblemGraph::block2() {

	int sum { A + B + C };

	if( 2*A < sum && 2*B < sum && 2*C < sum ) {
		testCase->addEdgeCoverage(edges::B2toB4);
		block4();
	} else {
		testCase->addEdgeCoverage(edges::B2toB3);

		if( 2*A < sum && 2*B < sum ) {
			testCase->addPredicateCoverage(predicates::B2_TTF);
		}
		else if( 2*B < sum && 2*C < sum ) {
			testCase->addPredicateCoverage(predicates::B2_FTT);
		}
		else if( 2*A < sum && 2*C < sum ) {
			testCase->addPredicateCoverage(predicates::B2_TFT);
		}
		else if( 2*A < sum ) {
			testCase->addPredicateCoverage(predicates::B2_TFF);
		}
		else if( 2*B < sum ) {
			testCase->addPredicateCoverage(predicates::B2_FTF);
		}
		else if( 2*C < sum ) {
			testCase->addPredicateCoverage(predicates::B2_FFT);
		} else {
			testCase->addPredicateCoverage(predicates::B2_FFF);
		}
	}

}

void TriangleProblemGraph::block4() {

	if( A == B ) {
		testCase->addEdgeCoverage(edges::B4toB5);
		testCase->addPredicateCoverage(predicates::B4_T);
	} else {
		testCase->addEdgeCoverage(edges::B4toB8);
		testCase->addPredicateCoverage(predicates::B4_F);
	}
}
void TriangleProblemGraph::block5() {
	if( B == C ) {
		testCase->addEdgeCoverage(edges::B5toB6);
		testCase->addPredicateCoverage(predicates::B5_T);
	} else {
		testCase->addEdgeCoverage(edges::B5toB7);
		testCase->addPredicateCoverage(predicates::B5_F);
	}
}

void TriangleProblemGraph::block8() {
	if( A == C ) {
		testCase->addEdgeCoverage(edges::B8toB9);
		testCase->addPredicateCoverage(predicates::B8_T);
	} else {
		testCase->addEdgeCoverage(edges::B8toB10);
		testCase->addPredicateCoverage(predicates::B8_F);
	}
}

void TriangleProblemGraph::block10() {
	if( B == C ) {
		testCase->addEdgeCoverage(edges::B10toB11);
		testCase->addPredicateCoverage(predicates::B10_T);
	} else {
		testCase->addEdgeCoverage(edges::B10toB12);
		testCase->addPredicateCoverage(predicates::B10_F);
	}
}
