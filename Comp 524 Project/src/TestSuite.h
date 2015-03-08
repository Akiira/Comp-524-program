///////////////////////////////////////////////////////////
//  TestSuite.h
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
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

	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	//bool* edgesCovered;
	//bool* predicatesCovered;
	int* duplicateEdgesCovered;
	int* duplicatePredicatesCovered;

	TestCase** testCases;


	// SHared code between constructors
	void initializeMembersAndAllocateMemory(int numberOfTestCases);

	void fillTestSuiteWithRandomTestCases();
	void fillTestSuiteWithExistingTestCases(TestCase** testCasesToCopy);

public:
	virtual ~TestSuite();

	TestSuite(const TestSuite&);
	TestSuite(int numberOfTestCases);
	TestSuite(int numberOfTestCases, TestCase** testCasesToCopy);

	TestCase** getAllTestCases() const;
	TestCase* getTestCase(int index);
	void calculateTestSuiteCoverage();

	void print();
	void printSimple();
	void printOnlyTestSuiteCoverage();
	TestSuite& operator=(const TestSuite& org);

	int* getDuplicateEdgesCovered() const {
		return duplicateEdgesCovered;
	}

	int* getDuplicatePredicatesCovered() const {
		return duplicatePredicatesCovered;
	}

	/*
	bool* getEdgesCovered() const {
		return edgesCovered;
	}

	void setEdgesCovered(bool* edgesCovered) {
		this->edgesCovered = edgesCovered;
	}


	bool* getPredicatesCovered() const {
		return predicatesCovered;
	}

	void setPredicatesCovered(bool* predicatesCovered) {
		this->predicatesCovered = predicatesCovered;
	}
	*/

	int getNumberOfTestCases() const {
		return numberOfTestCases;
	}

	void setNumberOfTestCases(int numberOfTestCases) {
		this->numberOfTestCases = numberOfTestCases;
	}
};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
