///////////////////////////////////////////////////////////
//  TestCase.h
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
#define EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_
#include "ControlFlowGraph.h"

//Forward declaration
class ControlFlowGraph;


#include "Random.h"

class TestCase
{

public:
	virtual ~TestCase();


	TestCase(int numberOfParameters, int numberOfEdges, int numberOfPredicates);

	TestCase(const ControlFlowGraph& cfg);


	bool* getEdgesCovered();
	bool* getPredicatesCovered();


	bool* edgesCovered;
	bool* predicatesCovered;

	int* getInputParameters();
	void setInputParameters(int[]);
	void setInputParameterAtIndex(int index, int newValue);

	void clearCoverage();
	void print();

private:
	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	int* inputParameters;
	void generateRandomParameters();

};
#endif // !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
