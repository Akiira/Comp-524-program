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
	TestSuite();
	virtual ~TestSuite();

	TestSuite(int numberOfTestCases, int numberOfParameters, int numberOfEdges);
	TestCase** getAllTestCases();
	TestCase* getTestCase(int index);

private:
	int* duplicates;
	bool* edgesCovered;
	TestCase** testCases;

};
#endif // !defined(EA_04823F4F_8B3F_4bb8_9DBC_42A1717DC256__INCLUDED_)
