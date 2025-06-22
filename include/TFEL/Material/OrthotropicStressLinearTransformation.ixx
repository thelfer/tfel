/*!
 * \file   include/TFEL/Material/OrthotropicStressLinearTransformation.ixx
 * \brief
 * \author HELFER Thomas 202608
 * \date   13/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICSTRESSLINEARTRANSFORMATION_IXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICSTRESSLINEARTRANSFORMATION_IXX

namespace tfel::material {

  namespace internals {

    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using OrthotropicAxesConvention = tfel::material::OrthotropicAxesConvention;
    //! a simple alias
    template <ModellingHypothesis::Hypothesis H>
    using ModellingHypothesisToSpaceDimension =
        tfel::material::ModellingHypothesisToSpaceDimension<H>;

    /*!
     * \brief structure in charge of computing a linear
     * transformation of the stress tensor.
     * \tparam N: space dimension
     */
    template <unsigned short N>
    struct OrthotropicStressLinearTransformation;
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformation` in `1D`.
     */
    template <>
    struct OrthotropicStressLinearTransformation<1u> {
      /*!
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of  linear transformation
       * \param[in] c21: coefficient of  linear transformation
       * \param[in] c13: coefficient of  linear transformation
       * \param[in] c31: coefficient of  linear transformation
       * \param[in] c23: coefficient of  linear transformation
       * \param[in] c32: coefficient of  linear transformation
       * \param[in] c44: coefficient of  linear transformation
       * \param[in] c55: coefficient of  linear transformation
       * \param[in] c66: coefficient of  linear transformation
       * \see tfel::material::makeOrthotropicStressLinearTransformation
       */
      template <typename real>
      static tfel::math::st2tost2<1u, real> exe(const real c12,
                                                const real c21,
                                                const real c13,
                                                const real c31,
                                                const real c23,
                                                const real c32,
                                                const real,
                                                const real,
                                                const real) {
        return {(c13 + c12) / 3,     (c13 - 2 * c12) / 3, (c12 - 2 * c13) / 3,
                (c23 - 2 * c21) / 3, (c23 + c21) / 3,     (c21 - 2 * c23) / 3,
                (c32 - 2 * c31) / 3, (c31 - 2 * c32) / 3, (c32 + c31) / 3};
      }  // end of exe
    };   // end of struct OrthotropicStressLinearTransformation<1u>
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformation` in `2D`.
     */
    template <>
    struct OrthotropicStressLinearTransformation<2u> {
      /*!
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of  linear transformation
       * \param[in] c21: coefficient of  linear transformation
       * \param[in] c13: coefficient of  linear transformation
       * \param[in] c31: coefficient of  linear transformation
       * \param[in] c23: coefficient of  linear transformation
       * \param[in] c32: coefficient of  linear transformation
       * \param[in] c44: coefficient of  linear transformation
       * \param[in] c55: coefficient of  linear transformation
       * \param[in] c66: coefficient of  linear transformation
       * \see tfel::material::makeOrthotropicStressLinearTransformation
       */
      template <typename real>
      static tfel::math::st2tost2<2u, real> exe(const real c12,
                                                const real c21,
                                                const real c13,
                                                const real c31,
                                                const real c23,
                                                const real c32,
                                                const real c44,
                                                const real,
                                                const real) {
        constexpr const auto zero = real{0};
        return {(c13 + c12) / 3,
                (c13 - 2 * c12) / 3,
                (c12 - 2 * c13) / 3,
                zero,
                (c23 - 2 * c21) / 3,
                (c23 + c21) / 3,
                (c21 - 2 * c23) / 3,
                zero,
                (c32 - 2 * c31) / 3,
                (c31 - 2 * c32) / 3,
                (c32 + c31) / 3,
                zero,
                zero,
                zero,
                zero,
                c44};
      }  // end of exe
    };   // end of struct OrthotropicStressLinearTransformation<2u>
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformation` in `3D`.
     */
    template <>
    struct OrthotropicStressLinearTransformation<3u> {
      /*!
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of  linear transformation
       * \param[in] c21: coefficient of  linear transformation
       * \param[in] c13: coefficient of  linear transformation
       * \param[in] c31: coefficient of  linear transformation
       * \param[in] c23: coefficient of  linear transformation
       * \param[in] c32: coefficient of  linear transformation
       * \param[in] c44: coefficient of  linear transformation
       * \param[in] c55: coefficient of  linear transformation
       * \param[in] c66: coefficient of  linear transformation
       * \see tfel::material::makeOrthotropicStressLinearTransformation
       */
      template <typename real>
      static tfel::math::st2tost2<3u, real> exe(const real c12,
                                                const real c21,
                                                const real c13,
                                                const real c31,
                                                const real c23,
                                                const real c32,
                                                const real c44,
                                                const real c55,
                                                const real c66) {
        constexpr const auto zero = real{0};
        return {(c13 + c12) / 3,
                (c13 - 2 * c12) / 3,
                (c12 - 2 * c13) / 3,
                zero,
                zero,
                zero,
                (c23 - 2 * c21) / 3,
                (c23 + c21) / 3,
                (c21 - 2 * c23) / 3,
                zero,
                zero,
                zero,
                (c32 - 2 * c31) / 3,
                (c31 - 2 * c32) / 3,
                (c32 + c31) / 3,
                zero,
                zero,
                zero,
                zero,
                zero,
                zero,
                c44,
                zero,
                zero,
                zero,
                zero,
                zero,
                zero,
                c55,
                zero,
                zero,
                zero,
                zero,
                zero,
                zero,
                c66};
      }  // end of exe
    };   // end of struct OrthotropicStressLinearTransformation<3u>
    /*!
     * \brief an helper structure used to build linear
     * transformation according to the modelling hypothesis and the
     * orthotropic axis convention.
     * \tparam H: modelling hypothesis
     * \tparam c: orthotropic axis convention
     */
    template <ModellingHypothesis::Hypothesis H, OrthotropicAxesConvention c>
    struct OrthotropicStressLinearTransformationII;
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for the
     * `tfel::material::OrthotropicAxesConvention::DEFAULT`
     * orthotropic convention.
     */
    template <ModellingHypothesis::Hypothesis H>
    struct OrthotropicStressLinearTransformationII<
        H,
        OrthotropicAxesConvention::DEFAULT>
        : public OrthotropicStressLinearTransformation<
              ModellingHypothesisToSpaceDimension<H>::value> {
    };  // end of struct
        // OrthotropicStressLinearTransformationII<H,OrthotropicAxesConvention::DEFAULT>
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for the
     * `tfel::material::OrthotropicAxesConvention::PIPE` orthotropic
     * convention for non plane modelling hypothesis.
     */
    template <ModellingHypothesis::Hypothesis H>
    struct OrthotropicStressLinearTransformationII<
        H,
        OrthotropicAxesConvention::PIPE>
        : public OrthotropicStressLinearTransformation<
              ModellingHypothesisToSpaceDimension<H>::value> {
    };  // end of struct
        // OrthotropicStressLinearTransformationII<H,OrthotropicAxesConvention::PIPE>
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for the
     * `tfel::material::OrthotropicAxesConvention::PIPE`
     * orthotropic convention and the
     * `tfel::material::ModellingHypothesis::PLANESTRAIN`
     * modelling hypothesis.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::PLANESTRAIN,
        OrthotropicAxesConvention::PIPE> {
      /*!
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of  linear transformation
       * \param[in] c21: coefficient of  linear transformation
       * \param[in] c13: coefficient of  linear transformation
       * \param[in] c31: coefficient of  linear transformation
       * \param[in] c23: coefficient of  linear transformation
       * \param[in] c32: coefficient of  linear transformation
       * \param[in] c44: coefficient of  linear transformation
       * \param[in] c55: coefficient of  linear transformation
       * \param[in] c66: coefficient of  linear transformation
       * \see tfel::material::makeOrthotropicStressLinearTransformation
       */
      template <typename real>
      static tfel::math::st2tost2<2u, real> exe(const real c12,
                                                const real c21,
                                                const real c13,
                                                const real c31,
                                                const real c23,
                                                const real c32,
                                                const real c44,
                                                const real c55,
                                                const real c66) {
        return OrthotropicStressLinearTransformation<2u>::exe(
            c13, c31, c12, c21, c32, c23, c55, c44, c66);
      }  // end of exe
    };   // end of struct OrthotropicStressLinearTransformationII
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for the
     * `tfel::material::OrthotropicAxesConvention::PIPE`
     * orthotropic convention and the
     * `tfel::material::ModellingHypothesis::PLANESTRESS`
     * modelling hypothesis.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::PLANESTRESS,
        OrthotropicAxesConvention::PIPE> {
      /*!
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of  linear transformation
       * \param[in] c21: coefficient of  linear transformation
       * \param[in] c13: coefficient of  linear transformation
       * \param[in] c31: coefficient of  linear transformation
       * \param[in] c23: coefficient of  linear transformation
       * \param[in] c32: coefficient of  linear transformation
       * \param[in] c44: coefficient of  linear transformation
       * \param[in] c55: coefficient of  linear transformation
       * \param[in] c66: coefficient of  linear transformation
       * \see tfel::material::makeOrthotropicStressLinearTransformation
       */
      template <typename real>
      static tfel::math::st2tost2<2u, real> exe(const real c12,
                                                const real c21,
                                                const real c13,
                                                const real c31,
                                                const real c23,
                                                const real c32,
                                                const real c44,
                                                const real c55,
                                                const real c66) {
        return OrthotropicStressLinearTransformation<2u>::exe(
            c13, c31, c12, c21, c32, c23, c55, c44, c66);
      }  // end of exe
    };   // end of struct
        // OrthotropicStressLinearTransformationII<H,tfel::material::OrthotropicAxesConvention::PIPE>
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for the
     * `tfel::material::OrthotropicAxesConvention::PIPE` orthotropic
     * convention and the
     * `tfel::material::ModellingHypothesis::GENERALISEDPLANESTRESS`
     * modelling hypothesis.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        OrthotropicAxesConvention::PIPE> {
      /*!
       * \return a linear transformation of the stresses
       * \param[in] c12: coefficient of  linear transformation
       * \param[in] c21: coefficient of  linear transformation
       * \param[in] c13: coefficient of  linear transformation
       * \param[in] c31: coefficient of  linear transformation
       * \param[in] c23: coefficient of  linear transformation
       * \param[in] c32: coefficient of  linear transformation
       * \param[in] c44: coefficient of  linear transformation
       * \param[in] c55: coefficient of  linear transformation
       * \param[in] c66: coefficient of  linear transformation
       * \see tfel::material::makeOrthotropicStressLinearTransformation
       */
      template <typename real>
      static tfel::math::st2tost2<2u, real> exe(const real c12,
                                                const real c21,
                                                const real c13,
                                                const real c31,
                                                const real c23,
                                                const real c32,
                                                const real c44,
                                                const real c55,
                                                const real c66) {
        return OrthotropicStressLinearTransformation<2u>::exe(
            c13, c31, c12, c21, c32, c23, c55, c44, c66);
      }  // end of exe
    };   // end of struct
        // OrthotropicStressLinearTransformationII<H,tfel::material::OrthotropicAxesConvention::PIPE>
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for:
     * - the `ModellingHypothesis::TRIDIMENSIONAL` modelling hypothesis.
     * - the `OrthotropicAxesConvention::PLATE` orthotropic convention.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::TRIDIMENSIONAL,
        OrthotropicAxesConvention::PLATE>
        : public OrthotropicStressLinearTransformation<3u> {
    };  // end of struct OrthotropicStressLinearTransformationII
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for:
     * - the `ModellingHypothesis::PLANESTRESS` modelling hypothesis.
     * - the `OrthotropicAxesConvention::PLATE` orthotropic convention.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::PLANESTRESS,
        OrthotropicAxesConvention::PLATE>
        : public OrthotropicStressLinearTransformation<2u> {
    };  // end of struct OrthotropicStressLinearTransformationII
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for:
     * - the `ModellingHypothesis::PLANESTRAIN` modelling hypothesis.
     * - the `OrthotropicAxesConvention::PLATE` orthotropic convention.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::PLANESTRAIN,
        OrthotropicAxesConvention::PLATE>
        : public OrthotropicStressLinearTransformation<2u> {
    };  // end of struct OrthotropicStressLinearTransformationII
    /*!
     * \brief partial specialisation of the
     * `OrthotropicStressLinearTransformationII` for:
     * - the `ModellingHypothesis::GENERALISEDPLANESTRAIN` modelling hypothesis.
     * - the `OrthotropicAxesConvention::PLATE` orthotropic convention.
     */
    template <>
    struct OrthotropicStressLinearTransformationII<
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        OrthotropicAxesConvention::PLATE>
        : public OrthotropicStressLinearTransformation<2u> {
    };  // end of struct OrthotropicStressLinearTransformationII

  }  // end of namespace internals

  template <unsigned short N, typename real>
  tfel::math::st2tost2<N, real> makeOrthotropicStressLinearTransformation(
      const real c12,
      const real c21,
      const real c13,
      const real c31,
      const real c23,
      const real c32,
      const real c44,
      const real c55,
      const real c66) {
    return internals::OrthotropicStressLinearTransformation<N>::exe(
        c12, c21, c13, c31, c23, c32, c44, c55, c66);
  }  // end of makeOrthotropicStressLinearTransformationType

  template <unsigned short N, typename real>
  tfel::math::st2tost2<N, real> makeOrthotropicStressLinearTransformation(
      const tfel::math::tvector<9u, real>& c) {
    return internals::OrthotropicStressLinearTransformation<N>::exe(
        c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
  }  // end of makeOrthotropicStressLinearTransformationType

  template <ModellingHypothesis::Hypothesis H,
            OrthotropicAxesConvention c,
            typename real>
  tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
  makeOrthotropicStressLinearTransformation(const real c12,
                                            const real c21,
                                            const real c13,
                                            const real c31,
                                            const real c23,
                                            const real c32,
                                            const real c44,
                                            const real c55,
                                            const real c66) {
    return internals::OrthotropicStressLinearTransformationII<H, c>::exe(
        c12, c21, c13, c31, c23, c32, c44, c55, c66);
  }  // end of makeOrthotropicStressLinearTransformation

  template <ModellingHypothesis::Hypothesis H,
            OrthotropicAxesConvention oac,
            typename real>
  tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
  makeOrthotropicStressLinearTransformation(
      const tfel::math::tvector<9u, real>& c) {
    return internals::OrthotropicStressLinearTransformationII<H, oac>::exe(
        c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
  }  // end of makeOrthotropicStressLinearTransformation

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICSTRESSLINEARTRANSFORMATION_IXX */
