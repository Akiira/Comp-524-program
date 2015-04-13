/*
 * HardCFG.cpp
 *
 *  Created on: Mar 24, 2015
 *      Author: Randall and Austin
 */

#include "MedHardCFG.h"
#include <limits>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "TestCase.h"
using std::endl;
using std::cout;
using std::numeric_limits;

MedHardCFG::MedHardCFG() {
	numberOfEdges = 39;
	numberOfPredicates = 90;

	numberOfParameters = 5;

	testCase = NULL;

	for (int i = 0; i < numberOfParameters; ++i) {
		rangeForEachParameter[i][0] = numeric_limits<int>::min() / ( i + 6 );
		rangeForEachParameter[i][1] = numeric_limits<int>::max() / ( i + 6 );
	}

	Mu = 0;
	sigma = 0;
	sumOfAll = 0;
	sumOfFirstHalf = 0;
	sumOfSecondHalf =0;
}

MedHardCFG::~MedHardCFG() {

}

int MedHardCFG::getLowerBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][0];
}
int MedHardCFG::getUpperBoundForParameter(int parameter) {
	assert(parameter >= 0 && parameter < numberOfParameters);
	return rangeForEachParameter[parameter][1];
}

inline void MedHardCFG::runTestCase() {
	testCase->clearCoverage();
	calculateMean(testCase->getInputParameters());
	calculateDeviation(testCase->getInputParameters());
	calculateSums(testCase->getInputParameters());

	//TODO test all edges and predicates are actually reachable

	testCase->addEdgeCoverage(edges::B0toB1);
	block1();
}

inline void MedHardCFG::block1() {
	//mu <= 500

	if( sigma < 100 ) {
		testCase->addEdgeCoverage(edges::B1toB3);
		testCase->addPredicateCoverage(predicates::B1_T);
		block3();
	} else {
		testCase->addEdgeCoverage(edges::B1toB4);
		testCase->addPredicateCoverage(predicates::B1_F);
		block4();
	}
}

inline void MedHardCFG::block3() {
	//mu <= 500 sigma < 100

	if( sigma < 50 ) {
		testCase->addEdgeCoverage(edges::B3toB7);
		testCase->addPredicateCoverage(predicates::B3_T);
		block7();
	} else {
		testCase->addEdgeCoverage(edges::B3toB8);
		testCase->addPredicateCoverage(predicates::B3_F);
		block8();
	}
}

inline void MedHardCFG::block4() {
	//mu <= 500 sigma >= 100

	if( sigma > 500 ) {
		testCase->addEdgeCoverage(edges::B4toB9);
		testCase->addPredicateCoverage(predicates::B4_T);
		block9();
	} else {
		testCase->addEdgeCoverage(edges::B4toB10);
		testCase->addPredicateCoverage(predicates::B4_F);
		block10();
	}
}

inline void MedHardCFG::block7() {
	//mu <= 500 sigma < 50

	if( sigma < 25 ) {
		testCase->addEdgeCoverage(edges::B7toB15);
		testCase->addPredicateCoverage(predicates::B7_T);
		block15();
	} else {
		testCase->addEdgeCoverage(edges::B7toB16);
		testCase->addPredicateCoverage(predicates::B7_F);
		block16();
	}
}

inline void MedHardCFG::block8() {
	//mu <= 500 sigma >= 50 sigma < 100

	if( sigma > 75 ) {
		testCase->addEdgeCoverage(edges::B8toB17);
		testCase->addPredicateCoverage(predicates::B8_T);
		block17();
	} else {
		testCase->addEdgeCoverage(edges::B8toB18);
		testCase->addPredicateCoverage(predicates::B8_F);
		block18();
	}
}

inline void MedHardCFG::block9() {
	//mu <= 500 sigma > 500

	if( sigma > 1000 ) {
		testCase->addEdgeCoverage(edges::B9toB19);
		testCase->addPredicateCoverage(predicates::B9_T);
		block19();
	} else {
		testCase->addEdgeCoverage(edges::B9toB20);
		testCase->addPredicateCoverage(predicates::B9_F);
		block20();
	}
}

inline void MedHardCFG::block10() {
	//mu <= 500 sigma >= 100 sigma < 500
	if( sigma > 250 ) {
		testCase->addEdgeCoverage(edges::B10toB21);
		testCase->addPredicateCoverage(predicates::B10_T);
		block21();
	} else {
		testCase->addEdgeCoverage(edges::B10toB22);
		testCase->addPredicateCoverage(predicates::B10_F);
		block22();
	}
}


inline void MedHardCFG::block15() {
	//mu <= 500 sigma < 25
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B15toB31);
		testCase->addPredicateCoverage(predicates::B15_TT);
	} else {
		if( (a1 == a2) ) {
			testCase->addPredicateCoverage(predicates::B15_TF);
		} else if( (a2 == a3) ) {
			testCase->addPredicateCoverage(predicates::B15_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B15_FF);
		}
		testCase->addEdgeCoverage(edges::B15toB32);
	}
}

inline void MedHardCFG::block16() {
	//mu <= 500 sigma < 50 sigma > 25
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B16toB33);
		testCase->addPredicateCoverage(predicates::B16A_TT);
	} else {

		if( sumOfFirstHalf == sumOfSecondHalf ) {
			testCase->addPredicateCoverage(predicates::B16A_TF);
		} else if ( sumOfAll < 0 ) {
			testCase->addPredicateCoverage(predicates::B16A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B16A_FF);
		}

	   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
			testCase->addEdgeCoverage(edges::B16toB34);
			testCase->addPredicateCoverage(predicates::B16B_TT);
		} else {
			if( sumOfFirstHalf == sumOfSecondHalf ) {
				testCase->addPredicateCoverage(predicates::B16B_TF);
			} else if ( sumOfAll > 0 ) {
				testCase->addPredicateCoverage(predicates::B16B_FT);
			} else {
				testCase->addPredicateCoverage(predicates::B16B_FF);
			}
			if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
				testCase->addEdgeCoverage(edges::B16toB35);
				testCase->addPredicateCoverage(predicates::B16C_TT);
			} else {
				if( sumOfFirstHalf == sumOfSecondHalf ) {
					testCase->addPredicateCoverage(predicates::B16C_TF);
				} else if ( sumOfAll > 0 ) {
					testCase->addPredicateCoverage(predicates::B16C_FT);
				} else {
					testCase->addPredicateCoverage(predicates::B16C_FF);
				}
				testCase->addEdgeCoverage(edges::B16toB36);
			}
		}
	}
}

inline void MedHardCFG::block17() {
	//mu <= 500 sigma >= 75 sigma < 100
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B17toB37);
		testCase->addPredicateCoverage(predicates::B17_TT);
	} else {
		if( (a1 == a2) ) {
			testCase->addPredicateCoverage(predicates::B17_TF);
		} else if( (a2 == a3) ) {
			testCase->addPredicateCoverage(predicates::B17_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B17_FF);
		}
		testCase->addEdgeCoverage(edges::B17toB37);
	}
}

inline void MedHardCFG::block18() {
	//mu <= 500 sigma >= 50 sigma <= 75
	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B18toB39);
		testCase->addPredicateCoverage(predicates::B18A_TT);
	} else {
		if( sumOfFirstHalf == sumOfSecondHalf ) {
			testCase->addPredicateCoverage(predicates::B18A_TF);
		} else if ( sumOfAll < 0 ) {
			testCase->addPredicateCoverage(predicates::B18A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B18A_FF);
		}

		   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
				testCase->addEdgeCoverage(edges::B18toB40);
				testCase->addPredicateCoverage(predicates::B18B_TT);
			} else {
				if( sumOfFirstHalf == sumOfSecondHalf ) {
					testCase->addPredicateCoverage(predicates::B18B_TF);
				} else if ( sumOfAll > 0 ) {
					testCase->addPredicateCoverage(predicates::B18B_FT);
				} else {
					testCase->addPredicateCoverage(predicates::B18B_FF);
				}
				if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
					testCase->addEdgeCoverage(edges::B18toB41);
					testCase->addPredicateCoverage(predicates::B18C_TT);
				} else {
					if( sumOfFirstHalf == sumOfSecondHalf ) {
						testCase->addPredicateCoverage(predicates::B18C_TF);
					} else if ( sumOfAll > 0 ) {
						testCase->addPredicateCoverage(predicates::B18C_FT);
					} else {
						testCase->addPredicateCoverage(predicates::B18C_FF);
					}
					testCase->addEdgeCoverage(edges::B18toB42);
				}
			}
		}
}

inline void MedHardCFG::block19() {
	//mu <= 500 sigma > 1000
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addEdgeCoverage(edges::B19toB43);
		testCase->addPredicateCoverage(predicates::B19_TT);
	} else {
		if( (a1 == a2) ) {
			testCase->addPredicateCoverage(predicates::B19_TF);
		} else if( (a2 == a3) ) {
			testCase->addPredicateCoverage(predicates::B19_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B19_FF);
		}
		testCase->addEdgeCoverage(edges::B19toB44);
	}
}

inline void MedHardCFG::block20() {
	//mu <= 500 sigma > 500 sigma < 1000

	if( sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0 ) {
		testCase->addEdgeCoverage(edges::B20toB45);
		testCase->addPredicateCoverage(predicates::B20A_TT);
	} else {
		if( sumOfFirstHalf == sumOfSecondHalf ) {
			testCase->addPredicateCoverage(predicates::B20A_TF);
		} else if ( sumOfAll < 0 ) {
			testCase->addPredicateCoverage(predicates::B20A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B20A_FF);
		}

		   if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0 ) {
				testCase->addEdgeCoverage(edges::B20toB46);
				testCase->addPredicateCoverage(predicates::B20B_TT);
			} else {
				if( sumOfFirstHalf == sumOfSecondHalf ) {
					testCase->addPredicateCoverage(predicates::B20B_TF);
				} else if ( sumOfAll > 0 ) {
					testCase->addPredicateCoverage(predicates::B20B_FT);
				} else {
					testCase->addPredicateCoverage(predicates::B20B_FF);
				}
				if ( sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0 ) {
					testCase->addEdgeCoverage(edges::B20toB47);
					testCase->addPredicateCoverage(predicates::B20C_TT);
				} else {
					if( sumOfFirstHalf == sumOfSecondHalf ) {
						testCase->addPredicateCoverage(predicates::B20C_TF);
					} else if ( sumOfAll > 0 ) {
						testCase->addPredicateCoverage(predicates::B20C_FT);
					} else {
						testCase->addPredicateCoverage(predicates::B20C_FF);
					}
					testCase->addEdgeCoverage(edges::B20toB48);
				}
			}
		}
}

inline void MedHardCFG::block21() {
	//mu <= 500 sigma > 250 sigma < 500
	int a1 = testCase->getInputParameterAtIndex(0),
		a2 = testCase->getInputParameterAtIndex(0),
		a3 = testCase->getInputParameterAtIndex(0);

	if( (a1 == a2) && (a2 == a3) ) {
		testCase->addPredicateCoverage(predicates::B21_TT);
		testCase->addEdgeCoverage(edges::B21toB49);
	} else {
		if( (a1 == a2) ) {
			testCase->addPredicateCoverage(predicates::B21_TF);
		} else if( (a2 == a3) ) {
			testCase->addPredicateCoverage(predicates::B21_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B21_FF);
		}
		testCase->addEdgeCoverage(edges::B21toB50);
	}
}

inline void MedHardCFG::block22() {
	//mu <= 500 sigma >= 100 sigma <= 250

	if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll < 0) {
		testCase->addEdgeCoverage(edges::B22toB51);
		testCase->addPredicateCoverage(predicates::B22A_TT);
	} else {
		if (sumOfFirstHalf == sumOfSecondHalf) {
			testCase->addPredicateCoverage(predicates::B22A_TF);
		} else if (sumOfAll < 0) {
			testCase->addPredicateCoverage(predicates::B22A_FT);
		} else {
			testCase->addPredicateCoverage(predicates::B22A_FF);
		}

		if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll > 0) {
			testCase->addEdgeCoverage(edges::B22toB52);
			testCase->addPredicateCoverage(predicates::B22B_TT);
		} else {
			if (sumOfFirstHalf == sumOfSecondHalf) {
				testCase->addPredicateCoverage(predicates::B22B_TF);
			} else if (sumOfAll > 0) {
				testCase->addPredicateCoverage(predicates::B22B_FT);
			} else {
				testCase->addPredicateCoverage(predicates::B22B_FF);
			}
			if (sumOfFirstHalf == sumOfSecondHalf && sumOfAll == 0) {
				testCase->addEdgeCoverage(edges::B22toB53);
				testCase->addPredicateCoverage(predicates::B22C_TT);
			} else {
				if (sumOfFirstHalf == sumOfSecondHalf) {
					testCase->addPredicateCoverage(predicates::B22C_TF);
				} else if (sumOfAll > 0) {
					testCase->addPredicateCoverage(predicates::B22C_FT);
				} else {
					testCase->addPredicateCoverage(predicates::B22C_FF);
				}
				testCase->addEdgeCoverage(edges::B22toB54);
			}
		}
	}
}

void MedHardCFG::calculateMean(const int* values) {
	long long sum { 0 };
	for (int i = 0; i < numberOfParameters; ++i) {
		sum += values[i];
	}

	Mu = ((double) sum / (double) numberOfParameters);
}

void MedHardCFG::calculateDeviation(const int* values) {
	long long sum { 0 };

	for (int i = 0; i < numberOfParameters; ++i) {
		sum += (pow(values[i] - Mu, 2) / numberOfParameters);
		assert(sum >= 0);
	}
	sigma = sqrt(sum);
	assert(sigma >= 0);
}



void MedHardCFG::calculateSums(const int* values) {
	sumOfAll = 0;
	sumOfFirstHalf = 0;
	sumOfSecondHalf = 0;

	for (int i = 0; i < numberOfParameters; ++i) {

		if( i < 5 ) {
			sumOfFirstHalf += values[i];
		} else {
			sumOfSecondHalf += values[numberOfParameters - i - 1];
		}
	}
	sumOfAll = sumOfFirstHalf + sumOfSecondHalf;
}

void MedHardCFG::printInputParameters(int* inputParameters) const{
	cout << endl << "Input Parameters" << endl;
	cout << "     A        B        C        D        E        F        G        H        I        J" << endl;
	cout << "-----------------------------------------------" << endl;
	for (int i = 0; i < numberOfParameters; i++) {
		cout << "  " << std::setw(8) << inputParameters[i] << "  |";
	}
	cout << endl;
}

void MedHardCFG::printEdgesCovered(bool* edgesCovered) const{
	cout << endl << "Edge Coverage" << endl;
	cout << " B0toB1 - B1toB3-B1toB4-B3toB7-B3toB8-B4toB9-B4toB10-B7toB15," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for (int j = 0; j < 8; j++) {
		cout << "   " << std::setw(8) << edgesCovered[j] << "   |";
	}
	cout << endl;
	cout << "B7toB16-B8toB17-B8toB18-B9toB19-B9toB20-B10toB21-B10toB22" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[8 + j] << "   |";
	}
	cout << endl;
	cout << "B15toB31, B15toB32, B16toB33, B16toB34, B16toB35, B16toB36 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[15 + j] << "   |";
	}
	cout << endl;
	cout << "B17toB37, B17toB38, B18toB39, B18toB40, B18toB41, B18toB42 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[21 + j] << "   |";
	}
	cout << endl;
	cout << "B19toB43, B19toB44, B20toB45, B20toB46, B20toB47, B20toB48 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[27 + j] << "   |";
	}
	cout << endl;
	cout << "B21toB49, B21toB50, B22toB51, B22toB52, B22toB53, B22toB54 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[33 + j] << "   |";
	}
	cout << endl;
	cout << "B0toB2-B2toB5-B2toB6-B5toB11-B5toB12-B6toB13-B6toB14-B11toB23," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 8; j++)
	{
		cout << "   " << edgesCovered[39 + j] << "   |";
	}
	cout << endl;
	cout << "B11toB24-B12toB25-B12toB26-B13toB27-B13toB28-B14toB29-B14toB30 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[47 + j] << "   |";
	}
	cout << endl;

	cout << "B23toB55, B23toB56, B24toB57, B24toB58, B24toB59, B24toB60," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[54 + j] << "   |";
	}
	cout << endl;
	cout << "B25toB61, B25toB62, B26toB63, B26toB64, B26toB65, B26toB66," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[60 + j] << "   |";
	}
	cout << endl;
	cout << "B27toB67, B27toB68, B28toB69, B28toB70, B28toB71, B28toB72," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[66 + j] << "   |";
	}
	cout << endl;
	cout << "B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[72 + j] << "   |";
	}
	cout << endl;
}

void MedHardCFG::printPredicatesCovered(bool* predicatesCovered) const{
	cout << "\tTODO" << endl; //TODO printPredicatesCovered
}

void MedHardCFG::printEdgesCovered(int* edgesCovered) const{
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
	cout << "B15toB31, B15toB32, B16toB33, B16toB34, B16toB35, B16toB36 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[15 + j] << "   |";
	}
	cout << endl;
	cout << "B17toB37, B17toB38, B18toB39, B18toB40, B18toB41, B18toB42 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[21 + j] << "   |";
	}
	cout << endl;
	cout << "B19toB43, B19toB44, B20toB45, B20toB46, B20toB47, B20toB48 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[27 + j] << "   |";
	}
	cout << endl;
	cout << "B21toB49, B21toB50, B22toB51, B22toB52, B22toB53, B22toB54 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[33 + j] << "   |";
	}
	cout << endl;
	cout << "B0toB2-B2toB5-B2toB6-B5toB11-B5toB12-B6toB13-B6toB14-B11toB23," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 8; j++)
	{
		cout << "   " << edgesCovered[39 + j] << "   |";
	}
	cout << endl;
	cout << "B11toB24-B12toB25-B12toB26-B13toB27-B13toB28-B14toB29-B14toB30 " << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 7; j++)
	{
		cout << "   " << edgesCovered[47 + j] << "   |";
	}
	cout << endl;

	cout << "B23toB55, B23toB56, B24toB57, B24toB58, B24toB59, B24toB60," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[54 + j] << "   |";
	}
	cout << endl;
	cout << "B25toB61, B25toB62, B26toB63, B26toB64, B26toB65, B26toB66," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[60 + j] << "   |";
	}
	cout << endl;
	cout << "B27toB67, B27toB68, B28toB69, B28toB70, B28toB71, B28toB72," << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[66 + j] << "   |";
	}
	cout << endl;
	cout << "B29toB73, B29toB74, B30toB75, B30toB76, B30toB77, B30toB78" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	for(int j = 0; j < 6; j++)
	{
		cout << "   " << edgesCovered[72 + j] << "   |";
	}
	cout << endl;
}

void MedHardCFG::printPredicatesCovered(int* predicatesCovered) const{
	cout << "\tTODO" << endl;//TODO printPredicatesCovered

}
