/*!
 * \file   RungeKutta42.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 oct 2007
 */

#ifndef _LIB_TFEL_RUNGEKUTTA42_HXX_
#define _LIB_TFEL_RUNGEKUTTA42_HXX_ 

#include"Metaprogramming/IF.hxx"
#include"Math/tvector.hxx"

namespace tfel{
  
  namespace math{

    template<unsigned short N,
	     typename Child,
	     typename Scalar = double>
    struct RungeKutta42
    {
      typedef typename tfel::meta::IF<N==1,
				      Scalar,
				      tvector<N,Scalar> >::type VariableType;
      void
      setInitialValue(const VariableType&);
      void
      setInitialTime(const Scalar);
      void
      setFinalTime(const Scalar);
      void
      setInitialTimeIncrement(const Scalar);
      void
      setCriterium(const Scalar);
      void
      iterate(void);
      const VariableType&
      getValue(void) const;
      const Scalar&
      getTimeIncrement(void) const;
    private:
      struct AbsoluteSommation;
      VariableType y;
      Scalar ti;
      Scalar tf;
      Scalar dt;
      Scalar epsilon;
    }; // end of struct RungeKutta42
    
  } // end of namespace math

} // end of namespace tfel

#include"Math/RungeKutta/RungeKutta42.ixx"

#endif /* _LIB_TFEL_RUNGEKUTTA42_HXX */

