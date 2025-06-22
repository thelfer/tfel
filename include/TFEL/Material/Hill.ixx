/*!
 * \file   include/TFEL/Material/Hill.ixx
 * \author Helfer Thomas
 * \date   26 March 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_HILL_IXX_
#define LIB_TFEL_MATERIAL_HILL_IXX_

namespace tfel {

  namespace material {

    namespace internals {

      template <unsigned short, typename stress>
      struct ComputeHillTensorBase;

      template <typename stress>
      struct ComputeHillTensorBase<1u, stress> {
        static tfel::math::st2tost2<1u, stress> hillTensor(const stress F,
                                                           const stress G,
                                                           const stress H,
                                                           const stress,
                                                           const stress,
                                                           const stress) {
          return {F + H, -F, -H, -F, G + F, -G, -H, -G, H + G};
        }
      };  // end of struct HillTensor

      template <typename stress>
      struct ComputeHillTensorBase<2u, stress> {
        static tfel::math::st2tost2<2u, stress> hillTensor(const stress F,
                                                           const stress G,
                                                           const stress H,
                                                           const stress L,
                                                           const stress,
                                                           const stress) {
          constexpr const auto zero = stress{0};
          return {F + H, -F, -H,    zero, -F,   G + F, -G,   zero,
                  -H,    -G, H + G, zero, zero, zero,  zero, L};
        }
      };  // end of struct HillTensor

      template <typename stress>
      struct ComputeHillTensorBase<3u, stress> {
        static tfel::math::st2tost2<3u, stress> hillTensor(const stress F,
                                                           const stress G,
                                                           const stress H,
                                                           const stress L,
                                                           const stress M,
                                                           const stress N) {
          constexpr const auto zero = stress{0};
          return {F + H, -F,   -H,   zero, zero, zero,  -F,   G + F, -G,
                  zero,  zero, zero, -H,   -G,   H + G, zero, zero,  zero,
                  zero,  zero, zero, L,    zero, zero,  zero, zero,  zero,
                  zero,  M,    zero, zero, zero, zero,  zero, zero,  N};
        }

      };  // end of struct HillTensor

      template <tfel::material::ModellingHypothesis::Hypothesis,
                tfel::material::OrthotropicAxesConvention,
                typename stress>
      struct ComputeHillTensor;

      template <tfel::material::ModellingHypothesis::Hypothesis H,
                typename stress>
      struct ComputeHillTensor<
          H,
          tfel::material::OrthotropicAxesConvention::DEFAULT,
          stress>
          : public ComputeHillTensorBase<
                tfel::material::ModellingHypothesisToSpaceDimension<H>::value,
                stress> {
        //! \brief space dimension
        static constexpr const auto N =
            tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
        //! \brief return type
        using result_type = tfel::math::st2tost2<N, stress>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const stress H_F,
                                                    const stress H_G,
                                                    const stress H_H,
                                                    const stress H_L,
                                                    const stress H_M,
                                                    const stress H_N) {
          return ComputeHillTensorBase<N, stress>::hillTensor(H_F, H_G, H_H,
                                                              H_L, H_M, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::DEFAULT,stress>

      template <tfel::material::ModellingHypothesis::Hypothesis H,
                typename stress>
      struct ComputeHillTensor<H,
                               tfel::material::OrthotropicAxesConvention::PIPE,
                               stress>
          : public ComputeHillTensorBase<
                tfel::material::ModellingHypothesisToSpaceDimension<H>::value,
                stress> {
        //! \brief space dimension
        static constexpr const unsigned short N =
            tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
        //! \brief return type
        using result_type = tfel::math::st2tost2<N, stress>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const stress H_F,
                                                    const stress H_G,
                                                    const stress H_H,
                                                    const stress H_L,
                                                    const stress H_M,
                                                    const stress H_N) {
          return ComputeHillTensorBase<N, stress>::hillTensor(H_F, H_G, H_H,
                                                              H_L, H_M, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,stress>

      template <typename stress>
      struct ComputeHillTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
                               tfel::material::OrthotropicAxesConvention::PIPE,
                               stress>
          : public ComputeHillTensorBase<2u, stress> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, stress>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const stress H_F,
                                                    const stress H_G,
                                                    const stress H_H,
                                                    const stress H_L,
                                                    const stress H_M,
                                                    const stress H_N) {
          return ComputeHillTensorBase<2u, stress>::hillTensor(H_H, H_G, H_F,
                                                               H_M, H_L, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,stress>

      template <typename stress>
      struct ComputeHillTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
                               tfel::material::OrthotropicAxesConvention::PIPE,
                               stress>
          : public ComputeHillTensorBase<2u, stress> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, stress>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const stress H_F,
                                                    const stress H_G,
                                                    const stress H_H,
                                                    const stress H_L,
                                                    const stress H_M,
                                                    const stress H_N) {
          return ComputeHillTensorBase<2u, stress>::hillTensor(H_H, H_G, H_F,
                                                               H_M, H_L, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,stress>

      template <typename stress>
      struct ComputeHillTensor<
          tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
          tfel::material::OrthotropicAxesConvention::PIPE,
          stress> : public ComputeHillTensorBase<2u, stress> {
        //! \brief return type
        using result_type = tfel::math::st2tost2<2u, stress>;
        /*!
         * \param[in] H_F: orthotropic coefficient
         * \param[in] H_G: orthotropic coefficient
         * \param[in] H_H: orthotropic coefficient
         * \param[in] H_L: orthotropic coefficient
         * \param[in] H_M: orthotropic coefficient
         * \param[in] H_N: orthotropic coefficient
         */
        static TFEL_MATERIAL_INLINE result_type exe(const stress H_F,
                                                    const stress H_G,
                                                    const stress H_H,
                                                    const stress H_L,
                                                    const stress H_M,
                                                    const stress H_N) {
          return ComputeHillTensorBase<2u, stress>::hillTensor(H_H, H_G, H_F,
                                                               H_M, H_L, H_N);
        }  // end of exe
      };   // end of struct
          // ComputeHillTensor<H,tfel::material::OrthotropicAxesConvention::PIPE,stress>

    }  // end of namespace internals

    template <unsigned short N, typename stress>
    tfel::math::st2tost2<N, stress> hillTensor(const stress H_F,
                                               const stress H_G,
                                               const stress H_H,
                                               const stress H_L,
                                               const stress H_M,
                                               const stress H_N) {
      using namespace tfel::material::internals;
      return ComputeHillTensorBase<N, stress>::hillTensor(H_F, H_G, H_H, H_L,
                                                          H_M, H_N);
    }

    template <ModellingHypothesis::Hypothesis H,
              OrthotropicAxesConvention c,
              typename stress>
    tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value, stress>
    computeHillTensor(const stress H_F,
                      const stress H_G,
                      const stress H_H,
                      const stress H_L,
                      const stress H_M,
                      const stress H_N) {
      using namespace tfel::material::internals;
      return ComputeHillTensor<H, c, stress>::exe(H_F, H_G, H_H, H_L, H_M, H_N);
    }  // end of function computeHillTensor

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_HILL_IXX_ */
