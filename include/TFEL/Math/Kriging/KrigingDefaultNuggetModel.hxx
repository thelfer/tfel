/*! 
 * \file  KrigingDefaultNuggetModel.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTNUGGETMODEL_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTNUGGETMODEL_H_ 

#include"TFEL/Math/Kriging/KrigingVariable.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N,
	     typename T>
    struct KrigingDefaultNuggetModel
    {

      KrigingDefaultNuggetModel<N,T>()
	: nugget(T(0))
      {}

      T
      nuggetEffect(typename vector<T>::size_type,
		   const typename KrigingVariable<N,T>::type&) const
      {
	using namespace std;
	return this->nugget;
      } // end of covariance

      void
      setNuggetEffect(const T& v)
      {
	this->nugget = v;
      }

    protected:
      
      T nugget;

    }; // end of KrigingDefaultNuggetModel

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTNUGGETMODEL_H */

