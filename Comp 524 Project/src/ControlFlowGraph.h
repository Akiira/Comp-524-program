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
	bool** getCoverageOfTestCase(TestCase* testCase);

private:


protected:
	bool** coverage;
	TestCase* testCase;
};

#endif // !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
