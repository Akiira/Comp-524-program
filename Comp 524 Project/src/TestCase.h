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
	TestCase(const TestCase& that);
	TestCase(Range* range);

	static TestCase* getRandomTestCase();


	//============================MUTATOR FUNCTIONS=======================//

	void mutate();
	void addEdgeCoverage(int edge);
	void addPredicateCoverage(int predicate);
	void setInputParameters(int[]);
	void setInputParameter(int index, int newValue);
	void clearCoverage();

	//============================PRINT FUNCTIONS=======================//

	void printInputsAndCoverage() const;
	void printInputsOnly() const;

	//============================GETTER FUNCTIONS=======================//

	bool* getEdgesCovered() const;
	bool* getPredicatesCovered() const;
	int* getInputParameters() const;
	int getParameter(int index) const;

	int getNumberOfParameters() const {
		return numberOfParameters;
	}

	int getNumberCovered() const {
		return numCovered;
	}

	bool hasSameCoverage(TestCase* that) const;

	//===========================OVERLOADED OPERATORS==========================//

	TestCase& operator=(const TestCase& org);

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
};
#endif // !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
