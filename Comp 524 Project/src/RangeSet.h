/*
 * RangeSet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: Austin and Randall
 */

#ifndef RANGESET_H_
#define RANGESET_H_

class Range;
class TestCase;
class TestSuite;

#include <cassert>
#include <string>
using std::string;
class RangeSet
{
public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//

	virtual ~RangeSet();

	RangeSet();
	RangeSet(int numberOfRanges, int maxNumberOfRanges);
	RangeSet(int numberOfRanges, int maxNumberOfRanges, Range** ranges);

	//========================== CORE FUNCTIONS ====================================//

	Range** selectRangesForNewTestCaseProportionalToUsefulness();
	void addRange(Range* r);
	void addNewRandomRange();
	void adaptRangesBasedOnUsefulness(); // May split, delete, or combine ranges maybe
	void offerToFinalTestSuite(TestCase* tc, int source);

	//========================== GETTER FUNCTIONS ==================================//

	TestCase* getNewTestCase();
	TestCase* getNewTestCaseEntirelyFromRange(Range* range);
	TestCase* getNewTestCaseEntirelyFromRange(int start, int end);

	int getNumberOfRanges() const {
		return numberOfRanges;
	}

	Range* getRange(int index) {
		assert(index >= 0 && index < numberOfRanges);
		return ranges[index];
	}

	TestSuite* getFinalTestSuite() {
		return finalTestSuite;
	}

	int* getTestCaseSourceCounts() {
		return testCaseSources;
	}

	string getTestCaseSourceReport() {
		string retval = "%Test Case Source Report:";
		retval += "\n%\tInit_Pop: " + std::to_string(testCaseSources[0]);
		retval += "\n%\tTestSuiteMutation: " + std::to_string(testCaseSources[1]);
		retval += "\n%\tTestCaseCrossoverAndMutation: " + std::to_string(testCaseSources[2]);
		retval += "\n%\tLocalOptFromParameters: " + std::to_string(testCaseSources[3]);
		retval += "\n%\tLocalOptFromZero: " + std::to_string(testCaseSources[4]) + "\n";
		return retval;
	}

	string getAvgTestCaseSourceReport(double* avgTestCaseSources) {
		double sum = 0;
		for (int i = 0; i < 5; i++) {
			sum += avgTestCaseSources[i];
		}

		string retval = "\t\t%Test Case Source Report: (Avg. Size of final suite: " + std::to_string(sum) + ")";
		retval += "\n\t\t%\tInitial Population:           " + std::to_string(avgTestCaseSources[0]) + " (" + std::to_string(avgTestCaseSources[0] / sum * 100) + "%)";
		retval += "\n\t\t%\tTestSuiteMutation:            " + std::to_string(avgTestCaseSources[1]) + " (" + std::to_string(avgTestCaseSources[1] / sum * 100) + "%)";
		retval += "\n\t\t%\tTestCaseCrossoverAndMutation: " + std::to_string(avgTestCaseSources[2]) + " (" + std::to_string(avgTestCaseSources[2] / sum * 100) + "%)";
		retval += "\n\t\t%\tLocalOptFromParameters:       " + std::to_string(avgTestCaseSources[3]) + " (" + std::to_string(avgTestCaseSources[3] / sum * 100) + "%)";
		retval += "\n\t\t%\tLocalOptFromZero:             " + std::to_string(avgTestCaseSources[4]) + " (" + std::to_string(avgTestCaseSources[4] / sum * 100) + "%)";
		return retval;
	}
	//========================== PRINT FUNCTIONS ===================================//

	void printRanges();
	void printRangesSimple();



private:
	int totalUsefulness, numberOfRanges, maxNumberOfRanges, minNumberOfRanges;
	Range** ranges;
	TestSuite* finalTestSuite;

	/*	Track where the test cases in the final suite came from
	 * [0] - initial population
	 * [1] - test suite mutation
	 * [2] - test case crossover and mutation
	 * [3] - local opt around parameters
	 * [4] - local opt around zero
	 */
	int* testCaseSources;

	unsigned int getRandomRange() const;

	void addRangesAdjacentToExistingRange(int index);

	void splitRange(int index);
	void deleteRange(int index);

	void sortRangesByUsefulness();
	void moveRangeToSortedPosition(int indexToSort);
};

#endif /* RANGESET_H_ */
