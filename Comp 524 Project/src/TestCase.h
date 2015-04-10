///////////////////////////////////////////////////////////
//  TestCase.h
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////

#if !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
#define EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_

//Forward declaration
class ControlFlowGraph;
class Range;
class TestCase
{

public:
	virtual ~TestCase();

	TestCase();
	// Copy constructor
	TestCase(const TestCase& that);

	//TestCase(int rangeNum);

	TestCase(bool empty); // If empty is true it doesn't create input parameters.
	TestCase(Range* range);


	void mutate();

	bool* getEdgesCovered() const;
	bool* getPredicatesCovered() const;
	int* getInputParameters() const;
	int getInputParameterAtIndex(int index) const;

	void addEdgeCoverage(int edge);
	void addPredicateCoverage(int predicate);
	void setInputParameters(int[]);
	void setInputParametersWithReference(int* []);
	void setInputParameterAtIndex(int index, int newValue);

	void clearCoverage();
	void printInputsAndCoverage();
	void printInputsOnly();

	bool hasSameCoverage(TestCase* that);

	TestCase& operator=(const TestCase& org);

	int getNumberOfParameters() const {
		return numberOfParameters;
	}

	int getNumberCovered() const {
		return numCovered;
	}

private:
	int numberOfParameters;
	int numberOfEdges;
	int numberOfPredicates;

	int* inputParameters;
	bool* edgesCovered;
	bool* predicatesCovered;

	// Total count of edges and predicates covered, used for sorting in test suite.
	int numCovered;

	void generateRandomParameters();

	void generateParametersFromSingleRange(Range* range);
	void generateParametersFromGlobalRangeSet();
	//void generateRandomParametersInRange(int rangeNum);
	//void generateRandomParametersFromRandomRanges();



};
#endif // !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
