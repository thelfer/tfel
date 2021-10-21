/*!
 * \file   include/TFEL/MATH/FischerBurmeister.hxx
 * \brief
 * \author Thomas Helfer
 * \date   29/05/2019
 */

#ifndef LIB_TFEL_MATH_FISCHERBURMEISTER_HXX
#define LIB_TFEL_MATH_FISCHERBURMEISTER_HXX

#include <tuple>
#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief return a regularised version of the Fischer-Burmeister function
     * defined by:
     * \[
     * f\left(x,y)=x+y-\sqrt{x^{2}+y^{2}+\varepsilon^{2}}
     * \]
     * This function is such that:
     * \[
     * f\left(x,y)=0 \RighatArrow x\leq 0, y\leq 0 and
     * \sqrt{2\,x\,y}=\varepsilon^{2}
     * \]
     *
     * > A Regularized and Smoothed Fischer-Burmeister Method for
     * > Quadratic Programming with Applications to Model Predictive
     * > Control.  Dominic Liao-McPherson, Mike Huang, and Ilya Kolmanovsky
     *
     * \tparam real: numeric type
     * \param[in] x: first argument
     * \param[in] y: second argument
     * \param[in] e: regularisation parameter
     * \return the value  of the Fischer-Burmeister function
     */
    template <typename real>
    TFEL_MATH_INLINE real regularisedFischerBurmeisterFunction(
        const real&, const real&, const real& = real{});
    /*!
     * \return the derivatives of the regularised version of the
     * Fischer-Burmeister function \tparam real: numeric type \param[in] x:
     * first argument \param[in] y: second argument \param[in] e: regularisation
     * parameter
     */
    template <typename real>
    TFEL_MATH_INLINE std::tuple<real, real>
    regularisedFischerBurmeisterFunctionFirstDerivatives(const real&,
                                                         const real&,
                                                         const real&);
    /*!
     * \return the square of the Fischer-Burmeister function
     * \tparam real: numeric type
     * \param[in] x: first argument
     * \param[in] y: second argument
     */
    template <typename real>
    TFEL_MATH_INLINE real squaredFischerBurmeisterFunction(const real&,
                                                           const real&);
    /*!
     * \return the derivatives of the square of the Fischer-Burmeister function
     * \tparam real: numeric type
     * \param[in] x: first argument
     * \param[in] y: second argument
     */
    template <typename real>
    TFEL_MATH_INLINE std::tuple<real, real>
    squaredFischerBurmeisterFunctionFirstDerivatives(const real&, const real&);
  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/FischerBurmeister.ixx"

#endif /* LIB_TFEL_MATH_FISCHERBURMEISTER_HXX */
