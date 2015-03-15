///////////////////////////////////////////////////////////
//  TestSuite.h
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
#define EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_

#include "TestCase.h"
#include "ControlFlowGraph.h"
#include <bitset>

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
	void addTestCase(TestCase* testCase);
	void resetCoverage();
	void calculateTestSuiteCoverage();

	void print();
	void printSimple();
	void printOnlyTestSuiteCoverage();
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

	void setNumberOfTestCases(int numberOfTestCases) {
		this->numberOfTestCases = numberOfTestCases;
	}
};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
