

#include <iostream>
#include "TestCase.h"
#include "TestSuite.h"
#include "ControlFlowGraph.h"
using namespace std;

int main() {

	TestCase* tc = new TestCase(4, 1);
	int p[1];
	p[0] = 3;
	tc->setInputParameters(p);



	return 0;
}
