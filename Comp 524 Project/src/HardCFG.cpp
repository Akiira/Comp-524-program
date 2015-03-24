/*
 * HardCFG.cpp
 *
 *  Created on: Mar 24, 2015
 *      Author: Kitty
 */

#include "HardCFG.h"
#include <limits>
#include <cassert>
#include <cmath>

using std::numeric_limits;

HardCFG::HardCFG() {
	//numberOfEdges = 28;
	//numberOfPredicates = 20;

	numberOfParameters = 10;

	testCase = NULL;

	for (int i = 0; i < numberOfParameters; ++i) {
		rangeForEachParameter[i][0] = numeric_limits<int>::min() / ( i + 2 );
		rangeForEachParameter[i][1] = numeric_limits<int>::max() / ( i + 2 );
	}

}

HardCFG::~HardCFG() {
	// TODO Auto-generated destructor stub
}

int HardCFG::getLowerBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][0];
}
int HardCFG::getUpperBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][1];
}

inline void HardCFG::runTestCase() {
	testCase->clearCoverage();
	calculateMean(testCase->getInputParameters());
	calculateDeviation(testCase->getInputParameters());
	calculateSums(testCase->getInputParameters());


	if( Mu <= 500 ) {
		testCase->addEdgeCoverage(edges::B0toB1);
		block1();
	} else {
		testCase->addEdgeCoverage(edges::B0toB2);
		block2();
	}
}

void HardCFG::block1() {
	//mu <= 500

	if( sigma < 100 ) {
		testCase->addEdgeCoverage(edges::B1toB3);
		block3();
	} else {
		testCase->addEdgeCoverage(edges::B1toB4);
		block4();
	}
}

void HardCFG::block2() {
	//mu > 500

	if( sigma > 100 ) {
		testCase->addEdgeCoverage(edges::B2toB5);
		block5();
	} else {
		testCase->addEdgeCoverage(edges::B2toB6);
		block6();
	}
}

void HardCFG::block3() {
	//mu <= 500 sigma < 100

	if( sigma < 50 ) {
		testCase->addEdgeCoverage(edges::B3toB7);
		block7();
	} else {
		testCase->addEdgeCoverage(edges::B3toB8);
		block8();
	}
}

void HardCFG::block4() {
	//mu <= 500 sigma >= 100

	if( sigma > 500 ) {
		testCase->addEdgeCoverage(edges::B4toB9);
		block9();
	} else {
		testCase->addEdgeCoverage(edges::B4toB10);
		block10();
	}
}

void HardCFG::block5() {
	//mu > 500 sigma > 100

	if( sigma > 500 ) {
		testCase->addEdgeCoverage(edges::B5toB11);
		block11();
	} else {
		testCase->addEdgeCoverage(edges::B5toB12);
		block12();
	}
}

void HardCFG::block6() {
	//mu > 500 sigma <= 100
	if( sigma < 50 ) {
		testCase->addEdgeCoverage(edges::B6toB13);
		block13();
	} else {
		testCase->addEdgeCoverage(edges::B6toB14);
		block14();
	}
}

void HardCFG::block7() {
	//mu <= 500 sigma < 50

	if( sigma < 25 ) {
		testCase->addEdgeCoverage(edges::B7toB15);
		block15();
	} else {
		testCase->addEdgeCoverage(edges::B7toB16);
		block16();
	}
}

void HardCFG::block8() {
	//mu <= 500 sigma >= 50 sigma < 100

	if( sigma > 75 ) {
		testCase->addEdgeCoverage(edges::B8toB17);
		block17();
	} else {
		testCase->addEdgeCoverage(edges::B8toB18);
		block18();
	}
}

void HardCFG::block9() {
	//mu <= 500 sigma > 500

	if( sigma > 1000 ) {
		testCase->addEdgeCoverage(edges::B9toB19);
		block19();
	} else {
		testCase->addEdgeCoverage(edges::B9toB20);
		block20();
	}
}

void HardCFG::block10() {
	//mu <= 500 sigma >= 100 sigma < 500
	if( sigma > 250 ) {
		testCase->addEdgeCoverage(edges::B10toB21);
		block21();
	} else {
		testCase->addEdgeCoverage(edges::B10toB22);
		block22();
	}
}

void HardCFG::block11() {
	//mu > 500 sigma > 500

	if( sigma > 1000 ) {
		testCase->addEdgeCoverage(edges::B11toB23);
		block23();
	} else {
		testCase->addEdgeCoverage(edges::B11toB24);
		block24();
	}
}

void HardCFG::block12() {
	//mu > 500 sigma > 100 sigma < 500

	if( sigma > 250 ) {
		testCase->addEdgeCoverage(edges::B12toB25);
		block25();
	} else {
		testCase->addEdgeCoverage(edges::B12toB26);
		block26();
	}
}

void HardCFG::block13() {
	//mu > 500 sigma <= 100
	if( sigma < 25 ) {
		testCase->addEdgeCoverage(edges::B13toB27);
		block27();
	} else {
		testCase->addEdgeCoverage(edges::B13toB28);
		block28();
	}
}

void HardCFG::block14() {
	if( sigma < 25 ) {
		testCase->addEdgeCoverage(edges::B14toB29);
		block29();
	} else {
		testCase->addEdgeCoverage(edges::B14toB29);
		block30();
	}
}

void HardCFG::block15() {

	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {

	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}

	}
}

void HardCFG::block16() {

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0) {

	} else if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0) {

	} else if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0) {

	} else {

	}
}

void HardCFG::block17() {
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {

	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}

	}
}

void HardCFG::block18() {
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0) {

	} else if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0) {

	} else {

	}
}

void HardCFG::block19() {
	//mu <= 500 sigma > 1000
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {

	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}

	}
}

void HardCFG::block20() {
	//mu <= 500 sigma > 500 sigma < 1000
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0) {

	} else if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0) {

	} else {

	}
}

void HardCFG::block21() {
	//mu <= 500 sigma > 250 sigma < 500
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {

	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}

	}
}

void HardCFG::block22() {
	//mu <= 500 sigma >= 100 sigma <= 250
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0) {

	} else if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0) {

	} else {

	}
}

void HardCFG::block23() {
	//mu > 500 sigma > 1000
}

void HardCFG::block24() {
	//mu > 500 sigma > 500 sigma < 1000
}

void HardCFG::block25() {
	//mu > 500 sigma > 250 sigma < 500
}

void HardCFG::block26() {
	//mu > 500 sigma > 100 sigma < 250
}

void HardCFG::block27() {
}

void HardCFG::block28() {
}

void HardCFG::block29() {
}

void HardCFG::block30() {
}

void HardCFG::calculateMean(const int* values) {
	long long sum { 0 };
	for (int i = 0; i < numberOfParameters; ++i) {
		sum += values[i];
	}

	Mu = ((double) sum / (double) numberOfParameters);
}

void HardCFG::calculateDeviation(const int* values) {
	long long sum { 0 };
	for (int i = 0; i < numberOfParameters; ++i) {

		sum += pow(values[i] - Mu, 2);
		assert(sum >= 0);
	}

	sigma = sqrt(sum / numberOfParameters);
	assert(sigma >= 0);
}



void HardCFG::calculateSums(const int* values) {
	sumOfAll = 0;
	sumOfFirstHalf = 0;
	sumOfSecondHalf = 0;

	for (int i = 0; i < numberOfParameters; ++i) {

		if( i < 5 ) {
			sumOfFirstHalf += values[i];
		} else {
			sumOfSecondHalf += values[i];
		}
	}
	sumOfAll = sumOfFirstHalf + sumOfSecondHalf;
}
