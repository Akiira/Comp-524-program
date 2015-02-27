///////////////////////////////////////////////////////////
//  TestCase.h
//  Implementation of the Class TestCase
//  Created on:      27-Feb-2015 12:13:42 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
#define EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_

class TestCase
{

public:
	TestCase();
	virtual ~TestCase();

	TestCase(int numberOfEdges, int numberOfParameters);
	bool* getEdgesCovered();
	int* getInputParameters();

private:
	bool* branchEdgesCovered;
	int* inputParameters;
	bool* multiConditionCovered;

};
#endif // !defined(EA_B2C38A95_D3AD_472b_BE3C_F83540FB1055__INCLUDED_)
