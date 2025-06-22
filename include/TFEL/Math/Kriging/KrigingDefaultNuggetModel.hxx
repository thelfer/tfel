/*!
 * \file  include/TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGINGDEFAULTNUGGETMODEL_H_
#define LIB_TFEL_MATH_KRIGINGDEFAULTNUGGETMODEL_H_

#include "TFEL/Math/Kriging/KrigingVariable.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename T>
    struct KrigingDefaultNuggetModel {
      KrigingDefaultNuggetModel() : nugget(T(0)) {}

      T nuggetEffect(typename vector<T>::size_type,
                     const typename KrigingVariable<N, T>::type&) const {
        using namespace std;
        return this->nugget;
      }  // end of nuggetEffect

      void setNuggetEffect(const T& v) { this->nugget = v; }

     protected:
      T nugget;

    };  // end of KrigingDefaultNuggetModel

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_KRIGINGDEFAULTNUGGETMODEL_H_ */
