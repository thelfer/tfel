/*!
 * \file   include/TFEL/Material/Hill.ixx
 * \author Thomas Helfer
 * \date   26 March 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_HILL_IXX
#define LIB_TFEL_MATERIAL_HILL_IXX

namespace tfel {

  namespace material {

    namespace internals {

      /*!
       * \brief a structure in charge of computing the Hill tensor for
       * the given space dimension.
       * \tparam N:    space dimension
       * \tparam real: numeric type
       */
      template <unsigned short, typename>
      struct ComputeHillTensorBase;

      template <typename real>
      struct ComputeHillTensorBase<1u, real> {
        static tfel::math::st2tost2<1u, real> hillTensor(const real F,
                                                         const real G,
                                                         const real H,
                                                         const real,
                                                         const real,
                                                         const real) {
          return {F + H, -F, -H, -F, G + F, -G, -H, -G, H + G};
        }
      };  // end of struct HillTensor

      template <typename real>
      struct ComputeHillTensorBase<2u, real> {
        static tfel::math::st2tost2<2u, real> hillTensor(const real F,
                                                         const real G,
                                                         const real H,
                                                         const real L,
                                                         const real,
                                                         const real) {
          constexpr const auto zero = real{0};
          return {F + H, -F, -H,    zero, -F,   G + F, -G,   zero,
                  -H,    -G, H + G, zero, zero, zero,  zero, L};
        }
      };  // end of struct HillTensor

      template <typename real>
      struct ComputeHillTensorBase<3u, real> {
        static tfel::math::st2tost2<3u, real> hillTensor(const real F,
                                                         const real G,
                                                         const real H,
                                                         const real L,
                                                         const real M,
                                                         const real N) {
          constexpr const auto zero = real{0};
          return {F + H, -F,   -H,   zero, zero, zero,  -F,   G + F, -G,
                  zero,  zero, zero, -H,   -G,   H + G, zero, zero,  zero,
                  zero,  zero, zero, L,    zero, zero,  zero, zero,  zero,
                  zero,  M,    zero, zero, zero, zero,  zero, zero,  N};
        }

      };  // end of struct HillTensor

      /*!
       * \brief a structure in charge of computing the Hill tensor
       * according to the modelling hypothesis and the orthotropic
       * axes convention.
       * \tparam H: modelling hypothesis
       * \tparam C: orthotropic axes convention
       * \tparam real: numeric type
       */
      template <tfel::material::ModellingHypothesis::Hypothesis,
                tfel::material::OrthotropicAxesConvention,
                typename>
      struct ComputeHillTensor;
      /*!
       * \brief partial specialisation of the `ComputeHillTensor`
       * class for the `OrthotropicAxesConvention::DEFAULT`
       * orthotropic axes convention.
       * \tparam H: modelling hypothesis
       * \tparam real: numeric type
       */
      template <tfel::material::ModellingHypothesis::Hypothesis H,
                typename real>
      struct ComputeHillTensor<
          H,
          tfel::material::OrthotropicAxesConvention::DEFAULT,
          real>
          : public ComputeHillTensorBase<
                tfel::material::ModellingHypothesisToSpaceDimension<H>::value,
                real> {
        //! \brief space dimension
        static constexpr const auto N =
            tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
        //! \brief return type
        using result_type = tfel::math::st2tost2<N, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<N, real>::hillTensor(H_F, H_G, H_H, H_L,
                                                            H_M, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::DEFAULT,real>
      /*!
       * \brief partial specialisation of the `ComputeHillTensor`
       * class for the `OrthotropicAxesConvention::PIPE`
       * orthotropic axes convention.
       *
       * According this convention, this specialisation is valid for
       * the following modelling hypotheses:
       * - `ModellingHypothesis::TRIDIMENSIONAL`.
       * - `ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN`.
       * - `ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS`.
       * - `ModellingHypothesis::AXISYMMETRICAL`.
       *
       * \tparam H: modelling hypothesis
       * \tparam real: numeric type
       */
      template <tfel::material::ModellingHypothesis::Hypothesis H,
                typename real>
      struct ComputeHillTensor<H,
                               tfel::material::OrthotropicAxesConvention::PIPE,
                               real>
          : public ComputeHillTensorBase<
                tfel::material::ModellingHypothesisToSpaceDimension<H>::value,
                real> {
        //! \brief space dimension
        static constexpr const unsigned short N =
            tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
        //! \brief return type
        using result_type = tfel::math::st2tost2<N, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<N, real>::hillTensor(H_F, H_G, H_H, H_L,
                                                            H_M, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,real>
      /*!
       * \brief partial specialisation of the `ComputeHillTensor`
       * class for the:
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       *   convention.
       * - `ModellingHypothesis::PLANESTRESS` modelling hypothesis.
       *
       * \tparam real: numeric type
       */
      template <typename real>
      struct ComputeHillTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
                               tfel::material::OrthotropicAxesConvention::PIPE,
                               real> : public ComputeHillTensorBase<2u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<2u, real>::hillTensor(H_H, H_G, H_F, H_M,
                                                             H_L, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,real>
      /*!
       * \brief partial specialisation of the `ComputeHillTensor`
       * class for the:
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       *   convention.
       * - `ModellingHypothesis::PLANESTRAIN` modelling hypothesis.
       *
       * \tparam real: numeric type
       */
      template <typename real>
      struct ComputeHillTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
                               tfel::material::OrthotropicAxesConvention::PIPE,
                               real> : public ComputeHillTensorBase<2u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<2u, real>::hillTensor(H_H, H_G, H_F, H_M,
                                                             H_L, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,real>
      /*!
       * \brief partial specialisation of the `ComputeHillTensor`
       * class for the:
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       *   convention.
       * - `ModellingHypothesis::GENERALISEDPLANESTRAIN` modelling hypothesis.
       *
       * \tparam real: numeric type
       */
      template <typename real>
      struct ComputeHillTensor<
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          tfel::material::OrthotropicAxesConvention::PIPE,
          real> : public ComputeHillTensorBase<2u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<2u, real>::hillTensor(H_H, H_G, H_F, H_M,
                                                             H_L, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,real>
      /*!
       * \brief partial specialisation of the `ComputeHillTensor`
       * class for the:
       * - `OrthotropicAxesConvention::PLATE` orthotropic axes
       *   convention.
       * - `ModellingHypothesis::TRIDIMENSIONAL` modelling hypothesis.
       *
       * \tparam real: numeric type
       */
      template <typename real>
      struct ComputeHillTensor<
          tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
          tfel::material::OrthotropicAxesConvention::PLATE,
          real> : public ComputeHillTensorBase<3u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<3u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<3u, real>::hillTensor(H_F, H_G, H_H, H_L,
                                                             H_M, H_N);
        }  // end of exe
      };   // end of struct ComputeHillTensor
           /*!
            * \brief partial specialisation of the `ComputeHillTensor`
            * class for the:
            * - `OrthotropicAxesConvention::PLATE` orthotropic axes
            *   convention.
            * - `ModellingHypothesis::PLANESTRESS` modelling hypothesis.
            *
            * \tparam real: numeric type
            */
      template <typename real>
      struct ComputeHillTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
                               tfel::material::OrthotropicAxesConvention::PLATE,
                               real> : public ComputeHillTensorBase<2u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<2u, real>::hillTensor(H_F, H_G, H_H, H_L,
                                                             H_M, H_N);
        }  // end of exe
      };   // end of struct ComputeHillTensor
           /*!
            * \brief partial specialisation of the `ComputeHillTensor`
            * class for the:
            * - `OrthotropicAxesConvention::PLATE` orthotropic axes
            *   convention.
            * - `ModellingHypothesis::PLANESTRAIN` modelling hypothesis.
            *
            * \tparam real: numeric type
            */
      template <typename real>
      struct ComputeHillTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
                               tfel::material::OrthotropicAxesConvention::PLATE,
                               real> : public ComputeHillTensorBase<2u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<2u, real>::hillTensor(H_F, H_G, H_H, H_L,
                                                             H_M, H_N);
        }  // end of exe
      };   // end of struct ComputeHillTensor
           /*!
            * \brief partial specialisation of the `ComputeHillTensor`
            * class for the:
            * - `OrthotropicAxesConvention::PLATE` orthotropic axes
            *   convention.
            * - `ModellingHypothesis::GENERALISEDPLANESTRAIN` modelling hypothesis.
            *
            * \tparam real: numeric type
            */
      template <typename real>
      struct ComputeHillTensor<
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          tfel::material::OrthotropicAxesConvention::PLATE,
          real> : public ComputeHillTensorBase<2u, real> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, real>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const real H_F,
                                                    const real H_G,
                                                    const real H_H,
                                                    const real H_L,
                                                    const real H_M,
                                                    const real H_N) {
          return ComputeHillTensorBase<2u, real>::hillTensor(H_F, H_G, H_H, H_L,
                                                             H_M, H_N);
        }  // end of exe
      };   // end of struct ComputeHillTensor

    }  // end of namespace internals

    template <unsigned short N, typename real>
    tfel::math::st2tost2<N, real> hillTensor(const real H_F,
                                             const real H_G,
                                             const real H_H,
                                             const real H_L,
                                             const real H_M,
                                             const real H_N) {
      using namespace tfel::material::internals;
      return ComputeHillTensorBase<N, real>::hillTensor(H_F, H_G, H_H, H_L, H_M,
                                                        H_N);
    }

    template <unsigned short N, typename real>
    tfel::math::st2tost2<N, real> makeHillTensor(const real H_F,
                                                 const real H_G,
                                                 const real H_H,
                                                 const real H_L,
                                                 const real H_M,
                                                 const real H_N) {
      using namespace tfel::material::internals;
      return ComputeHillTensorBase<N, real>::hillTensor(H_F, H_G, H_H, H_L, H_M,
                                                        H_N);
    }  // end of makeHillTensor

    template <ModellingHypothesis::Hypothesis H,
              OrthotropicAxesConvention c,
              typename real>
    tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
    computeHillTensor(const real H_F,
                      const real H_G,
                      const real H_H,
                      const real H_L,
                      const real H_M,
                      const real H_N) {
      using namespace tfel::material::internals;
      return ComputeHillTensor<H, c, real>::exe(H_F, H_G, H_H, H_L, H_M, H_N);
    }  // end of function computeHillTensor

    template <ModellingHypothesis::Hypothesis H,
              OrthotropicAxesConvention c,
              typename real>
    tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, real>
    makeHillTensor(const real H_F,
                   const real H_G,
                   const real H_H,
                   const real H_L,
                   const real H_M,
                   const real H_N) {
      using namespace tfel::material::internals;
      return ComputeHillTensor<H, c, real>::exe(H_F, H_G, H_H, H_L, H_M, H_N);
    }  // end of function makeHillTensor

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_HILL_IXX */
