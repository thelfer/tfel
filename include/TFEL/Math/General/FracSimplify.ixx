/*!
 * \file   include/TFEL/Math/General/FracSimplify.ixx
 * \brief  This file declares the useful metafunction FracSimplify.
 * \author Helfer Thomas
 * \date   18 Jul 2006
 */

#ifndef _LIB_TFEL_FRACSIMPLIFY_I_
#define _LIB_TFEL_FRACSIMPLIFY_I_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

namespace tfel{

  namespace math{

    namespace internals{
    
      template<unsigned int N1,unsigned int D1,unsigned int Dac>
      struct RecursiveFracSimplify
      {
	TFEL_STATIC_ASSERT(D1!=0u);
	
	static const unsigned int Ntmp    = ((N1%Dac==0)&&(D1%Dac==0u)) ? N1/Dac  : N1;
	static const unsigned int Dtmp    = ((N1%Dac==0)&&(D1%Dac==0u)) ? D1/Dac  : D1;
	static const unsigned int Dactmp1 = ((N1%Dac==0)&&(D1%Dac==0u)) ? Dac     : Dac+1;
	static const unsigned int Dactmp  = (Dactmp1>Dtmp) ? 0u : Dactmp1;
	
	static const unsigned int N = (Dac>D1) ? N1 : RecursiveFracSimplify<Ntmp,Dtmp,Dactmp>::N;
	static const unsigned int D = (Dac>D1) ? D1 : RecursiveFracSimplify<Ntmp,Dtmp,Dactmp>::D;
      };
      
      template<unsigned int N1,unsigned int D1>
      struct RecursiveFracSimplify<N1,D1,0u>
      {
	TFEL_STATIC_ASSERT(D1!=0u);
	static const unsigned int N = N1;
	static const unsigned int D = D1;
      };
    
    } // end of namespace internals
    
    template<int N1, unsigned int D1>
    struct FracSimplify
    {
    private:
      TFEL_STATIC_ASSERT(D1!=0u);
      static const int sign           = (N1<0) ? -1 : 1;   
      static const unsigned int Ntmp  = sign*N1;
      static const unsigned int Ntmp2 = tfel::math::internals::RecursiveFracSimplify<Ntmp,D1,2>::N;
    public:
      static const int N              = sign*static_cast<int>(Ntmp2);
      static const unsigned int D     = tfel::math::internals::RecursiveFracSimplify<Ntmp,D1,2>::D;
    };
      
    template<unsigned int D1>
    struct FracSimplify<0,D1>
    {
      TFEL_STATIC_ASSERT(D1!=0u);
      static const int N          = 0;
      static const unsigned int D = 1u;
    };
      
    template<int N1>
    struct FracSimplify<N1,1u>
    {
      static const int N          = N1;
      static const unsigned int D = 1u;
    };
      
    template<>
    struct FracSimplify<0,1u>
    {
      static const int N          = 0;
      static const unsigned int D = 1u;
    };

    } // end of namespace math
    
  } // end of namespace tfel

#endif /* _LIB_TFEL_FRACSIMPLIFY_I_ */

