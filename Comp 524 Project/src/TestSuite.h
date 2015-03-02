///////////////////////////////////////////////////////////
//  TestSuite.h
//  Implementation of the Class TestSuite
//  Created on:      27-Feb-2015 12:13:32 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
#define EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_

#include "TestCase.h"

class TestSuite
{

public:
	virtual ~TestSuite();

	TestSuite(int numberOfTestCases, int numberOfParameters, int numberOfEdges, int numberOfPredicates);
	TestCase** getAllTestCases();
	TestCase* getTestCase(int index);



	int* getDuplicates() const {
		return duplicates;
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

private:
	int numberOfTestCases;
	int* duplicates;
	bool* edgesCovered;
	TestCase** testCases;

};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
