/*!
 * \file   tests/Math/newton_raphson.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/TinyNewtonRaphson.hxx"

struct Func
  : public tfel::math::TinyNewtonRaphson<2u,double,Func>
{
  static const double a; 
  static const double b;
  void computeFdF()
  {
    f(0)   = a*(1-x(0));
    f(1)   = b*(x(1)-x(0)*x(0));
    J(0,0) = -a;
    J(0,1) = 0.;
    J(1,0) = -2*b*x(0);
    J(1,1) = b;
  } // end of computeFdF
}; // end of struct Func

const double Func::a = 1.;
const double Func::b = 10.;

struct Func2
  : public tfel::math::TinyNewtonRaphson<1u,double,Func2>
{
  void computeFdF()
  {
    f = x*x-2.;
    J = 2*x;
  } // end of computFdF
}; // end of struct Func2


int main(void){

  using namespace std;
  using namespace tfel::math;

  double x[] = {0.,0.};
  double y = 100.;

  Func nr;
  nr.setPrecision(1.e-5);
  nr.setMaximumIteration(20);
  nr.setInitialGuess(x);
  nr.exe();

  Func2 nr2;
  nr2.setPrecision(1.e-10);
  nr2.setMaximumIteration(20);
  nr2.setInitialGuess(y);
  nr2.exe();

  return EXIT_SUCCESS;
}
