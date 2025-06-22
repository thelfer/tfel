/*!
 * \file  include/TFEL/Math/Kriging/KrigingDefaultModels.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_HXX
#define LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_HXX

#include <cmath>

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Kriging/KrigingVariable.hxx"
#include "TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel::math {

  template <unsigned short N,
            typename T,
            typename NM = KrigingDefaultNuggetModel<N, T>>
  struct KrigingDefaultModel;

}  // end of namespace tfel::math

#include "TFEL/Math/Kriging/KrigingDefaultModel1D.hxx"
#include "TFEL/Math/Kriging/KrigingDefaultModel2D.hxx"
#include "TFEL/Math/Kriging/KrigingDefaultModel3D.hxx"

#endif /* LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_HXX */
