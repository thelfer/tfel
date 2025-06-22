/*!
 * \file   include/TFEL/Math/General/RunTimeCheck.hxx
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

#ifndef LIB_TFEL_MATH_RUNTIMECHECK_HXX
#define LIB_TFEL_MATH_RUNTIMECHECK_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/MathException.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel {

  namespace math {

    /*!
     * Exception thrown when runtime checks failed
     */
    struct TFELMATH_VISIBILITY_EXPORT RuntimeCheckFailure
        : public MathRunTimeException {
      RuntimeCheckFailure() = default;
      RuntimeCheckFailure(RuntimeCheckFailure&&) = default;
      RuntimeCheckFailure(const RuntimeCheckFailure&) = default;
      //! \return a string describing the error
      const char* what() const noexcept override final;
      //! destructor
      ~RuntimeCheckFailure() noexcept override;
    };  // end of struct RuntimeCheckFailure

    /*
     * \class RunTimeCheck
     * \brief Type used to compare two runtime properties.
     * \param RunTimeProperties, type of twe runtime properties used.
     */
    template <typename RunTimeProperties>
    struct RunTimeCheck {
      /*
       * \brief check if two runtime properties matches.
       * This is the default implementation which takes two arguments,
       * and checks if they are equals. If they are, return a copy of
       * the first one. If not, throws an exception.
       * \param[in] a : first property.
       * \param[in] b : second property.
       * \return const RunTimeProperties.
       * \require fdq
       * \pre RunTimeProperties must be comparable.
       * \pre RunTimeProperties must be copy-able.
       * \throw MathRunTimeExpection, if test fails.
       */
      TFEL_MATH_INLINE
      static const RunTimeProperties& exe(const RunTimeProperties& a,
                                          const RunTimeProperties& b) {
        if (a != b) {
          throw(RuntimeCheckFailure());
        }
        return a;
      }  // end of exe.
      /*
       * \brief check if two runtime properties matches.
       * This is the default implementation which takes two arguments,
       * and checks if they are equals. If they are, return a copy of
       * the first one. If not, throws an exception.
       * \param[in] a : first property.
       * \param[in] b : second property.
       * \return const RunTimeProperties.
       * \require fdq
       * \pre RunTimeProperties must be comparable.
       * \pre RunTimeProperties must be copy-able.
       * \throw MathRunTimeExpection, if test fails.
       */
      TFEL_MATH_INLINE
      static RunTimeProperties exe(RunTimeProperties&& a,
                                   const RunTimeProperties& b) {
        if (a != b) {
          throw(RuntimeCheckFailure());
        }
        return a;
      }  // end of exe.
    };   // end of RunTimeCheck.

    /*!
     * Partial specialisation for EmptyRunTimeProperties.
     * \see RunTimeCheck.
     * \see EmptyRunTimeProperties.
     */
    template <>
    struct RunTimeCheck<EmptyRunTimeProperties> {
      /*!
       * A do nothing function.
       */
      TFEL_MATH_INLINE
      static EmptyRunTimeProperties exe(const EmptyRunTimeProperties,
                                        const EmptyRunTimeProperties) noexcept {
        return EmptyRunTimeProperties();
      }  // end of exe
    };   // end of RunTimeCheck<EmptyRunTimeProperties>

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_RUNTIMECHECK_HXX */
