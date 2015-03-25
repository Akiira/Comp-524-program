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
	numberOfEdges = 78;
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
		testCase->addPredicateCoverage(predicates::B0_T);
		block1();
	} else {
		testCase->addEdgeCoverage(edges::B0toB2);
		testCase->addPredicateCoverage(predicates::B0_F);
		block2();
	}
}

void HardCFG::block1() {
	//mu <= 500
	//cout << "\t" << sigma << endl;
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

void HardCFG::block2() {
	//mu > 500

	if( sigma > 100 ) {
		testCase->addEdgeCoverage(edges::B2toB5);
		testCase->addPredicateCoverage(predicates::B2_T);
		block5();
	} else {
		testCase->addEdgeCoverage(edges::B2toB6);
		testCase->addPredicateCoverage(predicates::B2_F);
		block6();
	}
}

void HardCFG::block3() {
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

void HardCFG::block4() {
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

void HardCFG::block5() {
	//mu > 500 sigma > 100

	if( sigma > 500 ) {
		testCase->addEdgeCoverage(edges::B5toB11);
		testCase->addPredicateCoverage(predicates::B5_T);
		block11();
	} else {
		testCase->addEdgeCoverage(edges::B5toB12);
		testCase->addPredicateCoverage(predicates::B5_F);
		block12();
	}
}

void HardCFG::block6() {
	//mu > 500 sigma <= 100
	if( sigma < 50 ) {
		testCase->addEdgeCoverage(edges::B6toB13);
		testCase->addPredicateCoverage(predicates::B6_T);
		block13();
	} else {
		testCase->addEdgeCoverage(edges::B6toB14);
		testCase->addPredicateCoverage(predicates::B6_F);
		block14();
	}
}

void HardCFG::block7() {
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

void HardCFG::block8() {
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

void HardCFG::block9() {
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

void HardCFG::block10() {
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

void HardCFG::block11() {
	//mu > 500 sigma > 500

	if( sigma > 1000 ) {
		testCase->addEdgeCoverage(edges::B11toB23);
		testCase->addPredicateCoverage(predicates::B11_T);
		block23();
	} else {
		testCase->addEdgeCoverage(edges::B11toB24);
		testCase->addPredicateCoverage(predicates::B11_F);
		block24();
	}
}

void HardCFG::block12() {
	//mu > 500 sigma > 100 sigma < 500

	if( sigma > 250 ) {
		testCase->addEdgeCoverage(edges::B12toB25);
		testCase->addPredicateCoverage(predicates::B12_T);
		block25();
	} else {
		testCase->addEdgeCoverage(edges::B12toB26);
		testCase->addPredicateCoverage(predicates::B12_F);
		block26();
	}
}

void HardCFG::block13() {
	//mu > 500 sigma <= 100
	if( sigma < 25 ) {
		testCase->addEdgeCoverage(edges::B13toB27);
		testCase->addPredicateCoverage(predicates::B13_T);
		block27();
	} else {
		testCase->addEdgeCoverage(edges::B13toB28);
		testCase->addPredicateCoverage(predicates::B13_F);
		block28();
	}
}

void HardCFG::block14() {
	//mu > 500 sigma <= 100 sigma > 50
	if( sigma < 75 ) {
		testCase->addEdgeCoverage(edges::B14toB29);
		testCase->addPredicateCoverage(predicates::B14_T);
		block29();
	} else {
		testCase->addEdgeCoverage(edges::B14toB29);
		testCase->addPredicateCoverage(predicates::B14_F);
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

void HardCFG::block16() {
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

void HardCFG::block17() {
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

void HardCFG::block18() {
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

void HardCFG::block19() {
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

void HardCFG::block20() {
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
	cout << "-----------------------------------------------" << endl;
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
}

void HardCFG::printPredicatesCovered(bool* predicatesCovered) const{
	cout << "\tTODO" << endl;
}

void HardCFG::printEdgesCovered(int* edgesCovered) const{
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
}

void HardCFG::printPredicatesCovered(int* predicatesCovered) const{
	cout << "\tTODO" << endl;

}
