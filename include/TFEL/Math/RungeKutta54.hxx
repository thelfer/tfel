/*!
 * \file   include/TFEL/Math/RungeKutta54.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   04 oct 2007
 */

#ifndef _LIB_TFEL_RUNGEKUTTA54_HXX_
#define _LIB_TFEL_RUNGEKUTTA54_HXX_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/FSAlgorithm/accumulate.hxx"
#include"TFEL/Math/tvector.hxx"

namespace tfel
{
  namespace math
  {

    template<unsigned short N,
	     typename Child,
	     typename Scalar = double>
    struct RungeKutta54
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
    }; // end of struct RungeKutta54

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/RungeKutta/RungeKutta54.ixx"

#endif /* _LIB_TFEL_RUNGEKUTTA54_HXX */

