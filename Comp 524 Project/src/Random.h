///////////////////////////////////////////////////////////
//  Random.h
//  Implementation of the Class Random
//  Created on:      27-Feb-2015 12:13:59 AM
//  Original author: Randall
///////////////////////////////////////////////////////////

#if !defined(EA_48879502_EC2C_4c41_94A4_804A9E653A41__INCLUDED_)
#define EA_48879502_EC2C_4c41_94A4_804A9E653A41__INCLUDED_

#include <random>     // needed for the mersenne twister random number generator
#include <functional>  // needed for bind
using namespace std;

extern long uniformInRange(long from, long to);
extern function<double()> uniform01;
extern mt19937 mersenne;


#endif // !defined(EA_48879502_EC2C_4c41_94A4_804A9E653A41__INCLUDED_)
