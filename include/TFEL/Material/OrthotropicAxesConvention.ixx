/*!
 * \file   OrthotropicAxesConvention.ixx
 * \brief
 * \author THOMAS HELFER
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX

namespace tfel {

  namespace material {

    namespace internals {

      /*!
       * an helper structure to perform template specialisation
       */
      template <ModellingHypothesis::Hypothesis mh, OrthotropicAxesConvention c>
      struct OrthotropicAxesConventionConverter {
        //! a simple alias
        static constexpr unsigned short N =
            ModellingHypothesisToSpaceDimension<mh>::value;
        /*!
         * convert a symmetric tensor to the new convention
         */
        template <typename T>
        static TFEL_MATERIAL_INLINE void convert(tfel::math::stensor<N, T>&) {
        }  // end of convert
      };   // end of struct OrthotropicAxesConventionConverter

      /*!
       * an helper structure used in "*plane*" generalised modelling
       * hypotheses to perform convertion
       */
      struct PipeOrthotropicAxesConventionConverter {
        /*!
         * convert a symmetric tensor to the new convention
         * \param[in] s : the tensor to be converted
         */
        template <typename T>
        static TFEL_MATERIAL_INLINE void convert(
            tfel::math::stensor<2u, T>& s) {
          std::swap(s[1], s[2]);
        }  // end of convert
      };

      /*!
       *  partial specialisation for the PLANESTRESS modelling
       * hypothesis and PIPE orthotropic axes convention
       */
      template <>
      struct OrthotropicAxesConventionConverter<
          ModellingHypothesis::PLANESTRESS,
          OrthotropicAxesConvention::PIPE>
          : public PipeOrthotropicAxesConventionConverter {
      };  // end of struct OrthotropicAxesConventionConverter

      /*!
       *  partial specialisation for the PLANESTRAIN modelling
       * hypothesis and PIPE orthotropic axes convention
       */
      template <>
      struct OrthotropicAxesConventionConverter<
          ModellingHypothesis::PLANESTRAIN,
          OrthotropicAxesConvention::PIPE>
          : public PipeOrthotropicAxesConventionConverter {
      };  // end of struct OrthotropicAxesConventionConverter

      /*!
       *  partial specialisation for the GENERALISEDPLANESTRAIN
       * modelling hypothesis and PIPE orthotropic axes convention
       */
      template <>
      struct OrthotropicAxesConventionConverter<
          ModellingHypothesis::GENERALISEDPLANESTRAIN,
          OrthotropicAxesConvention::PIPE>
          : public PipeOrthotropicAxesConventionConverter {
      };  // end of struct OrthotropicAxesConventionConverter

      template <ModellingHypothesis::Hypothesis H,
                StiffnessTensorAlterationCharacteristic smt,
                OrthotropicAxesConvention c>
      struct ComputeOrthotropicStiffnessTensor
          : public ComputeOrthotropicStiffnessTensorII<H, smt> {};

      template <StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensor<
          ModellingHypothesis::PLANESTRESS,
          smt,
          OrthotropicAxesConvention::PIPE> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<2u, StressType>& C,
            const StressType E1,
            const StressType E2,
            const StressType E3,
            const RealType n12,
            const RealType n23,
            const RealType n13,
            const StressType G12,
            const StressType G23,
            const StressType G13) {
          using COST = ComputeOrthotropicStiffnessTensorII<
              ModellingHypothesis::PLANESTRESS, smt>;
          COST::exe(C, E1, E3, E2, n13, n23 * E3 / E2, n12, G13, G23, G12);
        }
      };

      template <StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensor<
          ModellingHypothesis::PLANESTRAIN,
          smt,
          OrthotropicAxesConvention::PIPE> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<2u, StressType>& C,
            const StressType E1,
            const StressType E2,
            const StressType E3,
            const RealType n12,
            const RealType n23,
            const RealType n13,
            const StressType G12,
            const StressType G23,
            const StressType G13) {
          using COST = ComputeOrthotropicStiffnessTensorII<
              ModellingHypothesis::PLANESTRAIN, smt>;
          COST::exe(C, E1, E3, E2, n13, n23 * E3 / E2, n12, G13, G23, G12);
        }
      };

      template <StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensor<
          ModellingHypothesis::GENERALISEDPLANESTRAIN,
          smt,
          OrthotropicAxesConvention::PIPE> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<2u, StressType>& C,
            const StressType E1,
            const StressType E2,
            const StressType E3,
            const RealType n12,
            const RealType n23,
            const RealType n13,
            const StressType G12,
            const StressType G23,
            const StressType G13) {
          using COST = ComputeOrthotropicStiffnessTensorII<
              ModellingHypothesis::GENERALISEDPLANESTRAIN, smt>;
          COST::exe(C, E1, E3, E2, n13, n23 * E3 / E2, n12, G13, G23, G12);
        }
      };

    }  // end of namespace internals

    template <ModellingHypothesis::Hypothesis mh,
              OrthotropicAxesConvention c,
              typename T>
    void convertStressFreeExpansionStrain(
        tfel::math::stensor<ModellingHypothesisToSpaceDimension<mh>::value, T>&
            s) {
      internals::OrthotropicAxesConventionConverter<mh, c>::convert(s);
    }  // end of convert

    template <ModellingHypothesis::Hypothesis H,
              StiffnessTensorAlterationCharacteristic smt,
              OrthotropicAxesConvention c,
              typename StressType,
              typename RealType>
    void computeOrthotropicStiffnessTensor(
        tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                             StressType>& C,
        const StressType E1,
        const StressType E2,
        const StressType E3,
        const RealType n12,
        const RealType n23,
        const RealType n13,
        const StressType G12,
        const StressType G23,
        const StressType G13) {
      internals::ComputeOrthotropicStiffnessTensor<H, smt, c>::exe(
          C, E1, E2, E3, n12, n23, n13, G12, G23, G13);
    }

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX */
