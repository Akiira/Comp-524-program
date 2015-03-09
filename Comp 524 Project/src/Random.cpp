///////////////////////////////////////////////////////////
//  Random.cpp
//  Implementation of the Class Random
//  Created on:      27-Feb-2015 12:13:59 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////


#include "Random.h"

random_device seed;
mt19937 mersenne(seed());  //Mersenne Twister random number generator
long uniformInRange(long from, long to)
{// generate a random uniformly in the range [from, to]
  uniform_int_distribution<long> uniformFT(from, to);
  return uniformFT(mersenne);
}


//uniform01() generates a random double uniformly in the range [0,1)
function<double()> uniform01 = bind(uniform_real_distribution<double>(0,1), mt19937(seed()));
