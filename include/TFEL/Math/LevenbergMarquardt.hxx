/*!
 * \file   include/TFEL/Math/LevenbergMarquardt.hxx
 * \brief
 *
 * "Methods for non-linear least
 * squares problems"
 * 2nd Edition, April 2004
 * K. Madsen, H.B. Nielsen, O. Tingleff
 *
 * \author Thomas Helfer
 * \date   21 nov 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_LEVENBERGMARQUARDT_HXX
#define LIB_LEVENBERGMARQUARDT_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/LUSolve.hxx"
#include "TFEL/Math/LevenbergMarquardt/LevenbergMarquardtFunctionWrapper.hxx"

namespace tfel {

  namespace math {

    template <typename F = LevenbergMarquardtFunctionWrapper<double>>
    struct LevenbergMarquardt {
      typedef typename F::NumericType T;

      typedef tfel::math::vector<T> Variable;
      typedef tfel::math::vector<T> Parameter;
      typedef tfel::math::vector<T> Gradient;
      typedef typename Variable::size_type size_type;

      LevenbergMarquardt(const F);

      void addData(const Variable&, const T);

      void setInitialGuess(const Parameter&);

      void setInitialDampingParameter(const T);

      void setFirstCriterium(const T);

      void setSecondCriterium(const T);

      void setMultiplicationFactor(const T);

      void setMaximumIteration(const T);

      unsigned short getNumberOfIterations() const;

      const Parameter& execute();

      ~LevenbergMarquardt();

     private:
      F f;
      std::vector<std::pair<Variable, double>> data;
      Parameter p;
      T lambda0 = T(1.e-3);
      T factor = T(2);
      T eps1 = T(1.e-10);
      T eps2 = T(1.e-10);
      unsigned short iter = 0;
      unsigned short iterMax = 100;
    };  // end of struct LevenbergMarquardt

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/LevenbergMarquardt/LevenbergMarquardt.ixx"

#endif /* LIB_LEVENBERGMARQUARDT_HXX */
