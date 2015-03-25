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

	int* duplicateEdgesCovered;
	int* duplicatePredicatesCovered;

	TestCase** testCases;

	double coverageRatio;

	// SHared code between constructors
	void initializeMembersAndAllocateMemory(int numberOfTestCases, int maxNumberOfTestCases);
	void fillTestSuiteWithRandomTestCases();


public:
	virtual ~TestSuite();

	TestSuite(const TestSuite&);
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases);
	TestSuite(int numberOfTestCases, int maxNumberOfTestCases, TestCase** testCasesToCopy);

	TestCase** getAllTestCases() const;
	TestCase* getTestCase(int index);
	void setTestCase(int index, TestCase* testCase);
	void replaceRandomTestCase(TestCase* testCase);
	void addTestCase(TestCase* testCase);
	void removeTestCase(int index);
	bool canRemoveTestCaseWithoutChangingCoverage(int index);

	void resetCoverage();
	void calculateTestSuiteCoverage();

	void printTestCaseInputsAndCoverage();
	void printTestCaseInputsOnly();
	void printTestSuiteCoverage();
	void printAll();

	TestSuite& operator=(const TestSuite& org);
	bool operator==(const TestSuite& right);

	int* getDuplicateEdgesCovered() const {
		return duplicateEdgesCovered;
	}

	int* getDuplicatePredicatesCovered() const {
		return duplicatePredicatesCovered;
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

	void setNumberOfTestCases(int numberOfTestCases) {
		this->numberOfTestCases = numberOfTestCases;
	}

	void sortTestSuiteByCoverageCounts();

	TestCase* getTestCaseThatCoversPredicate(int predicateNumber);
	TestCase* getTestCaseThatCoversEdge(int edgeNumber);
};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
