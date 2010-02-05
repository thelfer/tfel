/*!
 * \file   LevenbergMarquardt.hxx
 * \brief  
 *
 * "Methods for non-linear least
 * squares problems"
 * 2nd Edition, April 2004
 * K. Madsen, H.B. Nielsen, O. Tingleff
 *
 * \author Helfer Thomas
 * \date   21 nov 2008
 */

#ifndef _LIB_LEVENBERGMARQUARDT_HXX_
#define _LIB_LEVENBERGMARQUARDT_HXX_ 

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/LUSolve.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T = double>
    struct LevenbergMarquardt
    {

      typedef tfel::math::vector<T> Variable;
      typedef tfel::math::vector<T> Parameter;
      typedef tfel::math::vector<T> Gradient;
      typedef typename Variable::size_type size_type;

      typedef void (* PtrFun)(T&,
			      Gradient&,
			      const Variable&,
			      const Parameter&);

      LevenbergMarquardt(const size_type, // number of variables
			 const size_type, // number of parameters
			 const PtrFun);

      void
      addData(const Variable&,
	      const T);

      void
      setInitialGuess(const Parameter&);

      void
      setInitialDampingParameter(const T);

      void
      setFirstCriterium(const T);

      void
      setSecondCriterium(const T);

      void
      setMultiplicationFactor(const T);

      void
      setMaximumIteration(const T);

      const Parameter&
      execute(void);
      
    private:
      
      std::vector<std::pair<Variable,double> > data;
      Parameter p;
      T lambda0;
      T factor;
      T eps1;
      T eps2;
      unsigned short iterMax;
      PtrFun f;
      size_type n;
      size_type m;

    }; // end of struct LevenbergMarquardt

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LevenbergMarquardt/LevenbergMarquardt.ixx"

#endif /* _LIB_LEVENBERGMARQUARDT_HXX */

