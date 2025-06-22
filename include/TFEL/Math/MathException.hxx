/*!
 * \file   include/TFEL/Math/MathException.hxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   24 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATHEXCEPTION_HXX
#define LIB_TFEL_MATHEXCEPTION_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Exception/TFELException.hxx"

namespace tfel {

  namespace math {

    struct TFELMATH_VISIBILITY_EXPORT MathException
        : public tfel::exception::TFELException {
      MathException() = default;
      MathException(MathException&&) = default;
      MathException(const MathException&) = default;
      ~MathException() noexcept override;
    };  // end of struct MathException

    struct TFELMATH_VISIBILITY_EXPORT MathRunTimeException
        : public MathException {
      MathRunTimeException() = default;
      MathRunTimeException(MathRunTimeException&&) = default;
      MathRunTimeException(const MathRunTimeException&) = default;
      ~MathRunTimeException() noexcept override;
    };  // end of struct MathRunTimeException

    struct TFELMATH_VISIBILITY_EXPORT InvalidTimeStepException
        : public MathRunTimeException {
      InvalidTimeStepException() = default;
      InvalidTimeStepException(InvalidTimeStepException&&) = default;
      InvalidTimeStepException(const InvalidTimeStepException&) = default;
      const char* what() const noexcept override final;
      ~InvalidTimeStepException() noexcept override;
    };  // end of struct InvalidTimeStepException

    struct TFELMATH_VISIBILITY_EXPORT MathDivergenceException
        : public MathException {
      MathDivergenceException() = default;
      MathDivergenceException(MathDivergenceException&&) = default;
      MathDivergenceException(const MathDivergenceException&) = default;
      ~MathDivergenceException() noexcept override;
    };  // end of struct MathDivergenceException

    struct TFELMATH_VISIBILITY_EXPORT MaximumNumberOfIterationsReachedException
        : public MathDivergenceException {
      MaximumNumberOfIterationsReachedException() = default;
      MaximumNumberOfIterationsReachedException(
          MaximumNumberOfIterationsReachedException&&) = default;
      MaximumNumberOfIterationsReachedException(
          const MaximumNumberOfIterationsReachedException&) = default;
      const char* what() const noexcept override final;
      ~MaximumNumberOfIterationsReachedException() noexcept override;
    };  // end of struct MaximumNumberOfIterationsReachedException

    struct TFELMATH_VISIBILITY_EXPORT SingularJacobianException
        : public MathDivergenceException {
      SingularJacobianException() = default;
      SingularJacobianException(SingularJacobianException&&) = default;
      SingularJacobianException(const SingularJacobianException&) = default;
      const char* what() const noexcept override final;
      ~SingularJacobianException() noexcept override;
    };  // end of struct SingularJacobianException

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATHEXCEPTION_HXX */
