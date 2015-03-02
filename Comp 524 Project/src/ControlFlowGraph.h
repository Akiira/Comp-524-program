///////////////////////////////////////////////////////////
//  ControlFlowGraph.h
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
#define EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_

#include "TestCase.h"
#include <iostream>
//Forward declaration
class TestCase;

class ControlFlowGraph
{

public:
	void setCoverageOfTestCase(TestCase* testCase);

	virtual ~ControlFlowGraph();
	virtual int getNumberOfEdges()=0;
	virtual int getNumberOfPredicates()=0;
	virtual int getNumberOfParameters()=0;


protected:
	TestCase* testCase;
	int numberOfEdges;
	int numberOfPredicates;
	int numberOfParameters;

	// Must be overridden by each subclass, called by getCoverageOfTestCase,
	//	expected to update coverage instance variable through side effects
	//	so it can ultimately be returned by setCOverageOfTestCase
	virtual void runTestCase()=0;
};

#endif // !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
