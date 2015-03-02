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

class ControlFlowGraph
{

public:
	ControlFlowGraph();
	virtual ~ControlFlowGraph();

	// Returns a 2d boolean array where retval[0] = edges covered, retval[1] = predicates covered
	bool** getCoverageOfTestCase(TestCase* testCase);

	// =0 makes it pure virtual, so it can only be defined in subclasses
	virtual int getNumberOfEdges()=0;
	virtual int getNumberOfPredicates()=0;

protected:
	bool** coverage;
	TestCase* testCase;
	int numberOfEdges;
	int numberOfPredicates;

	// Must be overridden by each subclass, called by getCoverageOfTestCase,
	//	expected to update coverage instance variable through side effects
	//	so it can ultimately be returned by getCOverageOfTestCase
	virtual void runTestCase()=0;
};

#endif // !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
