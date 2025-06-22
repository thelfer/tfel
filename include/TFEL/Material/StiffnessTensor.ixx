/*!
 * \file   StiffnessTensor.ixx
 * \brief
 * \author Thomas Helfer
 * \date   23 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_STIFFNESSTENSORIXX
#define LIB_TFEL_MATERIAL_STIFFNESSTENSORIXX

#include <algorithm>

namespace tfel {

  namespace material {

    namespace internals {

      template <unsigned short N, StiffnessTensorAlterationCharacteristic smt>
      struct ComputeIsotropicStiffnessTensorI;

      template <unsigned short N, StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensorI;

      template <>
      struct ComputeIsotropicStiffnessTensorI<
          1u,
          StiffnessTensorAlterationCharacteristic::UNALTERED> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<1u, StressType>& C,
            const StressType E,
            const RealType n) {
          const StressType l = E * n / ((1. - 2 * n) * (1 + n));
          const StressType G = E / (1 + n);
          const StressType C11 = l + G;
          C(0, 0) = C11;
          C(0, 1) = C(0, 2) = l;
          C(1, 1) = C11;
          C(1, 0) = C(1, 2) = l;
          C(2, 0) = C(2, 1) = l;
          C(2, 2) = C11;
        }
      };

      template <>
      struct ComputeIsotropicStiffnessTensorI<
          1u,
          StiffnessTensorAlterationCharacteristic::ALTERED> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<1u, StressType>& C,
            const StressType E,
            const RealType n)

        {
          constexpr const StressType zero = StressType(0);
          const StressType C1 = E / (1 - n * n);
          const StressType C2 = n * C1;
          C(0, 0) = C1;
          C(0, 1) = C2;
          C(0, 2) = C(0, 3) = zero;
          C(1, 0) = C2;
          C(1, 1) = C1;
          C(1, 2) = C(1, 3) = zero;
          C(2, 0) = C(2, 1) = zero;
        }  // end of struct computeIsotropicPlaneStressAlteredStiffnessTensor
      };

      template <>
      struct ComputeIsotropicStiffnessTensorI<
          2u,
          StiffnessTensorAlterationCharacteristic::UNALTERED> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<2u, StressType>& C,
            const StressType E,
            const RealType n) {
          constexpr const StressType zero = StressType(0);
          const StressType l = E * n / ((1 - 2 * n) * (1 + n));
          const StressType G = E / (1 + n);
          const StressType C11 = l + G;
          C(0, 0) = C11;
          C(0, 1) = l;
          C(0, 2) = l;
          C(0, 3) = zero;
          C(1, 0) = l;
          C(1, 1) = C11;
          C(1, 2) = l;
          C(1, 3) = zero;
          C(2, 0) = l;
          C(2, 1) = l;
          C(2, 2) = C11;
          C(2, 3) = C(3, 0) = zero;
          C(3, 1) = C(3, 2) = zero;
          C(3, 3) = G;
        }
      };  // end of struct ComputeIsotropicStiffnessTensorI

      template <>
      struct ComputeIsotropicStiffnessTensorI<
          2u,
          StiffnessTensorAlterationCharacteristic::ALTERED> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<2u, StressType>& C,
            const StressType E,
            const RealType n)

        {
          constexpr const StressType zero = StressType(0);
          const StressType C1 = E / (1 - n * n);
          const StressType C2 = n * C1;
          const StressType C3 = (1 - n) * C1;
          C(0, 0) = C1;
          C(0, 1) = C2;
          C(0, 2) = C(0, 3) = zero;
          C(1, 0) = C2;
          C(1, 1) = C1;
          C(1, 2) = C(1, 3) = zero;
          C(2, 0) = C(2, 1) = zero;
          C(2, 2) = C(2, 3) = zero;
          C(3, 0) = C(3, 1) = zero;
          C(3, 2) = zero;
          C(3, 3) = C3;
        }  // end of struct computeIsotropicPlaneStressAlteredStiffnessTensor
      };

      template <StiffnessTensorAlterationCharacteristic smt>
      struct ComputeIsotropicStiffnessTensorI<3u, smt> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<3u, StressType>& C,
            const StressType E,
            const RealType n) {
          constexpr const StressType zero = StressType(0);
          const StressType l = E * n / ((1 - 2 * n) * (1 + n));
          const StressType G = E / (1 + n);
          const StressType C11 = l + G;
          C(0, 0) = C11;
          C(0, 1) = C(0, 2) = l;
          C(0, 3) = C(0, 4) = C(0, 5) = zero;
          C(1, 0) = C(1, 2) = l;
          C(1, 1) = C11;
          C(1, 3) = C(1, 4) = C(1, 5) = zero;
          C(2, 0) = C(2, 1) = l;
          C(2, 2) = C11;
          C(2, 3) = C(2, 4) = C(2, 5) = zero;
          C(3, 0) = C(3, 1) = C(3, 2) = zero;
          C(3, 3) = G;
          C(3, 4) = C(3, 5) = C(4, 0) = zero;
          C(4, 1) = C(4, 2) = C(4, 3) = zero;
          C(4, 4) = G;
          C(4, 5) = C(5, 0) = C(5, 1) = zero;
          C(5, 2) = C(5, 3) = C(5, 4) = zero;
          C(5, 5) = G;
        }  // end of struct computeStiffnessTensor
      };

      template <>
      struct ComputeOrthotropicStiffnessTensorI<
          1u,
          StiffnessTensorAlterationCharacteristic::UNALTERED> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<1u, StressType>& C,
            const StressType E1,
            const StressType E2,
            const StressType E3,
            const RealType n12,
            const RealType n23,
            const RealType n13,
            const StressType,
            const StressType,
            const StressType) {
          const auto S11 = 1 / E1;
          const auto S22 = 1 / E2;
          const auto S33 = 1 / E3;
          const auto S12 = -n12 / E1;
          const auto S13 = -n13 / E1;
          const auto S23 = -n23 / E2;
          const auto inv_detS =
              1 / (S11 * S22 * S33 + 2 * S23 * S13 * S12 - S11 * S23 * S23 -
                   S22 * S13 * S13 - S33 * S12 * S12);
          C(0, 0) = (S22 * S33 - S23 * S23) * inv_detS;
          C(0, 1) = (S13 * S23 - S12 * S33) * inv_detS;
          C(0, 2) = (S12 * S23 - S13 * S22) * inv_detS;
          C(1, 0) = (S13 * S23 - S12 * S33) * inv_detS;
          C(1, 1) = (S11 * S33 - S13 * S13) * inv_detS;
          C(1, 2) = (S12 * S13 - S11 * S23) * inv_detS;
          C(2, 0) = (S12 * S23 - S13 * S22) * inv_detS;
          C(2, 1) = (S12 * S13 - S11 * S23) * inv_detS;
          C(2, 2) = (S11 * S22 - S12 * S12) * inv_detS;
        }
      };

      template <>
      struct ComputeOrthotropicStiffnessTensorI<
          1u,
          StiffnessTensorAlterationCharacteristic::ALTERED> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<1u, StressType>& C,
            const StressType E1,
            const StressType E2,
            const StressType E3,
            const RealType n12,
            const RealType n23,
            const RealType n13,
            const StressType,
            const StressType,
            const StressType) {
          const auto S11 = 1 / E1;
          const auto S22 = 1 / E2;
          const auto S33 = 1 / E3;
          const auto S12 = -n12 / E1;
          const auto S13 = -n13 / E1;
          const auto S23 = -n23 / E2;
          const auto inv_detS =
              1 / (S11 * S22 * S33 + 2 * S23 * S13 * S12 - S11 * S23 * S23 -
                   S22 * S13 * S13 - S33 * S12 * S12);
          const auto C00 = (S22 * S33 - S23 * S23) * inv_detS;
          const auto C01 = (S13 * S23 - S12 * S33) * inv_detS;
          const auto C02 = (S12 * S23 - S13 * S22) * inv_detS;
          const auto C10 = (S13 * S23 - S12 * S33) * inv_detS;
          const auto C11 = (S11 * S33 - S13 * S13) * inv_detS;
          const auto C12 = (S12 * S13 - S11 * S23) * inv_detS;
          const auto C20 = (S12 * S23 - S13 * S22) * inv_detS;
          const auto C21 = (S12 * S13 - S11 * S23) * inv_detS;
          const auto C22 = (S11 * S22 - S12 * S12) * inv_detS;
          const auto tmp20 = C20 / C22;
          const auto tmp21 = C21 / C22;
          std::fill(C.begin(), C.end(), StressType(0.));
          C(0, 0) = C00 - C02 * tmp20;
          C(0, 1) = C01 - C02 * tmp21;
          C(1, 0) = C10 - C12 * tmp20;
          C(1, 1) = C11 - C12 * tmp21;
        }  // end of exe
      };

      template <>
      struct ComputeOrthotropicStiffnessTensorI<
          2u,
          StiffnessTensorAlterationCharacteristic::UNALTERED> {
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
            const StressType,
            const StressType) {
          const auto S11 = 1 / E1;
          const auto S22 = 1 / E2;
          const auto S33 = 1 / E3;
          const auto S12 = -n12 / E1;
          const auto S13 = -n13 / E1;
          const auto S23 = -n23 / E2;
          const auto inv_detS =
              1 / (S11 * S22 * S33 + 2 * S23 * S13 * S12 - S11 * S23 * S23 -
                   S22 * S13 * S13 - S33 * S12 * S12);
          std::fill(C.begin(), C.end(), StressType(0.));
          C(0, 0) = (S22 * S33 - S23 * S23) * inv_detS;
          C(0, 1) = (S13 * S23 - S12 * S33) * inv_detS;
          C(0, 2) = (S12 * S23 - S13 * S22) * inv_detS;
          C(1, 0) = (S13 * S23 - S12 * S33) * inv_detS;
          C(1, 1) = (S11 * S33 - S13 * S13) * inv_detS;
          C(1, 2) = (S12 * S13 - S11 * S23) * inv_detS;
          C(2, 0) = (S12 * S23 - S13 * S22) * inv_detS;
          C(2, 1) = (S12 * S13 - S11 * S23) * inv_detS;
          C(2, 2) = (S11 * S22 - S12 * S12) * inv_detS;
          C(3, 3) = 2 * G12;
        }  // end of exe
      };

      template <>
      struct ComputeOrthotropicStiffnessTensorI<
          2u,
          StiffnessTensorAlterationCharacteristic::ALTERED> {
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
            const StressType,
            const StressType) {
          const auto S11 = 1 / E1;
          const auto S22 = 1 / E2;
          const auto S33 = 1 / E3;
          const auto S12 = -n12 / E1;
          const auto S13 = -n13 / E1;
          const auto S23 = -n23 / E2;
          const auto inv_detS =
              1 / (S11 * S22 * S33 + 2 * S23 * S13 * S12 - S11 * S23 * S23 -
                   S22 * S13 * S13 - S33 * S12 * S12);
          const auto C00 = (S22 * S33 - S23 * S23) * inv_detS;
          const auto C01 = (S13 * S23 - S12 * S33) * inv_detS;
          const auto C02 = (S12 * S23 - S13 * S22) * inv_detS;
          const auto C10 = (S13 * S23 - S12 * S33) * inv_detS;
          const auto C11 = (S11 * S33 - S13 * S13) * inv_detS;
          const auto C12 = (S12 * S13 - S11 * S23) * inv_detS;
          const auto C20 = (S12 * S23 - S13 * S22) * inv_detS;
          const auto C21 = (S12 * S13 - S11 * S23) * inv_detS;
          const auto C22 = (S11 * S22 - S12 * S12) * inv_detS;
          const auto tmp20 = C20 / C22;
          const auto tmp21 = C21 / C22;
          std::fill(C.begin(), C.end(), StressType(0.));
          C(0, 0) = C00 - C02 * tmp20;
          C(0, 1) = C01 - C02 * tmp21;
          C(1, 0) = C10 - C12 * tmp20;
          C(1, 1) = C11 - C12 * tmp21;
          C(3, 3) = 2 * G12;
        }  // end of exe
      };

      template <StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensorI<3u, smt> {
        template <typename StressType, typename RealType>
        static TFEL_MATERIAL_INLINE void exe(
            tfel::math::st2tost2<3u, StressType>& C,
            const StressType E1,
            const StressType E2,
            const StressType E3,
            const RealType n12,
            const RealType n23,
            const RealType n13,
            const StressType G12,
            const StressType G23,
            const StressType G13) {
          const auto S11 = 1 / E1;
          const auto S22 = 1 / E2;
          const auto S33 = 1 / E3;
          const auto S12 = -n12 / E1;
          const auto S13 = -n13 / E1;
          const auto S23 = -n23 / E2;
          const auto inv_detS =
              1 / (S11 * S22 * S33 + 2 * S23 * S13 * S12 - S11 * S23 * S23 -
                   S22 * S13 * S13 - S33 * S12 * S12);
          std::fill(C.begin(), C.end(), StressType(0.));
          C(0, 0) = (S22 * S33 - S23 * S23) * inv_detS;
          C(1, 1) = (S11 * S33 - S13 * S13) * inv_detS;
          C(2, 2) = (S11 * S22 - S12 * S12) * inv_detS;
          C(0, 1) = C(1, 0) = (S13 * S23 - S12 * S33) * inv_detS;
          C(0, 2) = C(2, 0) = (S12 * S23 - S13 * S22) * inv_detS;
          C(1, 2) = C(2, 1) = (S12 * S13 - S11 * S23) * inv_detS;
          C(3, 3) = 2 * G12;
          C(4, 4) = 2 * G13;
          C(5, 5) = 2 * G23;
        }  // end of struct exe
      };

      template <ModellingHypothesis::Hypothesis H,
                StiffnessTensorAlterationCharacteristic>
      struct ComputeIsotropicStiffnessTensorII
          : public ComputeIsotropicStiffnessTensorI<
                ModellingHypothesisToSpaceDimension<H>::value,
                StiffnessTensorAlterationCharacteristic::UNALTERED> {};

      template <>
      struct ComputeIsotropicStiffnessTensorII<
          ModellingHypothesis::PLANESTRESS,
          StiffnessTensorAlterationCharacteristic::ALTERED>
          : public ComputeIsotropicStiffnessTensorI<
                2u,
                StiffnessTensorAlterationCharacteristic::ALTERED> {};

      template <>
      struct ComputeIsotropicStiffnessTensorII<
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
          StiffnessTensorAlterationCharacteristic::ALTERED>
          : public ComputeIsotropicStiffnessTensorI<
                1u,
                StiffnessTensorAlterationCharacteristic::ALTERED> {};

      template <ModellingHypothesis::Hypothesis H,
                StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensorII
          : public ComputeOrthotropicStiffnessTensorI<
                ModellingHypothesisToSpaceDimension<H>::value,
                StiffnessTensorAlterationCharacteristic::UNALTERED> {};

      template <>
      struct ComputeOrthotropicStiffnessTensorII<
          ModellingHypothesis::PLANESTRESS,
          StiffnessTensorAlterationCharacteristic::ALTERED>
          : public ComputeOrthotropicStiffnessTensorI<
                2u,
                StiffnessTensorAlterationCharacteristic::ALTERED> {};

      template <>
      struct ComputeOrthotropicStiffnessTensorII<
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
          StiffnessTensorAlterationCharacteristic::ALTERED>
          : public ComputeOrthotropicStiffnessTensorI<
                1u,
                StiffnessTensorAlterationCharacteristic::ALTERED> {};

      /*!
       * \brief class in charge of computing an orthotropic stiffness
       * tensor according to:
       * - The modelling hypothesis.
       * - The choice of computing an altered or unaltered stiffness
       *   tensor. This parameter is only useful in plane stress
       *   modelling hypotheses.
       * - The orthotropic axes convention.
       * \tparam H: modelling hypothesis
       * \tparam smt: stiffness matrix alteration choice
       * \tparam c: orthotropic axis convention
       */
      template <ModellingHypothesis::Hypothesis,
                StiffnessTensorAlterationCharacteristic,
                OrthotropicAxesConvention>
      struct ComputeOrthotropicStiffnessTensor;
      /*!
       * \brief partial specialisation for the
       * `OrthotropicAxesConvention::DEFAULT` orthotropic axes
       * convention.
       * \tparam H: modelling hypothesis
       * \tparam smt: stiffness matrix alteration choice
       */
      template <ModellingHypothesis::Hypothesis H,
                StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensor<
          H,
          smt,
          OrthotropicAxesConvention::DEFAULT>
          : public ComputeOrthotropicStiffnessTensorII<H, smt> {};
      /*!
       * \brief partial specialisation for the
       * `OrthotropicAxesConvention::PIPE` orthotropic axes
       * convention.
       * \tparam H: modelling hypothesis
       * \tparam smt: stiffness matrix alteration choice
       */
      template <ModellingHypothesis::Hypothesis H,
                StiffnessTensorAlterationCharacteristic smt>
      struct ComputeOrthotropicStiffnessTensor<H,
                                               smt,
                                               OrthotropicAxesConvention::PIPE>
          : public ComputeOrthotropicStiffnessTensorII<H, smt> {};
      /*!
       * \brief partial specialisation for the:
       * - `ModellingHypothesis::PLANESTRESS` modelling hypothesis.
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       * convention.
       * \tparam smt: stiffness matrix alteration choice
       */
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
      /*!
       * \brief partial specialisation for the:
       * - `ModellingHypothesis::PLANESTRAIN` modelling hypothesis.
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       * convention.
       * \tparam smt: stiffness matrix alteration choice
       */
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
      /*!
       * \brief partial specialisation for the:
       * - `ModellingHypothesis::GENERALISEDPLANESTRAIN` modelling
       *   hypothesis.
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       * convention.
       * \tparam smt: stiffness matrix alteration choice
       */
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

    template <ModellingHypothesis::Hypothesis H>
    template <typename StressType>
    void ComputeAlteredStiffnessTensor<H>::exe(
        tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                             StressType>& Da,
        const tfel::math::st2tost2<
            ModellingHypothesisToSpaceDimension<H>::value,
            StressType>& D) {
      Da = D;
    }

    template <typename StressType>
    void ComputeAlteredStiffnessTensor<ModellingHypothesis::PLANESTRESS>::exe(
        tfel::math::st2tost2<2u, StressType>& Da,
        const tfel::math::st2tost2<2u, StressType>& D) {
      Da(0, 0) = D(0, 0) - D(2, 0) / D(2, 2) * D(0, 2);
      Da(0, 1) = D(0, 1) - D(2, 1) / D(2, 2) * D(0, 2);
      Da(1, 0) = D(1, 0) - D(2, 0) / D(2, 2) * D(1, 2);
      Da(1, 1) = D(1, 1) - D(2, 1) / D(2, 2) * D(1, 2);
      Da(0, 2) = StressType(0);
      Da(1, 2) = StressType(0);
      Da(2, 0) = StressType(0);
      Da(2, 1) = StressType(0);
      Da(2, 2) = StressType(0);
      Da(0, 3) = StressType(0);
      Da(1, 3) = StressType(0);
      Da(2, 3) = StressType(0);
      Da(3, 0) = StressType(0);
      Da(3, 1) = StressType(0);
      Da(3, 2) = StressType(0);
      Da(3, 3) = D(3, 3);
    }  // end of ComputeAlteredStiffnessTensor

    template <unsigned short N,
              StiffnessTensorAlterationCharacteristic smt,
              typename StressType,
              typename RealType>
    void computeIsotropicStiffnessTensorII(
        tfel::math::st2tost2<N, StressType>& C,
        const StressType E,
        const RealType n) {
      tfel::material::internals::ComputeIsotropicStiffnessTensorI<N, smt>::exe(
          C, E, n);
    }

    template <unsigned short N,
              StiffnessTensorAlterationCharacteristic smt,
              typename StressType,
              typename RealType>
    void computeOrthotropicStiffnessTensorII(
        tfel::math::st2tost2<N, StressType>& C,
        const StressType E1,
        const StressType E2,
        const StressType E3,
        const RealType n12,
        const RealType n23,
        const RealType n13,
        const StressType G12,
        const StressType G23,
        const StressType G13) {
      tfel::material::internals::ComputeOrthotropicStiffnessTensorI<
          N, smt>::exe(C, E1, E2, E3, n12, n23, n13, G12, G23, G13);
    }

    template <ModellingHypothesis::Hypothesis H,
              StiffnessTensorAlterationCharacteristic smt,
              typename StressType,
              typename RealType>
    void computeIsotropicStiffnessTensor(
        tfel::math::st2tost2<ModellingHypothesisToSpaceDimension<H>::value,
                             StressType>& C,
        const StressType E,
        const RealType n) {
      tfel::material::internals::ComputeIsotropicStiffnessTensorII<H, smt>::exe(
          C, E, n);
    }

    template <ModellingHypothesis::Hypothesis H,
              StiffnessTensorAlterationCharacteristic smt,
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
      tfel::material::internals::ComputeOrthotropicStiffnessTensorII<
          H, smt>::exe(C, E1, E2, E3, n12, n23, n13, G12, G23, G13);
    }

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

#endif /* LIB_TFEL_MATERIAL_STIFFNESSTENSORIXX */
