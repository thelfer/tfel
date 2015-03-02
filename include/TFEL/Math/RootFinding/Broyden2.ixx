/*!
 * \file   include/TFEL/Math/RootFinding/Broyden2.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 avr 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_BROYDEN2_IXX_
#define LIB_TFEL_BROYDEN2_IXX_ 

#ifdef TFEL_BROYDEN2_VERBOSE_MODE
#include<iostream>
#endif /* LIB_TFEL_BROYDEN2_IXX_ */

#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    template<unsigned short N,
	     typename T,
	     const tvector<N,T> (*f)(const tvector<N,T>&)>
    const std::pair<bool,tvector<N,T> >
    broyden2(const tvector<N,T>& x0,
	     tmatrix<N,N,T>& A,
	     const T e,
	     const unsigned short n)
    {
      using namespace std;
      using namespace tfel::math;
      tmatrix<N,N,T> J;
      tvector<N,T> x;
      tvector<N,T> s;
      tvector<N,T> y;
      tvector<N,T> vf;
      tvector<N,T> vf2;
      unsigned short i;
      x  = x0;
      vf = f(x);
      i  = 0;
      while((i<n)&&(norm(vf)/N>e)){
#ifdef TFEL_BROYDEN2_VERBOSE_MODE
	std::cout << "iteration : " << i        << std::endl;	
	std::cout << "x         : " << x        << std::endl;	
	std::cout << "||f||     : " << norm(vf) << std::endl;	
	std::cout << "A         : " << A        << std::endl;	
#endif /* LIB_TFEL_BROYDEN2_IXX_ */
	s   = -A*vf;
	x  += s;
	vf2 = vf;
	vf  = f(x);
	y   = vf-vf2;
	J   = A;
	A  += ((s-J*y)^(s*J))/(s|J*y);
	++i;
      }
      return pair<bool,tvector<N,T> >(i!=n,x);
    } // end of function broyden2

    template<unsigned short N,
	     typename T,
	     const tvector<N,T> (*f)(const tvector<N,T>&)>
    const std::pair<bool,tvector<N,T> >
    broyden2(const tvector<N,T>& x0,
	     const T e,
	     const unsigned short n)
    {
      using namespace std;
      using namespace tfel::math;
      tmatrix<N,N,T> A(0.);
      unsigned short i;
      for(i=0;i!=N;++i){
	A(i,i) = 1.;
      }
      return broyden2<N,T,f>(x0,A,e,n);
    } // end of function broyden2
    
  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_BROYDEN2_IXX_ */

