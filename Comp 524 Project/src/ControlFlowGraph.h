///////////////////////////////////////////////////////////
//  ControlFlowGraph.h
//  Implementation of the Class ControlFlowGraph
//  Created on:      27-Feb-2015 12:13:50 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
#define EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_

#include "TestCase.h"
#include "TestSuite.h"

#include <iostream>
//Forward declaration
class TestCase;
class TestSuite;

class ControlFlowGraph
{

public:
	virtual ~ControlFlowGraph();

	void setCoverageOfTestCase(TestCase* testCase);

	void printTestCaseCoverage(TestCase* testCase) const;
	void printTestSuiteCoverage(TestSuite* testSuite) const;
	void printPopulationCoverage(int* edges, int* predicates) const;

	int getNumberOfEdges() const;
	int getNumberOfParameters() const;
	int getNumberOfPredicates() const;
	virtual int getLowerBoundForParameter(int)=0;
	virtual int getUpperBoundForParameter(int)=0;
	TestCase* getTestCase() const;

protected:
	TestCase* testCase;
	int numberOfEdges;
	int numberOfPredicates;
	int numberOfParameters;

	// Must be overridden by each subclass, called by getCoverageOfTestCase,
	//	expected to update coverage instance variable through side effects
	//	so it can ultimately be returned by setCOverageOfTestCase
	virtual void runTestCase()=0;

	virtual void printInputParameters(int* inputParameters) const=0;
	virtual void printEdgesCovered(bool* edgesCovered) const=0;
	virtual void printPredicatesCovered(bool* predicatesCovered) const=0;
	virtual void printEdgesCovered(int* edgesCovered) const=0;
	virtual void printPredicatesCovered(int* predicatesCovered) const=0;


	virtual ControlFlowGraph& operator=(const ControlFlowGraph& other);
};

#endif // !defined(EA_36970A6C_11F2_400d_B9FF_6A9242FEA2B5__INCLUDED_)
