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

public:
	virtual ~TestSuite();

	TestSuite(int numberOfTestCases, ControlFlowGraph* targetCFG);
	TestSuite(int numberOfTestCases, TestCase** testCasesToCopy, ControlFlowGraph* targetCFG);

	TestCase** getAllTestCases() const;
	TestCase* getTestCase(int index);

	int* getDuplicateEdgesCovered() const {
		return duplicateEdgesCovered;
	}

	int* getDuplicatePredicatesCovered() const {
		return duplicatePredicatesCovered;
	}

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

	int getNumberOfTestCases() const {
		return numberOfTestCases;
	}

	void setNumberOfTestCases(int numberOfTestCases) {
		this->numberOfTestCases = numberOfTestCases;
	}

	void calculateTestSuiteCoverage();

	void print();

private:
	int numberOfTestCases;

	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	bool* edgesCovered;
	bool* predicatesCovered;
	int* duplicateEdgesCovered;
	int* duplicatePredicatesCovered;

	TestCase** testCases;

	// Adding this so it can be passed to print, Also will probably be needed in other
	//	methods later.
	ControlFlowGraph* targetCFG;

	// SHared code between constructors
	void initializeMembersAndAllocateMemory(int numberOfTestCases, ControlFlowGraph* targetCFG);

	void fillTestSuiteWithRandomTestCases();
	void fillTestSuiteWithExistingTestCases(TestCase** testCasesToCopy);

};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
