/*
 * HardCFG.cpp
 *
 *  Created on: Mar 24, 2015
 *      Author: Randall and Austin
 */

#include "MediumCFG.h"
#include <limits>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "TestCase.h"
using std::endl;
using std::cout;
using std::numeric_limits;

/*
 * Tweaks from hard graph: smaller ranges, from 10 parameters to 4, graph cut in half
 *  edges from 78 to 39 and predicates from 150 to 72
 */

MediumCFG::MediumCFG() {
	numberOfEdges = 39;
	numberOfPredicates = 71;

	numberOfParameters = 4;

	testCase = NULL;

	for (int i = 0; i < numberOfParameters; ++i) {
		//rangeForEachParameter[i][0] = numeric_limits<int>::min() / ( i + 6 );
		//rangeForEachParameter[i][1] = numeric_limits<int>::max() / ( i + 6 );

		rangeForEachParameter[i][0] = numeric_limits<int>::min();
		rangeForEachParameter[i][1] = numeric_limits<int>::max();
	}

	Mu = 0;
	sigma = 0;
	sumOfAll = 0;
	sumOfFirstHalf = 0;
	sumOfSecondHalf =0;
}

MediumCFG::~MediumCFG() {

}

int MediumCFG::getLowerBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][0];
}
int MediumCFG::getUpperBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][1];
}

inline void MediumCFG::runTestCase() {
	testCase->clearCoverage();
	calculateMean(testCase->getInputParameters());
	calculateDeviation(testCase->getInputParameters());
	calculateSums(testCase->getInputParameters());

	//TODO test all edges and predicates are actually reachable
	testCase->addEdgeCoverage(edges::B0toB2);
	testCase->addPredicateCoverage(predicates::B0_F);
	block2();

}

inline void MediumCFG::block2() {
	//mu > 500

	if( sigma > 1000 ) {
		testCase->addEdgeCoverage(edges::B2toB5);
		testCase->addPredicateCoverage(predicates::B2_T);
		block5();
	} else {
		testCase->addEdgeCoverage(edges::B2toB6);
		testCase->addPredicateCoverage(predicates::B2_F);
		block6();
	}
}

inline void MediumCFG::block5() {
	//mu > 500 sigma > 1000

	if( sigma > 5000 ) {
		testCase->addEdgeCoverage(edges::B5toB11);
		testCase->addPredicateCoverage(predicates::B5_T);
		block11();
	} else {
		testCase->addEdgeCoverage(edges::B5toB12);
		testCase->addPredicateCoverage(predicates::B5_F);
		block12();
	}
}

inline void MediumCFG::block6() {
	//mu > 500 sigma <= 1000
	if( sigma < 500 ) {
		testCase->addEdgeCoverage(edges::B6toB13);
		testCase->addPredicateCoverage(predicates::B6_T);
		block13();
	} else {
		testCase->addEdgeCoverage(edges::B6toB14);
		testCase->addPredicateCoverage(predicates::B6_F);
		block14();
	}
}



inline void MediumCFG::block11() {
	//mu > 500 sigma > 5000

	if( sigma > 10000 ) {
		testCase->addEdgeCoverage(edges::B11toB23);
		testCase->addPredicateCoverage(predicates::B11_T);
		block23();
	} else {
		testCase->addEdgeCoverage(edges::B11toB24);
		testCase->addPredicateCoverage(predicates::B11_F);
		block24();
	}
}

inline void MediumCFG::block12() {
	//mu > 500 sigma > 1000 sigma < 5000

	if( sigma > 2500 ) {
		testCase->addEdgeCoverage(edges::B12toB25);
		testCase->addPredicateCoverage(predicates::B12_T);
		block25();
	} else {
		testCase->addEdgeCoverage(edges::B12toB26);
		testCase->addPredicateCoverage(predicates::B12_F);
		block26();
	}
}

inline void MediumCFG::block13() {
	//mu > 500 sigma < 500
	if( sigma < 250 ) {
		testCase->addEdgeCoverage(edges::B13toB27);
		testCase->addPredicateCoverage(predicates::B13_T);
		block27();
	} else {
		testCase->addEdgeCoverage(edges::B13toB28);
		testCase->addPredicateCoverage(predicates::B13_F);
		block28();
	}
}

inline void MediumCFG::block14() {
	//mu > 500 sigma <= 1000 sigma > 500
	if( sigma < 750 ) {
		testCase->addEdgeCoverage(edges::B14toB29);
		testCase->addPredicateCoverage(predicates::B14_T);
		block29();
	} else {
		testCase->addEdgeCoverage(edges::B14toB30);
		testCase->addPredicateCoverage(predicates::B14_F);
		block30();
	}
}

inline void MediumCFG::block23() {
	//mu > 500 sigma > 10000
	int a1 = testCase->getParameter(0),
		a2 = testCase->getParameter(1),
		a3 = testCase->getParameter(2);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addPredicateCoverage(predicates::B23_TT);
		testCase->addEdgeCoverage(edges::B23toB55);
	} else {
		if( (a1 == a2) ) {
			testCase->addPredicateCoverage(predicates::B23_TF);
		} else if( (a2 == a3) ) {
			testCase->addPredicateCoverage(predicates::B23_TF);
		} else {
			testCase->addPredicateCoverage(predicates::B23_FF);
		}
		testCase->addEdgeCoverage(edges::B23toB56);
	}
}

inline void MediumCFG::block24() {
	//mu > 500 sigma > 5000 sigma < 10000

	if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000) {
		testCase->addEdgeCoverage(edges::B24toB57);
		testCase->addPredicateCoverage(predicates::B24A_TT);
	} else {
		if (sumOfFirstHalf == sumOfSecondHalf) {
			testCase->addPredicateCoverage(predicates::B24A_TF);
		} else if (sumOfAll < 0) {
			testCase->addPredicateCoverage(predicates::B24A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B24A_FF);
		}

		if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000) {
			testCase->addEdgeCoverage(edges::B24toB58);
			testCase->addPredicateCoverage(predicates::B24B_TT);
		} else {
			if (sumOfFirstHalf == sumOfSecondHalf) {
				testCase->addPredicateCoverage(predicates::B24B_TF);
			} else if (sumOfAll > 0) {
				testCase->addPredicateCoverage(predicates::B24B_FT);
			} else {
				testCase->addPredicateCoverage(predicates::B24B_FF);
			}
			if (sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B24toB59);
				testCase->addPredicateCoverage(predicates::B24C_T);
			} else {
				testCase->addPredicateCoverage(predicates::B24C_F);
				testCase->addEdgeCoverage(edges::B24toB60);
			}
		}
	}
}

inline void MediumCFG::block25() {
	//mu > 500 sigma > 2500 sigma < 5000
	int a1 = testCase->getParameter(0),
		a2 = testCase->getParameter(0),
		a3 = testCase->getParameter(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B25toB61);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B25toB62);
	}
}

inline void MediumCFG::block26() {
	//mu > 500 sigma > 1000 sigma < 2500

	if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000) {
		testCase->addEdgeCoverage(edges::B26toB63);
		testCase->addPredicateCoverage(predicates::B26A_TT);
	} else {
		if (sumOfFirstHalf == sumOfSecondHalf) {
			testCase->addPredicateCoverage(predicates::B26A_TF);
		} else if (sumOfAll < 0) {
			testCase->addPredicateCoverage(predicates::B26A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B26A_FF);
		}

		if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000) {
			testCase->addEdgeCoverage(edges::B26toB64);
			testCase->addPredicateCoverage(predicates::B26B_TT);
		} else {
			if (sumOfFirstHalf == sumOfSecondHalf) {
				testCase->addPredicateCoverage(predicates::B26B_TF);
			} else if (sumOfAll > 0) {
				testCase->addPredicateCoverage(predicates::B26B_FT);
			} else {
				testCase->addPredicateCoverage(predicates::B26B_FF);
			}
			if (sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B26toB65);
				testCase->addPredicateCoverage(predicates::B26C_T);
			} else {
				testCase->addPredicateCoverage(predicates::B26C_F);
				testCase->addEdgeCoverage(edges::B26toB66);
			}
		}
	}
}

inline void MediumCFG::block27() {
	//mu > 500 sigma < 250
	int a1 = testCase->getParameter(0),
		a2 = testCase->getParameter(1),
		a3 = testCase->getParameter(2);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B27toB67);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B27toB68);
	}
}

inline void MediumCFG::block28() {
	//mu > 500 sigma <= 100 sigma > 25

	if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000) {
			testCase->addEdgeCoverage(edges::B28toB69);
			testCase->addPredicateCoverage(predicates::B28A_TT);
		} else {
			if (sumOfFirstHalf == sumOfSecondHalf) {
				testCase->addPredicateCoverage(predicates::B28A_TF);
			} else if (sumOfAll < 0) {
				testCase->addPredicateCoverage(predicates::B28A_FT);
			} else {
				testCase->addPredicateCoverage(predicates::B28A_FF);
			}

			if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000) {
				testCase->addEdgeCoverage(edges::B28toB70);
				testCase->addPredicateCoverage(predicates::B28B_TT);
			} else {
				if (sumOfFirstHalf == sumOfSecondHalf) {
					testCase->addPredicateCoverage(predicates::B28B_TF);
				} else if (sumOfAll > 0) {
					testCase->addPredicateCoverage(predicates::B28B_FT);
				} else {
					testCase->addPredicateCoverage(predicates::B28B_FF);
				}
				if (sumOfFirstHalf <= sumOfSecondHalf) {
					testCase->addEdgeCoverage(edges::B28toB71);
					testCase->addPredicateCoverage(predicates::B28C_T);
				} else {
					testCase->addPredicateCoverage(predicates::B28C_F);
					testCase->addEdgeCoverage(edges::B28toB72);
				}
			}
		}
}

inline void MediumCFG::block29() {
	//mu > 500 sigma < 750 sigma > 500
	int a1 = testCase->getParameter(0),
		a2 = testCase->getParameter(1),
		a3 = testCase->getParameter(2);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B29toB73);
	} else {
		if( (a1 == a2) ) {
			testCase->addPredicateCoverage(predicates::B29_TF);
		} else if( (a2 == a3) ) {
			testCase->addPredicateCoverage(predicates::B29_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B29_FF);
		}
		testCase->addEdgeCoverage(edges::B29toB74);
	}
}

inline void MediumCFG::block30() {
	//mu > 500 sigma <= 1000 sigma >= 750

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000 ) {
		testCase->addEdgeCoverage(edges::B30toB75);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000 ) {
		   testCase->addEdgeCoverage(edges::B30toB76);
		} else {
			if ( sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B30toB77);
			} else {
				testCase->addEdgeCoverage(edges::B30toB78);
			}
		}
	}

	if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000) {
		testCase->addEdgeCoverage(edges::B30toB75);
		testCase->addPredicateCoverage(predicates::B30A_TT);
	} else {
		if (sumOfFirstHalf == sumOfSecondHalf) {
			testCase->addPredicateCoverage(predicates::B30A_TF);
		} else if (sumOfAll < 0) {
			testCase->addPredicateCoverage(predicates::B30A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B30A_FF);
		}

		if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000) {
			testCase->addEdgeCoverage(edges::B30toB76);
			testCase->addPredicateCoverage(predicates::B30B_TT);
		} else {
			if (sumOfFirstHalf == sumOfSecondHalf) {
				testCase->addPredicateCoverage(predicates::B30B_TF);
			} else if (sumOfAll > 0) {
				testCase->addPredicateCoverage(predicates::B30B_FT);
			} else {
				testCase->addPredicateCoverage(predicates::B30B_FF);
			}
			if (sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B30toB77);
				testCase->addPredicateCoverage(predicates::B30C_T);
			} else {
				testCase->addPredicateCoverage(predicates::B30C_F);
				testCase->addEdgeCoverage(edges::B30toB78);
			}
		}
	}
}

void MediumCFG::calculateMean(const int* values) {
	long long sum { 0 };
	for (int i = 0; i < numberOfParameters; ++i) {
		sum += values[i];
	}

	Mu = ((double) sum / (double) numberOfParameters);
}

void MediumCFG::calculateDeviation(const int* values) {
	long long sum { 0 };

	for (int i = 0; i < numberOfParameters; ++i) {
		sum += (pow(values[i] - Mu, 2) / numberOfParameters);
		assert(sum >= 0);
	}
	sigma = sqrt(sum);
	assert(sigma >= 0);
}



void MediumCFG::calculateSums(const int* values) {
	sumOfAll = 0;
	sumOfFirstHalf = 0;
	sumOfSecondHalf = 0;

	for (int i = 0; i < numberOfParameters; ++i) {

		if( i < numberOfParameters / 2 ) {
			sumOfFirstHalf += values[i];
		} else {
			sumOfSecondHalf += values[i];
		}
	}
	sumOfAll = sumOfFirstHalf + sumOfSecondHalf;
}

void MediumCFG::printInputParameters(int* inputParameters) const{
	cout << endl << "Input Parameters" << endl;
	cout << "     A        B        C        D        E        F        G        H        I        J" << endl;
	cout << "-----------------------------------------------" << endl;
	for (int i = 0; i < numberOfParameters; i++) {
		cout << "  " << std::setw(8) << inputParameters[i] << "  |";
	}
	cout << endl;
}

void MediumCFG::printEdgesCovered(bool* edgesCovered) const{
	cout << endl << "Edge Coverage" << endl;

	cout << "B0toB2-B2toB5-B2toB6-B5toB11-B5toB12-B6toB13-B6toB14-B11toB23," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 8; j++)
	{
		cout << "   " << edgesCovered[0 + j] << "   |";
	}
	cout << endl;
	cout << "B11toB24-B12toB25-B12toB26-B13toB27-B13toB28-B14toB29-B14toB30 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[8 + j] << "   |";
	}
	cout << endl;

	cout << "B23toB55, B23toB56, B24toB57, B24toB58, B24toB59, B24toB60," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[15 + j] << "   |";
	}
	cout << endl;
	cout << "B25toB61, B25toB62, B26toB63, B26toB64, B26toB65, B26toB66," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[21 + j] << "   |";
	}
	cout << endl;
	cout << "B27toB67, B27toB68, B28toB69, B28toB70, B28toB71, B28toB72," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[27 + j] << "   |";
	}
	cout << endl;
	cout << "B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[32 + j] << "   |";
	}
	cout << endl;
}

void MediumCFG::printPredicatesCovered(bool* predicatesCovered) const{
	cout << "\tTODO" << endl; //TODO printPredicatesCovered
}

void MediumCFG::printEdgesCovered(int* edgesCovered) const{

	cout << "B0toB2-B2toB5-B2toB6-B5toB11-B5toB12-B6toB13-B6toB14-B11toB23," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 8; j++)
	{
		cout << "   " << edgesCovered[0 + j] << "   |";
	}
	cout << endl;
	cout << "B11toB24-B12toB25-B12toB26-B13toB27-B13toB28-B14toB29-B14toB30 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[8 + j] << "   |";
	}
	cout << endl;

	cout << "B23toB55, B23toB56, B24toB57, B24toB58, B24toB59, B24toB60," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[15 + j] << "   |";
	}
	cout << endl;
	cout << "B25toB61, B25toB62, B26toB63, B26toB64, B26toB65, B26toB66," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[21 + j] << "   |";
	}
	cout << endl;
	cout << "B27toB67, B27toB68, B28toB69, B28toB70, B28toB71, B28toB72," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[27 + j] << "   |";
	}
	cout << endl;
	cout << "B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[33 + j] << "   |";
	}
	cout << endl;
}

void MediumCFG::printPredicatesCovered(int* predicatesCovered) const{
	cout << "\tTODO" << endl;//TODO printPredicatesCovered

}
