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

class TestSuite
{

public:
	virtual ~TestSuite();

	TestSuite(int numberOfTestCases, ControlFlowGraph* cfg);
	TestCase** getAllTestCases();
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

	int getNumberOfTestCases() const {
		return numberOfTestCases;
	}

	void setNumberOfTestCases(int numberOfTestCases) {
		this->numberOfTestCases = numberOfTestCases;
	}

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

};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
