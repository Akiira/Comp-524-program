///////////////////////////////////////////////////////////
//  Random.cpp
//  Implementation of the Class Random
//  Created on:      27-Feb-2015 12:13:59 AM
//  Original author: Randall and Austin
///////////////////////////////////////////////////////////


#include "Random.h"

//random_device seed;
//mt19937 mersenne(seed());  //Mersenne Twister random number generator
//long uniformInRange(long from, long to)
//{// generate a random uniformly in the range [from, to]
//  uniform_int_distribution<long> uniformFT(from, to);
//  return uniformFT(mersenne);
//}
//
//
////uniform01() generates a random double uniformly in the range [0,1)
//function<double()> uniform01 = bind(uniform_real_distribution<double>(0,1), mt19937(seed()));

#include <ctime>
#include <cassert>
int seed = time(NULL);
mt19937 mersenne(seed);  //Mersenne Twister random number generator
long uniformInRange(long from, long to)
{// generate a random uniformly in the range [from, to]
  uniform_int_distribution<long> uniformFT(from, to);
  return uniformFT(mersenne);
}


//uniform01() generates a random double uniformly in the range [0,1)
function<double()> uniform01 = bind(uniform_real_distribution<double>(0,1), mt19937(seed));

/**
 * Returns random numbers according to an (approximate) normal distribution
 * with average mu and standard deviation sigma.
 *
 * @param mu     The average value to return
 * @param sigma  The standard deviation of the values to return
 * @param n	     The number of uniformly distributed values that are used to
 *         	     create this value. The higher n, the better the distribution
 *               will resemble a normal distribution. Defaults to 4.
 * @return       A random value
 */
double normalDist( double mu, double sigma, int n )
{
	assert( n > 0 );

	double sum = 0.0;
	for( int i = 0; i < n; i++ )
		sum += uniform01();

	// RandomDouble gives us a uniformly distributed number between 0 and 1.
	// This value has mu=0.5 and var=1/12. For the sum, this is mu=n*0.5 and
	// var=n/12.

	sum -= ( (double)n / 2.0 );						// Go to N(0, 1/12n)
	sum *= ( sigma / ( sqrt((double)n / 12.0 )) );	// Go to N(0, var)
	sum += mu;										// Go to N(mu, var)

	return sum;
}
