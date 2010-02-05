/*!
 * \file   FSLevenbergMarquardt.hxx
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

#ifndef _LIB_FSLEVENBERGMARQUARDT_HXX_
#define _LIB_FSLEVENBERGMARQUARDT_HXX_ 

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N, // number of variables
	     typename T>
    struct FSLevenbergMarquardtVariable
    {
      typedef tvector<N,T> type;
    }; // end of struct Variable

    template<typename T>
    struct FSLevenbergMarquardtVariable<1u,T>
    {
      typedef T type;
    }; // end of struct Variable

    template<unsigned short N, // number of variables
	     unsigned short M, // number of parameters
	     typename T = double>
    struct FSLevenbergMarquardt
    {

      typedef  typename FSLevenbergMarquardtVariable<N,T>::type Variable;
      typedef  typename FSLevenbergMarquardtVariable<M,T>::type Parameter;
      
      typedef std::pair<T,Parameter> (* PtrFun)(const Variable&,
						const Parameter&);

      FSLevenbergMarquardt(const PtrFun);

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

    }; // end of struct FSLevenbergMarquardt

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LevenbergMarquardt/FSLevenbergMarquardt.ixx"

#endif /* _LIB_FSLEVENBERGMARQUARDT_HXX */

