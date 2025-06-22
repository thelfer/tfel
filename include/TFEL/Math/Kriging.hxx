/*!
 * \file  include/TFEL/Math/Kriging.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGING_H_
#define LIB_TFEL_MATH_KRIGING_H_

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/vector.hxx"

#include "TFEL/Math/Kriging/KrigingVariable.hxx"
#include "TFEL/Math/Kriging/KrigingDefaultModels.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N,
              typename T = double,
              typename Model = KrigingDefaultModel<N, T>>
    struct TFEL_VISIBILITY_LOCAL Kriging : public Model {
      Kriging() = default;

      void addValue(const typename KrigingVariable<N, T>::type&, const T&);

      void buildInterpolation(void);

      T operator()(const typename KrigingVariable<N, T>::type&) const;

      ~Kriging() noexcept;

     private:
      Kriging(const Kriging&) = delete;
      Kriging& operator=(const Kriging&) = delete;

      tfel::math::vector<typename KrigingVariable<N, T>::type> x;
      tfel::math::vector<T> f;
      tfel::math::vector<T> a;

    };  // end of struct Kriging

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Kriging/Kriging.ixx"

#endif /* LIB_TFEL_MATH_KRIGING_H_ */
