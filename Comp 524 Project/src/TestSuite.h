///////////////////////////////////////////////////////////
//  TestSuite.h
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
#define EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_

#include "TestCase.h"

class TestSuite
{
private:
	int numberOfTestCases;
	int maxNumberOfTestCases;


	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	int* edgeCoverageCounts;
	int* predicateCoverageCounts;

	TestCase** testCases;

	double coverageRatio;


public:
	virtual ~TestSuite();

	TestSuite(const TestSuite&);
	TestSuite(int numberOfTestCases) : TestSuite(numberOfTestCases, numberOfTestCases) {}
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases): TestSuite(numberOfTestCases, numberOfTestCases, 0) {}
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases, TestCase** testCases);

	//============================MUTATOR FUNCTIONS=======================//

	void setTestCase(int index, TestCase* testCase);
	void replaceRandomTestCase(TestCase* testCase);
	void replaceDuplicateTestCase(TestCase* testCase);
	void addTestCase(TestCase* testCase);
	void removeTestCase(int index);

	void resetCoverage();
	void calculateTestSuiteCoverage();
	void sortTestSuiteByCoverageCounts();

	//============================PREDICATE FUNCTIONS=======================//

	bool canRemoveTestCaseWithoutChangingCoverage(int index) const;
	bool coversNewEdge(TestCase* ) const;

	//============================PRINT FUNCTIONS=======================//

	void printTestCaseInputsAndCoverage();
	void printTestCaseInputsOnly();
	void printTestSuiteCoverage();
	void printAll();

	//============================GETTER FUNCTIONS=======================//

	TestCase* getDuplicateTestCase() const;
	TestCase* getRandomTestCase() const;
	TestCase* getTestCaseThatCoversPredicate(int predicateNumber) const;
	TestCase* getTestCaseThatCoversEdge(int edgeNumber) const;

	bool* getAllUncoveredEdges() const {
		auto unCovered = new bool[numberOfEdges];
		for (int i = 0; i < numberOfEdges; ++i) {
			unCovered[i] = ( edgeCoverageCounts[i] == 0 ? true : false );
		}

		return unCovered;
	}

	bool* getAllUncoveredPredicates() const {
		auto unCovered = new bool[numberOfPredicates];
		for (int i = 0; i < numberOfPredicates; ++i) {
			unCovered[i] = ( edgeCoverageCounts[i] == 0 ? true : false );
		}

		return unCovered;
	}

	bool* getDuplicateEdges() const {
		auto dupes = new bool[numberOfEdges];
		for (int i = 0; i < numberOfEdges; ++i) {
			dupes[i] = ( edgeCoverageCounts[i] > 1 ? true : false );
		}

		return dupes;
	}

	bool* getDuplicatePredicates() const {
		auto dupes = new bool[numberOfPredicates];
		for (int i = 0; i < numberOfPredicates; ++i) {
			dupes[i] = ( edgeCoverageCounts[i] > 1 ? true : false );
		}

		return dupes;
	}

	TestCase** getAllTestCases() const{
		return  testCases;
	}

	TestCase* getTestCase(int index) const {
		return  testCases[index];
	}

	int* getEdgeCoverageCounts() const {
		return edgeCoverageCounts;
	}

	int* getPredicateCoverageCounts() const {
		return predicateCoverageCounts;
	}

	int getNumberOfTestCases() const {
		return numberOfTestCases;
	}

	int getMaxNumberOfTestCases() const {
		return maxNumberOfTestCases;
	}

	double getCoverageRatio() const {
		return coverageRatio;
	}

	//===========================OVERLOADED OPERATORS===========================

	TestSuite& operator=(const TestSuite& org);
	bool operator==(const TestSuite& right);
};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
