/*! 
 * \file  Kriging.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGING_H_
#define _LIB_TFEL_MATH_KRIGING_H_ 

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultModels.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N,
	     typename T = double,
	     typename Model = KrigingDefaultModel<N,T> >
    struct Kriging
      : public Model
    {

      void
      addValue(const typename KrigingVariable<N,T>::type&,
	       const T&);

      void
      buildInterpolation(void);
  
      T
      operator()(const typename KrigingVariable<N,T>::type&) const;

    private:

      tfel::math::vector<typename KrigingVariable<N,T>::type> x;
      tfel::math::vector<T> f;
      tfel::math::vector<T> a;

    }; // end of struct Kriging

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Kriging/Kriging.ixx"

#endif /* _LIB_TFEL_MATH_KRIGING_H */

