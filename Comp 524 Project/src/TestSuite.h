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
public:

	//========================== CONSTRUCTORS AND DESTRUCTORS ======================//
	virtual ~TestSuite();

	TestSuite(const TestSuite&);

	TestSuite(Range *range, int numberOfTestCases): TestSuite(range, numberOfTestCases, numberOfTestCases) {};
	TestSuite(Range *range, int numberOfTestCases, int maxNumberOfTestCases) : TestSuite(numberOfTestCases, maxNumberOfTestCases, 0, range) {};

	TestSuite(int numberOfTestCases) : TestSuite(numberOfTestCases, numberOfTestCases) {}
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases): TestSuite(numberOfTestCases, numberOfTestCases, 0) {}
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases, TestCase** testCases): TestSuite(numberOfTestCases, maxNumberOfTestCases, testCases, 0) {};

	// Actually implements
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases, TestCase** testCases, Range* range);

	//========================== MUTATOR FUNCTIONS =================================//

	void setTestCase(int index, TestCase* testCase);
	void replaceDuplicateTestCase(TestCase* testCase);
	void addTestCase(TestCase* testCase);
	void removeTestCase(int index);

	void resetCoverage();
	void calculateTestSuiteCoverage();

	void sortTestSuiteByCoverageCounts();

	//========================== GETTER FUNCTIONS ==================================//

	TestCase* getDuplicateTestCase() const;
	TestCase* getRandomTestCase() const;
	TestCase* getTestCaseThatCoversPredicate(int predicateNumber) const;
	TestCase* getTestCaseThatCoversEdge(int edgeNumber) const;

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

	double getBranchCoverageRatio() const {
		return branchCoverageRatio;
	}

	double getMCCCoverageRatio() const {
		return mccCoverageRatio;
	}

	//========================== PREDICATE FUNCTIONS ===============================//

	bool canRemoveTestCaseWithoutChangingCoverage(int index) const;

	bool isCoveringNew(const TestCase * tc) const;
	bool isCoveringNewEdge(const bool * coverage) const;
	bool isCoveringNewPred(const bool * coverage) const;

	//========================== PRINT FUNCTIONS ===================================//

	void printTestCaseInputsAndCoverage() const;
	void printTestCaseInputsOnly() const;
	void printTestSuiteCoverage() const;
	void printAll() const;
	void printTestSuiteCoverageRatio() const;

	//========================== OVERLOADED OPERATORS ==============================//

	TestSuite& operator=(const TestSuite& org);

private:
	int numberOfTestCases;
	int maxNumberOfTestCases;

	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	int* edgeCoverageCounts;
	int* predicateCoverageCounts;

	TestCase** testCases;

	double coverageRatio, branchCoverageRatio, mccCoverageRatio;
};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
