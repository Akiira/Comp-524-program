///////////////////////////////////////////////////////////
//  TestCase.h
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
#define EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_

//Forward declaration
class ControlFlowGraph;

class TestCase
{

public:
	virtual ~TestCase();

	TestCase(int numberOfParameters, int numberOfEdges, int numberOfPredicates);
	TestCase(ControlFlowGraph& cfg);
	// Copy constructor
	TestCase(const TestCase& that);


	bool* getEdgesCovered();
	bool* getPredicatesCovered();
	int* getInputParameters();
	int getInputParameterAtIndex(int index);

	void addEdgeCoverage(int edge);
	void addPredicateCoverage(int predicate);
	void setInputParameters(int[]);
	void setInputParameterAtIndex(int index, int newValue);

	void clearCoverage();
	void print(ControlFlowGraph* cfg);

	TestCase& operator=(const TestCase& org);

private:
	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	int* inputParameters;
	bool* edgesCovered;
	bool* predicatesCovered;

	void generateRandomParameters();

};
#endif // !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
