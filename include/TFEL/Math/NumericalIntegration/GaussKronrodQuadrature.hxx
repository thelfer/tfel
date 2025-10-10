/*!
 * \file   TFEL/Math/NumericalIntegration/GaussKronrodQuadrature.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08/10/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_NUMERICALINTEGRATION_GAUSSKRONRODQUADRATURE_HXX
#define LIB_TFEL_MATH_NUMERICALINTEGRATION_GAUSSKRONRODQUADRATURE_HXX

#include <array>
#include <tuple>
#include <cstddef>
#include <optional>
#include <numeric>
#include <type_traits>

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ResultType.hxx"

namespace tfel::math {

  struct GaussKronrodQuadrature {
    /*!
     * \brief numerical parameters for the adaptative version
     */
    template <ScalarConcept real>
    struct NumericalParameters {
      //! \brief upper bound of the absolute tolerance on the integral value
      const real absolute_tolerance;
      //! \brief maximum level of refinement
      const std::size_t maximum_number_of_refinements;
    };

    /*!
     * \param[in] f: function to be integrated
     * \param[in] a: first bound
     * \param[in] b: second bound
     * \param[in] e: expected errors
     */
    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto operator()(
        const FunctionType &f,                                     //
        const real a,                                              //
        const real b,                                              //
        const NumericalParameters<real> params) const              //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        -> std::optional<
            tfel::math::result_type<real,
                                    std::invoke_result_t<FunctionType, real>,
                                    OpMult>>  //
    requires(std::is_invocable_v<FunctionType, real>);

    /*!
     * \param[in] f: function to be integrated
     * \param[in] a: first bound
     * \param[in] b: second bound
     */
    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto operator()(const FunctionType &f,
                                            const real a,
                                            const real b) const    //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        -> std::optional<std::tuple<
            tfel::math::result_type<real,
                                    std::invoke_result_t<FunctionType, real>,
                                    OpMult>,
            tfel::math::result_type<
                real,
                numeric_type<std::invoke_result_t<FunctionType, real>>,
                OpMult>>>  //
    requires(std::is_invocable_v<FunctionType, real>);

   private:
    static constexpr auto eval_if_required(const auto &);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto integrate(
        const FunctionType &,
        const real,
        const real,
        const NumericalParameters<real> &) const                   //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        -> std::optional<
            tfel::math::result_type<real,
                                    std::invoke_result_t<FunctionType, real>,
                                    OpMult>>  //
    requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto integrate(const FunctionType &f,
                                           const real a,
                                           const real b) const     //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto computeUnboundedIntegral(
        const FunctionType &) const                                //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto computeLeftUnboundedIntegral(
        const FunctionType &, const real) const                    //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto computeRightUnboundedIntegral(
        const FunctionType &, const real) const                    //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto computeUnboundedIntegral(
        const FunctionType &,
        const NumericalParameters<real> &) const                   //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto computeLeftUnboundedIntegral(
        const FunctionType &,
        const real,
        const NumericalParameters<real> &) const                   //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        -> std::optional<
            tfel::math::result_type<real,
                                    std::invoke_result_t<FunctionType, real>,
                                    OpMult>>
    requires(std::is_invocable_v<FunctionType, real>);

    template <ScalarConcept real, typename FunctionType>
    [[nodiscard]] constexpr auto computeRightUnboundedIntegral(
        const FunctionType &,
        const real,
        const NumericalParameters<real> &) const                   //
        noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
        -> std::optional<
            tfel::math::result_type<real,
                                    std::invoke_result_t<FunctionType, real>,
                                    OpMult>>
    requires(std::is_invocable_v<FunctionType, real>);
  };

  inline constexpr auto gauss_kronrod_integrate = GaussKronrodQuadrature{};

}  // end of namespace tfel::math

#include "TFEL/Math/NumericalIntegration/GaussKronrodQuadrature.ixx"

#endif /* LIB_TFEL_MATH_NUMERICALINTEGRATION_GAUSSKRONRODQUADRATURE_HXX */
