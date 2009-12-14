/*!
 * \file   FSLevenbergMarquardt.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 nov 2008
 */

#ifndef _LIB_FSLEVENBERGMARQUARDT_HXX_
#define _LIB_FSLEVENBERGMARQUARDT_HXX_ 

#include"Math/tvector.hxx"
#include"Math/tmatrix.hxx"
#include"Math/TinyMatrixSolve.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N, // number of variables
	     unsigned short M, // number of parameters
	     typename T = double>
    struct FSLevenbergMarquardt
    {

      typedef std::pair<T,tvector<M,T> > (* PtrFun)(const tvector<N,T>&,
						    const tvector<M,T>&);

      FSLevenbergMarquardt(const PtrFun);

      void
      addData(const tvector<N,T>&,const T);

      void
      setInitialGuess(const tvector<M,T>&);

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

      const tvector<M,T>&
      execute(void);
      
    private:
      
      std::vector<std::pair<tvector<N,T>,double> > data;
      tvector<M,T> p;
      T lambda0;
      T factor;
      T eps1;
      T eps2;
      unsigned short iterMax;
      PtrFun f;

    }; // end of struct FSLevenbergMarquardt

  } // end of namespace math

} // end of namespace tfel

#include"Math/LevenbergMarquardt/FSLevenbergMarquardt.ixx"

#endif /* _LIB_FSLEVENBERGMARQUARDT_HXX */

