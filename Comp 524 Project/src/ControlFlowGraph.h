///////////////////////////////////////////////////////////
//  ControlFlowGraph.h
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
#define EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_

#include "TestCase.h"

class ControlFlowGraph
{

public:
	ControlFlowGraph();
	virtual ~ControlFlowGraph();

	// Returns a 2d boolean array where retval[0] = edges covered, retval[1] = predicates covered
	bool** getCoverageOfTestCase(TestCase* testCase);

protected:
	bool** coverage;
	TestCase* testCase;
	int NUM_OF_EDGES;
	int NUM_OF_PREDICATES;

	// Must be overridden by each subclass, called by getCoverageOfTestCase,
	//	expected to update coverage instance variable through side effects
	//	so it can ultimately be returned by getCOverageOfTestCase
	virtual void runTestCase();
};

#endif // !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
