/*! 
 * \file   test-f
 * \author Helfer Thomas
 * \date   16 jan 2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#ifndef F77_FUNC
#define F77_FUNC(X,Y) X##_
#endif
#ifndef F77_FUNC_
#define F77_FUNC_(X,Y) X##_
#endif

double F77_FUNC(computeyoung,COMPUTEYOUNG)(const double*);
double F77_FUNC(computenu,COMPUTENU)(const double*);

int main(void){
  
  const double T = 900.;
  const double y = 127.8e9 * (1.-7.825e-5*(T-293.15));
  const double n = 0.3272  * (1.-3.056e-5*(T-293.15));
  assert(fabs(F77_FUNC(computeyoung,COMPUTEYOUNG)(&T)-y)<1.e-14*y);
  assert(fabs(F77_FUNC(computenu,COMPUTENU)(&T)-n)<1.e-14*n);

  return EXIT_SUCCESS;
}
