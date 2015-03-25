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

using std::endl;
using std::cout;
using std::numeric_limits;

HardCFG::HardCFG() {
	numberOfEdges = 79;
	numberOfPredicates = 90;

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

	//TODO added missing edge coverages
	//TODO add all predicates coverage
	//TODO test all edges and predicates are actually reachable

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
	//cout << "\t" << sigma << endl;
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
	//mu > 500 sigma <= 100 sigma > 50
	if( sigma < 75 ) {
		testCase->addEdgeCoverage(edges::B14toB29);
		block29();
	} else {
		testCase->addEdgeCoverage(edges::B14toB29);
		block30();
	}
}

void HardCFG::block15() {
	//mu <= 500 sigma < 25
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B15toB31);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B15toB32);
	}
}

void HardCFG::block16() {
	//mu <= 500 sigma < 50 sigma > 25
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B16toB33);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
			testCase->addEdgeCoverage(edges::B16toB34);
		} else {
			if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
				testCase->addEdgeCoverage(edges::B16toB35);
			} else {
				testCase->addEdgeCoverage(edges::B16toB36);
			}
		}
	}
}

void HardCFG::block17() {
	//mu <= 500 sigma >= 75 sigma < 100
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B17toB37);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B17toB38);
	}
}

void HardCFG::block18() {
	//mu <= 500 sigma >= 50 sigma <= 75
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B18toB39);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
			testCase->addEdgeCoverage(edges::B18toB40);
		} else {
			if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
				testCase->addEdgeCoverage(edges::B18toB41);
			} else {
				testCase->addEdgeCoverage(edges::B18toB42);
			}
		}
	}
}

void HardCFG::block19() {
	//mu <= 500 sigma > 1000
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B19toB43);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}

		testCase->addEdgeCoverage(edges::B19toB44);
	}
}

void HardCFG::block20() {
	//mu <= 500 sigma > 500 sigma < 1000

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B20toB45);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
		   testCase->addEdgeCoverage(edges::B20toB46);
		} else {
			if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
				testCase->addEdgeCoverage(edges::B20toB47);
			} else {
				testCase->addEdgeCoverage(edges::B20toB48);
			}
		}
	}
}

void HardCFG::block21() {
	//mu <= 500 sigma > 250 sigma < 500
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B21toB49);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B21toB50);
	}
}

void HardCFG::block22() {
	//mu <= 500 sigma >= 100 sigma <= 250

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B22toB51);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
		   testCase->addEdgeCoverage(edges::B22toB52);
		} else {
			if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
				testCase->addEdgeCoverage(edges::B22toB53);
			} else {
				testCase->addEdgeCoverage(edges::B22toB54);
			}
		}
	}
}
//-----------------------------------------
void HardCFG::block23() {
	//mu > 500 sigma > 1000
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B23toB55);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B23toB56);
	}
}

void HardCFG::block24() {
	//mu > 500 sigma > 500 sigma < 1000

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000 ) {
		testCase->addEdgeCoverage(edges::B24toB57);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000 ) {
		   testCase->addEdgeCoverage(edges::B24toB58);
		} else {
			if ( sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B24toB59);
			} else {
				testCase->addEdgeCoverage(edges::B24toB60);
			}
		}
	}
}

void HardCFG::block25() {
	//mu > 500 sigma > 250 sigma < 500
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

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

void HardCFG::block26() {
	//mu > 500 sigma > 100 sigma < 250

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000 ) {
		testCase->addEdgeCoverage(edges::B26toB63);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000 ) {
		   testCase->addEdgeCoverage(edges::B26toB64);
		} else {
			if ( sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B26toB65);
			} else {
				testCase->addEdgeCoverage(edges::B26toB66);
			}
		}
	}
}

void HardCFG::block27() {
	//mu > 500 sigma < 25
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

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

void HardCFG::block28() {
	//mu > 500 sigma <= 100 sigma > 25
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 10000000 ) {
		testCase->addEdgeCoverage(edges::B28toB69);
	} else {
	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > -10000 ) {
		   testCase->addEdgeCoverage(edges::B28toB70);
		} else {
			if ( sumOfFirstHalf <= sumOfSecondHalf) {
				testCase->addEdgeCoverage(edges::B28toB71);
			} else {
				testCase->addEdgeCoverage(edges::B28toB72);
			}
		}
	}
}

void HardCFG::block29() {
	//mu > 500 sigma < 75 sigma > 50
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B29toB73);
	} else {
		if( (a1 == a2) ) {

		} else if( (a2 == a3) ) {

		} else {

		}
		testCase->addEdgeCoverage(edges::B29toB74);
	}
}

void HardCFG::block30() {
	//mu > 500 sigma <= 100 sigma >= 75

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

void HardCFG::printInputParameters(int* inputParameters) const{
	cout << endl << "Input Parameters" << endl;
	cout << " A    B    C    D    E    F    G    H    I    J" << endl;
	cout << "------------------" << endl;
	for (int i = 0; i < numberOfParameters; i++) {
		cout << "  " << inputParameters[i] << "  |";
	}
	cout << endl;
}

void HardCFG::printEdgesCovered(bool* edgesCovered) const{
	cout << endl << "Edge Coverage" << endl;
	cout << " B0toB1-B1toB3-B1toB4-B3toB7-B3toB8-B4toB9-B4toB10-B7toB15," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for (int j = 0; j < 8; j++) {
		cout << "   " << edgesCovered[j] << "   |";
	}
	cout << endl;
	cout << "B7toB16-B8toB17-B8toB18-B9toB19-B9toB20-B10toB21-B10toB22" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[8 + j] << "   |";
	}
	cout << endl;
	cout << "B0toB2-B2toB5-B2toB6-B5toB11-B5toB12-B6toB13-B6toB14-B11toB23," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 8; j++)
	{
		cout << "   " << edgesCovered[15 + j] << "   |";
	}
	cout << endl;
	cout << "B11toB24-B12toB25-B12toB26-B13toB27-B13toB28-B14toB29-B14toB30 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[23 + j] << "   |";
	}
	cout << endl;
}

void HardCFG::printPredicatesCovered(bool* predicatesCovered) const{
	cout << "\tTEST1" << endl;
	cout << endl << "Predicate Coverage" << endl;
	cout << " B0_TTT | B0_TTF | B0_TFT | B0_FTT | B0_TFF | B0_FFT | B0_FTF | B0_FFF | B2_TTT | B2_TTF | B2_TFT | B2_FTT | B2_TFF | B2_FFT | B2_FTF | B2_FFF | B4_T | B4_F | B5_T | B5_F | B8_T | B8_F | B10_T | B10_F" << endl;
	cout << "-------------------------------------------------------------------------------------------------" << endl;
	for(int i = 0; i < numberOfPredicates; i++)
	{
		cout << "  " << predicatesCovered[i] << "   |";
	}
	cout << endl;
}

void HardCFG::printEdgesCovered(int* edgesCovered) const{
	cout << "\tTEST2" << endl;
	cout << endl << "Edge Coverage" << endl;
	cout << " B0toB1 | B1toB3 | 1toB4 | 3toB7 | 3toB8 | 4toB9 | 4toB10 | 7toB15," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for (int j = 0; j < 8; j++) {
		cout << "   " << edgesCovered[j] << "   |";
	}
	cout << endl;
	cout << " B7toB16 | B8toB17 | B8toB18 | B9toB19 | B9toB20 | B10toB21 | B10toB22, " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[8 + j] << "   |";
	}
	cout << endl;
	cout << " B0toB2 | B2toB5 | B2toB6 | B5toB11 | B5toB12 | B6toB13 | B6toB14 | B11toB23," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 8; j++)
	{
		cout << "   " << edgesCovered[15 + j] << "   |";
	}
	cout << endl;
	cout << " B11toB24 | B12toB25 | B12toB26 | B13toB27 | B13toB28 | B14toB29 | B14toB30 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[23 + j] << "   |";
	}
	cout << endl;
}

void HardCFG::printPredicatesCovered(int* predicatesCovered) const{
	cout << "\tTEST3" << endl;
	cout << endl << "Predicate Coverage" << endl;
	cout << " B0_TTT | B0_TTF | B0_TFT | B0_FTT | B0_TFF | B0_FFT | B0_FTF | B0_FFF | B2_TTT | B2_TTF | B2_TFT | B2_FTT | B2_TFF | B2_FFT | B2_FTF | B2_FFF | B4_T | B4_F | B5_T | B5_F | B8_T | B8_F | B10_T | B10_F" << endl;
	cout << "-------------------------------------------------------------------------------------------------" << endl;
	for(int i = 0; i < numberOfPredicates; i++)
	{
		cout << "  " << predicatesCovered[i] << "   |";
	}
	cout << endl;
}
