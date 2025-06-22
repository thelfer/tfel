/*!
 * \file   LogarithmicStrainHandler.ixx
 * \brief
 * \author Thomas Helfer
 * \date   25 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_IXX
#define LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_IXX

#include <cmath>
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

namespace tfel {

  namespace material {

    LogarithmicStrainHandlerBase::LogarithmicStrainHandlerBase(const Setting c)
        : s(c) {
    }  // end of LogarithmicStrainHandlerBase::LogarithmicStrainHandlerBase

    template <typename StressType>
    LogarithmicStrainHandler<1u, StressType>::LogarithmicStrainHandler(
        const Setting c, const DeformationGradient& F1)
        : LogarithmicStrainHandlerBase(c), F(F1) {
      this->e = {std::log(this->F[0]), std::log(this->F[1]),
                 std::log(this->F[2])};
    }  // end of
       // LogarithmicStrainHandler<1u,StressTupe>::LogarithmicStrainHandler

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::StrainStensor
    LogarithmicStrainHandler<1u, StressType>::getHenckyLogarithmicStrain()
        const {
      return {this->e[0], this->e[1], this->e[2]};
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::getHenckyLogarithmicStrain

    template <typename StressType>
    void LogarithmicStrainHandler<1u, StressType>::getHenckyLogarithmicStrain(
        real* const elog) const {
      elog[0] = this->e[0];
      elog[1] = this->e[1];
      elog[2] = this->e[2];
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::getHenckyLogarithmicStrain

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::StressStensor
    LogarithmicStrainHandler<1u, StressType>::
        convertFromSecondPiolaKirchhoffStress(const StressStensor& S) const {
      return {S[0] * (this->F[0] * this->F[0]),
              S[1] * (this->F[1] * this->F[1]),
              S[2] * (this->F[2] * this->F[2])};
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertFromSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<1u, StressType>::
        convertFromSecondPiolaKirchhoffStress(stress* const ST) const {
      ST[0] *= this->F[0] * this->F[0];
      ST[1] *= this->F[1] * this->F[1];
      ST[2] *= this->F[2] * this->F[2];
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertFromSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::StressStensor
    LogarithmicStrainHandler<1u, StressType>::
        convertToSecondPiolaKirchhoffStress(const StressStensor& T) const {
      return {T[0] / (this->F[0] * this->F[0]),
              T[1] / (this->F[1] * this->F[1]),
              T[2] / (this->F[2] * this->F[2])};
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<1u, StressType>::
        convertToSecondPiolaKirchhoffStress(stress* const TS) const {
      TS[0] /= this->F[0] * this->F[0];
      TS[1] /= this->F[1] * this->F[1];
      TS[2] /= this->F[2] * this->F[2];
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::StressStensor
    LogarithmicStrainHandler<1u, StressType>::convertToCauchyStress(
        const StressStensor& T) const {
      return T / tfel::math::det(this->F);
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<1u, StressType>::convertToCauchyStress(
        stress* const T) const {
      const auto iJ = 1 / tfel::math::det(this->F);
      T[0] *= iJ;
      T[1] *= iJ;
      T[2] *= iJ;
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::StressStensor
    LogarithmicStrainHandler<1u, StressType>::convertFromCauchyStress(
        const StressStensor& sig) const {
      return sig * tfel::math::det(this->F);
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertFromCauchyStress

    template <typename StressType>
    void LogarithmicStrainHandler<1u, StressType>::convertFromCauchyStress(
        stress* const T) const {
      const auto J = tfel::math::det(this->F);
      T[0] *= J;
      T[1] *= J;
      T[2] *= J;
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::TangentOperator
    LogarithmicStrainHandler<1u, StressType>::convertToMaterialTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      const real iC[3] = {1 / ((this->F[0]) * (this->F[0])),
                          1 / ((this->F[1]) * (this->F[1])),
                          1 / ((this->F[2]) * (this->F[2]))};
      auto Kr = TangentOperator();
      Kr(0, 0) = (Ks(0, 0) - 2 * T[0]) * iC[0] * iC[0];
      Kr(0, 1) = Ks(0, 1) * iC[0] * iC[1];
      Kr(0, 2) = Ks(0, 2) * iC[0] * iC[2];
      Kr(1, 0) = Ks(1, 0) * iC[0] * iC[1];
      Kr(1, 1) = (Ks(1, 1) - 2 * T[1]) * iC[1] * iC[1];
      Kr(1, 2) = Ks(1, 2) * iC[1] * iC[2];
      Kr(2, 0) = Ks(2, 0) * iC[0] * iC[2];
      Kr(2, 1) = Ks(2, 1) * iC[1] * iC[2];
      Kr(2, 2) = (Ks(2, 2) - 2 * T[2]) * iC[2] * iC[2];
      return Kr;
    }

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::TangentOperator
    LogarithmicStrainHandler<1u, StressType>::convertToSpatialTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      auto Kr = TangentOperator();
      Kr(0, 0) = (Ks(0, 0) - 2 * T[0]);
      Kr(0, 1) = Ks(0, 1);
      Kr(0, 2) = Ks(0, 2);
      Kr(1, 0) = Ks(1, 0);
      Kr(1, 1) = (Ks(1, 1) - 2 * T[1]);
      Kr(1, 2) = Ks(1, 2);
      Kr(2, 0) = Ks(2, 0);
      Kr(2, 1) = Ks(2, 1);
      Kr(2, 2) = (Ks(2, 2) - 2 * T[2]);
      return Kr;
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToSpatialTangentModuli

    template <typename StressType>
    typename LogarithmicStrainHandler<1u, StressType>::TangentOperator
    LogarithmicStrainHandler<1u, StressType>::
        convertToCauchyStressTruesdellRateTangentModuli(
            const TangentOperator& Ks, const StressStensor& T) const {
      const auto iJ = 1 / tfel::math::det(this->F);
      auto Kr = TangentOperator();
      Kr(0, 0) = (Ks(0, 0) - 2 * T[0]) * iJ;
      Kr(0, 1) = Ks(0, 1) * iJ;
      Kr(0, 2) = Ks(0, 2) * iJ;
      Kr(1, 0) = Ks(1, 0) * iJ;
      Kr(1, 1) = (Ks(1, 1) - 2 * T[1]) * iJ;
      Kr(1, 2) = Ks(1, 2) * iJ;
      Kr(2, 0) = Ks(2, 0) * iJ;
      Kr(2, 1) = Ks(2, 1) * iJ;
      Kr(2, 2) = (Ks(2, 2) - 2 * T[2]) * iJ;
      return Kr;
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToCauchyStressTruesdellRateTangentModuli

    template <typename StressType>
    void LogarithmicStrainHandler<1u, StressType>::
        convertToCauchyStressTruesdellRateTangentModuli(
            stress* const K, const stress* const T) const {
      const auto iJ = 1 / tfel::math::det(this->F);
      tfel::math::ST2toST2View<1u, stress> Kr(K);
      auto transpose = [&Kr]() {
        std::swap(Kr(0, 1), Kr(1, 0));
        std::swap(Kr(0, 2), Kr(2, 0));
        std::swap(Kr(1, 2), Kr(2, 1));
      };
      transpose();
      Kr(0, 0) = (Kr(0, 0) - 2 * T[0]) * iJ;
      Kr(0, 1) *= iJ;
      Kr(0, 2) *= iJ;
      Kr(1, 0) *= iJ;
      Kr(1, 1) = (Kr(1, 1) - 2 * T[1]) * iJ;
      Kr(1, 2) *= iJ;
      Kr(2, 0) *= iJ;
      Kr(2, 1) *= iJ;
      Kr(2, 2) = (Kr(2, 2) - 2 * T[2]) * iJ;
      transpose();
    }  // end of
       // LogarithmicStrainHandler<1u,StressType>::convertToCauchyStressTruesdellRateTangentModuli

    template <typename StressType>
    LogarithmicStrainHandler<2u, StressType>::LogarithmicStrainHandler(
        const Setting c, const DeformationGradient& F1, const bool b)
        : LogarithmicStrainHandler(Builder(c, F1, b), c, F1) {
    }  // end of
       // LogarithmicStrainHandler<2u,StressTupe>::LogarithmicStrainHandler

    template <typename StressType>
    LogarithmicStrainHandler<2u, StressType>::Builder::Builder(
        const Setting c, const DeformationGradient& F1, const bool b) {
      using namespace tfel::math;
      using stensor = stensor<2u, real>;
      const auto fl = [](const real x) { return std::log1p(x - 1) / 2; };
      const auto dfl = [](const real x) { return 1 / (2 * x); };
      const auto C = computeRightCauchyGreenTensor(F1);
      std::tie(this->vp, this->m) =
          C.template computeEigenVectors<stensor::FSESJACOBIEIGENSOLVER>();
      if (b) {
        this->e = map(fl, this->vp);
      } else {
        this->e[0] = fl(this->vp(0));
        this->e[1] = fl(this->vp(1));
        this->e[2] = real(0);
      }
      const auto d = map(dfl, this->vp);
      if (c == LAGRANGIAN) {
        // p is one half of the tensor defined by Miehe
        this->p = stensor::computeIsotropicFunctionDerivative(
            this->e, d, this->vp, this->m, eps);
      } else {
        const auto theta = [this, &d] {
          if (std::abs(this->vp[0] - this->vp[1]) < eps) {
            return (d[0] + d[1]) / 2;
          }
          return (this->e[0] - this->e[1]) / (this->vp[0] - this->vp[1]);
        }();
        const auto N = getNTensors(m);
        const auto M = getEulerianMTensors(m, F1);
        this->p = (d[0] * (N(0) ^ M(0)) + d[1] * (N(1) ^ M(1))) / 4;
        this->p(2, 2) = (d[2] / 4) * ((N(2))(2)) * ((M(2))(2));
        this->p += (theta / 2) * (N(3) ^ M(3));
      }
    }

    template <typename StressType>
    LogarithmicStrainHandler<2u, StressType>::LogarithmicStrainHandler(
        Builder&& src, const Setting c, const DeformationGradient& F1)
        : LogarithmicStrainHandlerBase(c),
          p(src.p),
          F(F1),
          m(src.m),
          vp(src.vp),
          e(src.e) {
    }  // end of
       // LogarithmicStrainHandler<2u,StressTupe>::LogarithmicStrainHandler

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::StrainStensor
    LogarithmicStrainHandler<2u, StressType>::getHenckyLogarithmicStrain()
        const {
      return StrainStensor::computeIsotropicFunction(this->e, this->m);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::getHenckyLogarithmicStrain

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::getHenckyLogarithmicStrain(
        real* const elog) const {
      const auto el = StrainStensor::computeIsotropicFunction(this->e, this->m);
      tfel::math::internals::ExportToVoigt<2u>::exe(&el[0], elog);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::getHenckyLogarithmicStrain

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::StressStensor
    LogarithmicStrainHandler<2u, StressType>::
        convertFromSecondPiolaKirchhoffStress(const StressStensor& S) const {
      this->checkLagrangianSetting();
      return (S | tfel::math::invert(p)) / 2;
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertFromSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::
        convertFromSecondPiolaKirchhoffStress(stress* const ST) const {
      this->checkLagrangianSetting();
      tfel::math::stensor<2u, stress> S;
      S.importTab(ST);
      const auto T = this->convertFromSecondPiolaKirchhoffStress(S);
      T.exportTab(ST);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertFromSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::StressStensor
    LogarithmicStrainHandler<2u, StressType>::
        convertToSecondPiolaKirchhoffStress(const StressStensor& T) const {
      this->checkLagrangianSetting();
      return 2 * (T | p);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::
        convertToSecondPiolaKirchhoffStress(stress* const TS) const {
      this->checkLagrangianSetting();
      tfel::math::stensor<2u, stress> T;
      T.importTab(TS);
      const auto S = this->convertToSecondPiolaKirchhoffStress(T);
      S.exportTab(TS);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::StressStensor
    LogarithmicStrainHandler<2u, StressType>::convertFromCauchyStress(
        const StressStensor& sig) const {
      if (this->s == LAGRANGIAN) {
        const auto S =
            tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(
                sig, this->F);
        return this->convertFromSecondPiolaKirchhoffStress(S);
      } else {
        return (sig | tfel::math::invert(p)) * tfel::math::det(this->F) / 2;
      }
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertFromCauchyStress

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::convertFromCauchyStress(
        stress* const sT) const {
      StressStensor sig;
      sig.importTab(sT);
      this->convertFromCauchyStress(sig).exportTab(sT);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertFromCauchyStress

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::StressStensor
    LogarithmicStrainHandler<2u, StressType>::convertToCauchyStress(
        const StressStensor& T) const {
      if (this->s == LAGRANGIAN) {
        const auto S = this->convertToSecondPiolaKirchhoffStress(T);
        return tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(
            S, this->F);
      }
      return (2 * (T | p)) / tfel::math::det(this->F);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::convertToCauchyStress(
        stress* const Ts) const {
      StressStensor T;
      T.importTab(Ts);
      this->convertToCauchyStress(T).exportTab(Ts);
    }  // end of LogarithmicStrainHandler<2u,StressType>::convertToCauchyStress

    template <typename StressType>
    void
    LogarithmicStrainHandler<2u, StressType>::updateAxialDeformationGradient(
        const real Fzz) {
      this->F[2] = Fzz;
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::updateAxialDeformationGradient

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::convertTangentModuli(
        TangentOperator& Kr,
        const TangentOperator& Ks,
        const StressStensor& T,
        const tfel::math::tvector<4u, tfel::math::stensor<2u, real>>& N,
        const tfel::math::tvector<4u, tfel::math::stensor<2u, real>>& M) const {
      using namespace tfel::math;
      // half compared to Miehe definition
      const auto d = map([](const real x) { return 1 / (2 * x); }, this->vp);
      // compute the derivative of the Hencky strain with respect to C
      const auto f = map([](const real x) { return -2 / (x * x); }, this->vp);
      const auto xsi = [this, &d, &f]() -> tvector<2u, real> {
        if (std::abs(this->vp[0] - this->vp[1]) < eps) {
          const auto rv = (f[0] + f[1]) / 16;
          return {rv, rv};
        }
        const auto idvp = 1 / (this->vp[0] - this->vp[1]);
        return {((this->e[0] - this->e[1]) * idvp - d[1]) * idvp,
                -((this->e[0] - this->e[1]) * idvp - d[0]) * idvp};
      }();
      const tvector<4u, real> dzeta = {(T | N(0)) / 2, (T | N(1)) / 2, T[2],
                                       (T | N(3)) / 2};
#if (!defined __INTEL_COMPILER) && (!defined __PGI)
      Kr = 4 * transpose(p) * Ks * p +
           (f[0] * dzeta(0) * (M(0) ^ M(0)) + f[1] * dzeta(1) * (M(1) ^ M(1)) +
            f[2] * dzeta(2) * (M(2) ^ M(2))) /
               4;
#else
      const st2tost2<2u, real> tp = transpose(p);
      Kr = 4 * tp * Ks * p +
           (f[0] * dzeta(0) * (M(0) ^ M(0)) + f[1] * dzeta(1) * (M(1) ^ M(1)) +
            f[2] * dzeta(2) * (M(2) ^ M(2))) /
               4;
#endif
      Kr += 2 * xsi[0] *
            (dzeta(3) * ((M(3) ^ M(1)) + (M(1) ^ M(3))) +
             dzeta(1) * (M(3) ^ M(3)));
      Kr += 2 * xsi[1] *
            (dzeta(3) * ((M(3) ^ M(0)) + (M(0) ^ M(3))) +
             dzeta(0) * (M(3) ^ M(3)));
    }  // end of LogarithmicStrainHandler<2u,StressType>::convertTangentModuli

    template <typename StressType>
    tfel::math::tvector<
        4u,
        tfel::math::stensor<
            2u,
            typename LogarithmicStrainHandler<2u, StressType>::real>>
    LogarithmicStrainHandler<2u, StressType>::getNTensors(
        const tfel::math::tmatrix<3u, 3u, real>& m) {
      using tvector = tfel::math::tvector<3u, real>;
      using stensor = tfel::math::stensor<2u, real>;
      const tvector v[2] = {m.template column_view<0u>(),
                            m.template column_view<1u>()};
      auto N = tfel::math::tvector<4u, stensor>();
      N(0) = stensor::buildFromVectorsSymmetricDiadicProduct(v[0], v[0]);
      N(1) = stensor::buildFromVectorsSymmetricDiadicProduct(v[1], v[1]);
      N(2) = {0, 0, 2, 0};
      N(3) = stensor::buildFromVectorsSymmetricDiadicProduct(v[0], v[1]);
      return N;
    }  // end of LogarithmicStrainHandler<2u,StressType>::getNTensors

    template <typename StressType>
    tfel::math::tvector<
        4u,
        tfel::math::stensor<
            2u,
            typename LogarithmicStrainHandler<2u, StressType>::real>>
    LogarithmicStrainHandler<2u, StressType>::getEulerianMTensors(
        const tfel::math::tmatrix<3u, 3u, real>& m,
        const DeformationGradient& F) {
      using tvector = tfel::math::tvector<3u, real>;
      using stensor = tfel::math::stensor<2u, real>;
      constexpr const auto zero = real{};
      const tvector v[2] = {m.template column_view<0u>(),
                            m.template column_view<1u>()};
      tfel::math::tvector<3u, real> n[2];
      n[0][0] = F[0] * v[0][0] + F[3] * v[0][1];
      n[0][1] = F[4] * v[0][0] + F[1] * v[0][1];
      n[0][2] = zero;
      n[1][0] = F[0] * v[1][0] + F[3] * v[1][1];
      n[1][1] = F[4] * v[1][0] + F[1] * v[1][1];
      n[1][2] = zero;
      auto M = tfel::math::tvector<4u, stensor>();
      M(0) = stensor::buildFromVectorsSymmetricDiadicProduct(n[0], n[0]);
      M(1) = stensor::buildFromVectorsSymmetricDiadicProduct(n[1], n[1]);
      M(2) = {zero, zero, 2 * F[2] * F[2], zero};
      M(3) = stensor::buildFromVectorsSymmetricDiadicProduct(n[0], n[1]);
      return M;
    }  // end of LogarithmicStrainHandler<2u,StressType>::getEulerianMTensors

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::TangentOperator
    LogarithmicStrainHandler<2u, StressType>::convertToMaterialTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      this->checkLagrangianSetting();
      // real work starts here
      const auto N = getNTensors(this->m);
      TangentOperator Kr;
      this->convertTangentModuli(Kr, Ks, T, N, N);
      return Kr;
    }

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::TangentOperator
    LogarithmicStrainHandler<2u, StressType>::convertToSpatialTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      if (this->s == LAGRANGIAN) {
        using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
        const auto F0 = tfel::math::tensor<2u, real>::Id();
        const auto sig = this->convertToCauchyStress(T);
        const auto Cse = this->convertToMaterialTangentModuli(Ks, T);
        return convert<FSTOBase::SPATIAL_MODULI, FSTOBase::DS_DEGL>(
            Cse, F0, this->F, sig);
      } else {
        const auto N = getNTensors(this->m);
        const auto M = getEulerianMTensors(this->m, this->F);
        TangentOperator Kr;
        this->convertTangentModuli(Kr, Ks, T, N, M);
        return Kr;
      }
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToSpatialTangentModuli

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::TangentOperator
    LogarithmicStrainHandler<2u, StressType>::
        convertToCauchyStressTruesdellRateTangentModuli(
            const TangentOperator& Ks, const StressStensor& T) const {
      if (this->s == LAGRANGIAN) {
        using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
        const auto F0 = tfel::math::tensor<2u, real>::Id();
        const auto sig = this->convertToCauchyStress(T);
        const auto Cse = this->convertToMaterialTangentModuli(Ks, T);
        auto Kr = convert<FSTOBase::SPATIAL_MODULI, FSTOBase::DS_DEGL>(
            Cse, F0, this->F, sig);
        Kr /= tfel::math::det(this->F);
        return Kr;
      }
      const auto N = getNTensors(this->m);
      const auto M = getEulerianMTensors(this->m, this->F);
      TangentOperator Kr;
      this->convertTangentModuli(Kr, Ks, T, N, M);
      Kr /= tfel::math::det(this->F);
      return Kr;
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToCauchyStressTruesdellRateTangentModuli

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::
        convertToCauchyStressTruesdellRateTangentModuli(
            stress* const K, const stress* const T) const {
      tfel::math::ST2toST2View<2u, stress> k(K);
      auto transpose = [&k]() {
        std::swap(k(0, 1), k(1, 0));
        std::swap(k(0, 2), k(2, 0));
        std::swap(k(0, 3), k(3, 0));
        std::swap(k(1, 2), k(2, 1));
        std::swap(k(1, 3), k(3, 1));
        std::swap(k(2, 3), k(3, 2));
      };
      auto to_tfel = [&k, transpose] {
        TFEL_CONSTEXPR const auto cste = tfel::math::Cste<real>::sqrt2;
        k(0, 3) *= cste;
        k(1, 3) *= cste;
        k(2, 3) *= cste;
        k(3, 0) *= cste;
        k(3, 1) *= cste;
        k(3, 2) *= cste;
        k(3, 3) *= 2;
        transpose();
      };
      auto to_abaqus = [&k, &transpose] {
        TFEL_CONSTEXPR const auto icste = tfel::math::Cste<real>::isqrt2;
        k(0, 3) *= icste;
        k(1, 3) *= icste;
        k(2, 3) *= icste;
        k(3, 0) *= icste;
        k(3, 1) *= icste;
        k(3, 2) *= icste;
        k(3, 3) /= 2;
        transpose();
      };
      to_tfel();
      tfel::math::stensor<2u, stress> t;
      t.importTab(T);
      k = this->convertToCauchyStressTruesdellRateTangentModuli(k, t);
      to_abaqus();
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToCauchyStressTruesdellRateTangentModuli

    template <typename StressType>
    typename LogarithmicStrainHandler<2u, StressType>::TangentOperator
    LogarithmicStrainHandler<2u, StressType>::convertToAbaqusTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
      const auto F0 = tfel::math::tensor<2u, real>::Id();
      const auto sig = this->convertToCauchyStress(T);
      if (this->s == LAGRANGIAN) {
        const auto Cse = this->convertToMaterialTangentModuli(Ks, T);
        auto Kr =
            convert<FSTOBase::ABAQUS, FSTOBase::DS_DEGL>(Cse, F0, this->F, sig);
        Kr /= tfel::math::det(this->F);
        return Kr;
      }
      const auto N = getNTensors(this->m);
      const auto M = getEulerianMTensors(this->m, this->F);
      TangentOperator Kr;
      this->convertTangentModuli(Kr, Ks, T, N, M);
      return convert<FSTOBase::ABAQUS, FSTOBase::SPATIAL_MODULI>(Kr, F0,
                                                                 this->F, sig);
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToAbaqusTangentModuli

    template <typename StressType>
    void LogarithmicStrainHandler<2u, StressType>::convertToAbaqusTangentModuli(
        stress* const K, const stress* const T) const {
      tfel::math::ST2toST2View<2u, stress> k(K);
      auto transpose = [&k]() {
        std::swap(k(0, 1), k(1, 0));
        std::swap(k(0, 2), k(2, 0));
        std::swap(k(0, 3), k(3, 0));
        std::swap(k(1, 2), k(2, 1));
        std::swap(k(1, 3), k(3, 1));
        std::swap(k(2, 3), k(3, 2));
      };
      auto to_tfel = [&k, transpose] {
        TFEL_CONSTEXPR const auto cste = tfel::math::Cste<real>::sqrt2;
        k(0, 3) *= cste;
        k(1, 3) *= cste;
        k(2, 3) *= cste;
        k(3, 0) *= cste;
        k(3, 1) *= cste;
        k(3, 2) *= cste;
        k(3, 3) *= 2;
        transpose();
      };
      auto to_abaqus = [&k, &transpose] {
        TFEL_CONSTEXPR const auto icste = tfel::math::Cste<real>::isqrt2;
        k(0, 3) *= icste;
        k(1, 3) *= icste;
        k(2, 3) *= icste;
        k(3, 0) *= icste;
        k(3, 1) *= icste;
        k(3, 2) *= icste;
        k(3, 3) /= 2;
        transpose();
      };
      to_tfel();
      tfel::math::stensor<2u, stress> t;
      t.importTab(T);
      k = this->convertToAbaqusTangentModuli(k, t);
      to_abaqus();
    }  // end of
       // LogarithmicStrainHandler<2u,StressType>::convertToAbaqusTangentModuli

    template <typename StressType>
    LogarithmicStrainHandler<3u, StressType>::LogarithmicStrainHandler(
        const Setting c, const DeformationGradient& F1)
        : LogarithmicStrainHandler(Builder(c, F1), c, F1) {
    }  // end of
       // LogarithmicStrainHandler<3u,StressTupe>::LogarithmicStrainHandler

    template <typename StressType>
    LogarithmicStrainHandler<3u, StressType>::Builder::Builder(
        const Setting c, const DeformationGradient& F1) {
      using namespace tfel::math;
      using stensor = stensor<3u, real>;
      const auto fl = [](const real x) { return std::log1p(x - 1) / 2; };
      const auto dfl = [](const real x) { return 1 / (2 * x); };
      const auto C = computeRightCauchyGreenTensor(F1);
      std::tie(this->vp, this->m) =
          C.template computeEigenVectors<stensor::FSESJACOBIEIGENSOLVER>();
      this->e = map(fl, this->vp);
      const auto d = map(dfl, this->vp);
      // p is one half of the tensor defined by Miehe
      if (c == LAGRANGIAN) {
        this->p = stensor::computeIsotropicFunctionDerivative(
            this->e, d, this->vp, this->m, eps);
      } else {
        const auto theta = [this, &d] {
          constexpr const auto zero = real{0};
          if (areEigenValuesEqual(vp)) {
            const auto rv = (d[0] + d[1] + d[2]) / 3;
            return tmatrix<3u, 3u, real>{zero, rv, rv, rv,  zero,
                                         rv,   rv, rv, zero};
          }
          auto r = tmatrix<3u, 3u, real>{};
          const auto k = findSingleEigenValue(vp);
          if (k != 3) {
            for (size_type i = 0; i != 3; ++i) {
              for (size_type j = 0; j != 3; ++j) {
                if (i == j) {
                  r(i, j) = zero;
                } else if ((i == k) || (j == k)) {
                  r(i, j) =
                      (this->e[i] - this->e[j]) / (this->vp[i] - this->vp[j]);
                } else {
                  r(i, j) = (d[i] + d[j]) / 2;
                }
              }
            }
            return r;
          }
          for (size_type i = 0; i != 3; ++i) {
            for (size_type j = 0; j != 3; ++j) {
              if (i == j) {
                r(i, j) = real{};
              } else {
                r(i, j) =
                    (this->e[i] - this->e[j]) / (this->vp[i] - this->vp[j]);
              }
            }
          }
          return r;
        }();
        const auto N = getNTensors(m);
        const auto M = getEulerianMTensors(m, F1);
        this->p = (d[0] * (N(0, 0) ^ M(0, 0)) + d[1] * (N(1, 1) ^ M(1, 1)) +
                   d[2] * (N(2, 2) ^ M(2, 2))) /
                  4;
        for (size_type i = 0; i != 3; ++i) {
          for (size_type j = 0; j != 3; ++j) {
            if (i == j) {
              continue;
            }
            this->p += (theta(i, j) / 4) * (N(i, j) ^ M(i, j));
          }
        }
      }
    }

    template <typename StressType>
    LogarithmicStrainHandler<3u, StressType>::LogarithmicStrainHandler(
        Builder&& src, const Setting c, const DeformationGradient& F1)
        : LogarithmicStrainHandlerBase(c),
          p(src.p),
          F(F1),
          m(src.m),
          vp(src.vp),
          e(src.e) {
    }  // end of
       // LogarithmicStrainHandler<3u,StressTupe>::LogarithmicStrainHandler

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::StrainStensor
    LogarithmicStrainHandler<3u, StressType>::getHenckyLogarithmicStrain()
        const {
      return StrainStensor::computeIsotropicFunction(this->e, this->m);
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::getHenckyLogarithmicStrain

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::getHenckyLogarithmicStrain(
        real* const elog) const {
      const auto el = StrainStensor::computeIsotropicFunction(this->e, this->m);
      tfel::math::internals::ExportToVoigt<3u>::exe(&el[0], elog);
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::getHenckyLogarithmicStrain

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::StressStensor
    LogarithmicStrainHandler<3u, StressType>::
        convertFromSecondPiolaKirchhoffStress(const StressStensor& S) const {
      this->checkLagrangianSetting();
      return (S | tfel::math::invert(p)) / 2;
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertFromSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::
        convertFromSecondPiolaKirchhoffStress(stress* const ST) const {
      this->checkLagrangianSetting();
      tfel::math::stensor<3u, stress> S;
      S.importTab(ST);
      const auto T = this->convertFromSecondPiolaKirchhoffStress(S);
      T.exportTab(ST);
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertFromSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::StressStensor
    LogarithmicStrainHandler<3u, StressType>::
        convertToSecondPiolaKirchhoffStress(const StressStensor& T) const {
      this->checkLagrangianSetting();
      return 2 * (T | p);
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::
        convertToSecondPiolaKirchhoffStress(stress* const TS) const {
      this->checkLagrangianSetting();
      tfel::math::stensor<3u, stress> T;
      T.importTab(TS);
      const auto S = this->convertToSecondPiolaKirchhoffStress(T);
      S.exportTab(TS);
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::StressStensor
    LogarithmicStrainHandler<3u, StressType>::convertToCauchyStress(
        const StressStensor& T) const {
      if (this->s == LAGRANGIAN) {
        const auto S = this->convertToSecondPiolaKirchhoffStress(T);
        return tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(
            S, this->F);
      } else {
        return (2 * (T | p)) / tfel::math::det(this->F);
      }
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::convertToCauchyStress(
        stress* const Ts) const {
      StressStensor T;
      T.importTab(Ts);
      this->convertToCauchyStress(T).exportTab(Ts);
    }  // end of LogarithmicStrainHandler<3u,StressType>::convertToCauchyStress

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::StressStensor
    LogarithmicStrainHandler<3u, StressType>::convertFromCauchyStress(
        const StressStensor& sig) const {
      if (this->s == LAGRANGIAN) {
        const auto S =
            tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(
                sig, this->F);
        return this->convertFromSecondPiolaKirchhoffStress(S);
      } else {
        return (sig | tfel::math::invert(p)) * tfel::math::det(this->F) / 2;
      }
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertFromCauchyStress

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::convertFromCauchyStress(
        stress* const sT) const {
      StressStensor sig;
      sig.importTab(sT);
      this->convertFromCauchyStress(sig).exportTab(sT);
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertFromCauchyStress

    template <typename StressType>
    tfel::math::tmatrix<
        3u,
        3u,
        tfel::math::stensor<
            3u,
            typename LogarithmicStrainHandler<3u, StressType>::real>>
    LogarithmicStrainHandler<3u, StressType>::getNTensors(
        const tfel::math::tmatrix<3u, 3u, real>& m) {
      using namespace tfel::math;
      using tvector = tfel::math::tvector<3u, real>;
      using stensor = tfel::math::stensor<3u, real>;
      const tvector v[3] = {m.template column_view<0u>(),
                            m.template column_view<1u>(),
                            m.template column_view<2u>()};
      auto N = tmatrix<3u, 3u, stensor>{};
      for (size_type i = 0; i != 3; ++i) {
        for (size_type j = 0; j != 3; ++j) {
          N(i, j) = stensor::buildFromVectorsSymmetricDiadicProduct(v[i], v[j]);
        }
      }
      return N;
    }  // end of LogarithmicStrainHandler<3u,StressType>::getNTensors

    template <typename StressType>
    tfel::math::tmatrix<
        3u,
        3u,
        tfel::math::stensor<
            3u,
            typename LogarithmicStrainHandler<3u, StressType>::real>>
    LogarithmicStrainHandler<3u, StressType>::getEulerianMTensors(
        const tfel::math::tmatrix<3u, 3u, real>& m,
        const DeformationGradient& F) {
      using namespace tfel::math;
      using tvector = tfel::math::tvector<3u, real>;
      using stensor = tfel::math::stensor<3u, real>;
      auto apply = [&F](const tvector& v2) -> tvector {
        return {F[0] * v2[0] + F[3] * v2[1] + F[5] * v2[2],
                F[4] * v2[0] + F[1] * v2[1] + F[7] * v2[2],
                F[6] * v2[0] + F[8] * v2[1] + F[2] * v2[2]};
      };
      const tvector v[3] = {m.template column_view<0u>(),
                            m.template column_view<1u>(),
                            m.template column_view<2u>()};
      const tvector n[3] = {apply(v[0]), apply(v[1]), apply(v[2])};
      auto M = tmatrix<3u, 3u, stensor>{};
      for (size_type i = 0; i != 3; ++i) {
        for (size_type j = 0; j != 3; ++j) {
          M(i, j) = stensor::buildFromVectorsSymmetricDiadicProduct(n[i], n[j]);
        }
      }
      return M;
    }  // end of LogarithmicStrainHandler<3u,StressType>::getEulerianMTensors

    template <typename StressType>
    bool LogarithmicStrainHandler<3u, StressType>::areEigenValuesEqual(
        const tfel::math::tvector<3u, real>& vp) {
      return ((std::abs(vp(1) - vp(0)) < eps) &&
              (std::abs(vp(1) - vp(2)) < eps) &&
              (std::abs(vp(2) - vp(0)) < eps));
    }  // end of LogarithmicStrainHandler<3u,StressType>::areEigenValuesEqual

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::size_type
    LogarithmicStrainHandler<3u, StressType>::findSingleEigenValue(
        const tfel::math::tvector<3u, real>& vp) {
      if (areEigenValuesEqual(vp)) {
        throw(
            std::runtime_error("LogarithmicStrainHandler<3u,StressType>::"
                               "findSingleEigenValue: "
                               "this method shall not be called if all "
                               "eigen values are equal."));
      }
      if ((std::abs(vp(1) - vp(0)) > eps) && (std::abs(vp(1) - vp(2)) > eps) &&
          (std::abs(vp(2) - vp(0)) > eps)) {
        // all eigenvalues are different
        return 3;
      }
      if ((std::abs(vp(1) - vp(0)) < eps)) {
        return 2;
      }
      if ((std::abs(vp(2) - vp(0)) < eps)) {
        return 1;
      }
      return 0;
    }

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::convertTangentModuli(
        TangentOperator& Kr,
        const TangentOperator& Ks,
        const StressStensor& T,
        const tfel::math::tmatrix<3u, 3u, tfel::math::stensor<3u, real>>& N,
        const tfel::math::tmatrix<3u, 3u, tfel::math::stensor<3u, real>>& M)
        const {
      using tfel::math::tmatrix;
      const auto lk = [](const size_type i, const size_type j) -> size_type {
        if (i == 0) {
          return (j == 1) ? 2 : 1;
        }
        if (i == 1) {
          return (j == 0) ? 2 : 0;
        }
        return (j == 0) ? 1 : 0;
      };
      const auto dfl = [](const real x) { return 1 / (2 * x); };
      const auto d = map(dfl, this->vp);  // half compared to Miehe definition
      const auto f = map([](const real x) { return -2 / (x * x); }, this->vp);
      const auto xsi = [this, &d, &f]() -> tmatrix<3u, 3u, real> {
        if (areEigenValuesEqual(this->vp)) {
          constexpr const auto zero = real{0};
          const auto rv = (f[0] + f[1] + f[2]) / 24;
          return {zero, rv, rv, rv, zero, rv, rv, rv, zero};
        }
        auto r = tmatrix<3u, 3u, real>{};
        const auto k = findSingleEigenValue(this->vp);
        if (k != 3) {
          for (size_type i = 0; i != 3; ++i) {
            for (size_type j = 0; j != 3; ++j) {
              if (i == j) {
                r(i, j) = real{};
              } else if ((i == k) || (j == k)) {
                const auto idvp = 1 / (this->vp[i] - this->vp[j]);
                r(i, j) = ((this->e[i] - this->e[j]) * idvp - d[j]) * idvp;
              } else {
                r(i, j) = (f[i] + f[j]) / 16;
              }
            }
          }
          return r;
        }
        for (size_type i = 0; i != 3; ++i) {
          for (size_type j = 0; j != 3; ++j) {
            if (i == j) {
              r(i, j) = real{};
            } else {
              const auto idvp = 1 / (this->vp[i] - this->vp[j]);
              r(i, j) = ((this->e[i] - this->e[j]) * idvp - d[j]) * idvp;
            }
          }
        }
        return r;
      }();
      const auto eta = [this, &lk, &f, &d] {
        if (areEigenValuesEqual(this->vp)) {
          return (f[0] + f[1] + f[2]) / 24;
        }
        const auto u = findSingleEigenValue(this->vp);
        if (u != 3) {
          const auto i = (u == 2) ? 0 : 2;
          const auto idvp = 1 / (this->vp[i] - this->vp[u]);
          return ((this->e[i] - this->e[u]) * idvp - d[u]) * idvp;
        }
        auto r = real{};
        for (size_type i = 0; i != 3; ++i) {
          for (size_type j = 0; j != 3; ++j) {
            if (i == j) {
              continue;
            }
            const auto k = lk(i, j);
            r += this->e[i] / (2 * (this->vp[i] - this->vp[j]) *
                               (this->vp[i] - this->vp[k]));
          }
        }
        return r;
      }();
      const auto dzeta = [&N, &T] {
        auto r = tmatrix<3u, 3u, real>{};
        for (size_type i = 0; i != 3; ++i) {
          for (size_type j = 0; j != 3; ++j) {
            r(i, j) = (T | N(i, j)) / 2;
          }
        }
        return r;
      }();
#if (!defined __INTEL_COMPILER) && (!defined __PGI)
      Kr = 4 * transpose(p) * Ks * p +
           (f[0] * dzeta(0, 0) * (M(0, 0) ^ M(0, 0)) +
            f[1] * dzeta(1, 1) * (M(1, 1) ^ M(1, 1)) +
            f[2] * dzeta(2, 2) * (M(2, 2) ^ M(2, 2))) /
               4;
#else
      const tfel::math::st2tost2<3u, real> tp = transpose(p);
      Kr = 4 * tp * Ks * p + (f[0] * dzeta(0, 0) * (M(0, 0) ^ M(0, 0)) +
                              f[1] * dzeta(1, 1) * (M(1, 1) ^ M(1, 1)) +
                              f[2] * dzeta(2, 2) * (M(2, 2) ^ M(2, 2))) /
                                 4;
#endif
      for (size_type i = 0; i != 3; ++i) {
        for (size_type j = 0; j != 3; ++j) {
          if (i == j) {
            continue;
          }
          const auto k = lk(i, j);
          Kr += 2 * eta * dzeta(i, j) * (M(i, k) ^ M(j, k)) +
                2 * xsi(i, j) *
                    (dzeta(i, j) * ((M(i, j) ^ M(j, j)) + (M(j, j) ^ M(i, j))) +
                     dzeta(j, j) * (M(i, j) ^ M(i, j)));
        }
      }
    }

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::TangentOperator
    LogarithmicStrainHandler<3u, StressType>::convertToMaterialTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      this->checkLagrangianSetting();
      // real work starts here
      const auto N = getNTensors(this->m);
      TangentOperator Kr;
      this->convertTangentModuli(Kr, Ks, T, N, N);
      return Kr;
    }

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::TangentOperator
    LogarithmicStrainHandler<3u, StressType>::convertToSpatialTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      if (this->s == LAGRANGIAN) {
        using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
        const auto F0 = tfel::math::tensor<3u, real>::Id();
        const auto sig = this->convertToCauchyStress(T);
        const auto Cse = this->convertToMaterialTangentModuli(Ks, T);
        return convert<FSTOBase::SPATIAL_MODULI, FSTOBase::DS_DEGL>(
            Cse, F0, this->F, sig);
      } else {
        const auto N = getNTensors(this->m);
        const auto M = getEulerianMTensors(this->m, this->F);
        TangentOperator Kr;
        this->convertTangentModuli(Kr, Ks, T, N, M);
        return Kr;
      }
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToSpatialTangentModuli

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::TangentOperator
    LogarithmicStrainHandler<3u, StressType>::
        convertToCauchyStressTruesdellRateTangentModuli(
            const TangentOperator& Ks, const StressStensor& T) const {
      if (this->s == LAGRANGIAN) {
        using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
        const auto F0 = tfel::math::tensor<3u, real>::Id();
        const auto sig = this->convertToCauchyStress(T);
        const auto Cse = this->convertToMaterialTangentModuli(Ks, T);
        auto Kr = convert<FSTOBase::SPATIAL_MODULI, FSTOBase::DS_DEGL>(
            Cse, F0, this->F, sig);
        Kr /= tfel::math::det(this->F);
        return Kr;
      } else {
        const auto N = getNTensors(this->m);
        const auto M = getEulerianMTensors(this->m, this->F);
        TangentOperator Kr;
        this->convertTangentModuli(Kr, Ks, T, N, M);
        Kr /= tfel::math::det(this->F);
        return Kr;
      }
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToCauchyStressTruesdellRateTangentModuli

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::
        convertToCauchyStressTruesdellRateTangentModuli(
            stress* const K, const stress* const T) const {
      tfel::math::ST2toST2View<3u, stress> k(K);
      auto transpose = [&k]() {
        std::swap(k(0, 1), k(1, 0));
        std::swap(k(0, 2), k(2, 0));
        std::swap(k(0, 3), k(3, 0));
        std::swap(k(0, 4), k(4, 0));
        std::swap(k(0, 5), k(5, 0));
        std::swap(k(1, 2), k(2, 1));
        std::swap(k(1, 3), k(3, 1));
        std::swap(k(1, 4), k(4, 1));
        std::swap(k(1, 5), k(5, 1));
        std::swap(k(2, 3), k(3, 2));
        std::swap(k(2, 4), k(4, 2));
        std::swap(k(2, 5), k(5, 2));
        std::swap(k(3, 4), k(4, 3));
        std::swap(k(3, 5), k(5, 3));
        std::swap(k(4, 5), k(5, 4));
      };
      auto to_tfel = [&k, &transpose] {
        TFEL_CONSTEXPR const auto cste = tfel::math::Cste<real>::sqrt2;
        k(0, 3) *= cste;
        k(1, 3) *= cste;
        k(2, 3) *= cste;
        k(0, 4) *= cste;
        k(1, 4) *= cste;
        k(2, 4) *= cste;
        k(0, 5) *= cste;
        k(1, 5) *= cste;
        k(2, 5) *= cste;
        k(3, 0) *= cste;
        k(3, 1) *= cste;
        k(3, 2) *= cste;
        k(4, 0) *= cste;
        k(4, 1) *= cste;
        k(4, 2) *= cste;
        k(5, 0) *= cste;
        k(5, 1) *= cste;
        k(5, 2) *= cste;
        k(3, 3) *= 2;
        k(3, 4) *= 2;
        k(3, 5) *= 2;
        k(4, 3) *= 2;
        k(4, 4) *= 2;
        k(4, 5) *= 2;
        k(5, 3) *= 2;
        k(5, 4) *= 2;
        k(5, 5) *= 2;
        transpose();
      };
      auto to_abaqus = [&k, &transpose] {
        TFEL_CONSTEXPR const auto icste = tfel::math::Cste<real>::isqrt2;
        k(0, 3) *= icste;
        k(1, 3) *= icste;
        k(2, 3) *= icste;
        k(0, 4) *= icste;
        k(1, 4) *= icste;
        k(2, 4) *= icste;
        k(0, 5) *= icste;
        k(1, 5) *= icste;
        k(2, 5) *= icste;
        k(3, 0) *= icste;
        k(3, 1) *= icste;
        k(3, 2) *= icste;
        k(4, 0) *= icste;
        k(4, 1) *= icste;
        k(4, 2) *= icste;
        k(5, 0) *= icste;
        k(5, 1) *= icste;
        k(5, 2) *= icste;
        k(3, 3) /= 2;
        k(3, 4) /= 2;
        k(3, 5) /= 2;
        k(4, 3) /= 2;
        k(4, 4) /= 2;
        k(4, 5) /= 2;
        k(5, 3) /= 2;
        k(5, 4) /= 2;
        k(5, 5) /= 2;
        transpose();
      };
      to_tfel();
      tfel::math::stensor<3u, stress> t;
      t.importTab(T);
      k = this->convertToCauchyStressTruesdellRateTangentModuli(k, t);
      to_abaqus();
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToCauchyStressTruesdellRateTangentModuli

    template <typename StressType>
    typename LogarithmicStrainHandler<3u, StressType>::TangentOperator
    LogarithmicStrainHandler<3u, StressType>::convertToAbaqusTangentModuli(
        const TangentOperator& Ks, const StressStensor& T) const {
      using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
      const auto F0 = tfel::math::tensor<3u, real>::Id();
      const auto sig = this->convertToCauchyStress(T);
      if (this->s == LAGRANGIAN) {
        const auto Cse = this->convertToMaterialTangentModuli(Ks, T);
        auto Kr =
            convert<FSTOBase::ABAQUS, FSTOBase::DS_DEGL>(Cse, F0, this->F, sig);
        Kr /= tfel::math::det(this->F);
        return Kr;
      } else {
        const auto N = getNTensors(this->m);
        const auto M = getEulerianMTensors(this->m, this->F);
        TangentOperator Kr;
        this->convertTangentModuli(Kr, Ks, T, N, M);
        return convert<FSTOBase::ABAQUS, FSTOBase::SPATIAL_MODULI>(
            Kr, F0, this->F, sig);
      }
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToAbaqusTangentModuli

    template <typename StressType>
    void LogarithmicStrainHandler<3u, StressType>::convertToAbaqusTangentModuli(
        stress* const K, const stress* const T) const {
      tfel::math::ST2toST2View<3u, stress> k(K);
      auto transpose = [&k]() {
        std::swap(k(0, 1), k(1, 0));
        std::swap(k(0, 2), k(2, 0));
        std::swap(k(0, 3), k(3, 0));
        std::swap(k(0, 4), k(4, 0));
        std::swap(k(0, 5), k(5, 0));
        std::swap(k(1, 2), k(2, 1));
        std::swap(k(1, 3), k(3, 1));
        std::swap(k(1, 4), k(4, 1));
        std::swap(k(1, 5), k(5, 1));
        std::swap(k(2, 3), k(3, 2));
        std::swap(k(2, 4), k(4, 2));
        std::swap(k(2, 5), k(5, 2));
        std::swap(k(3, 4), k(4, 3));
        std::swap(k(3, 5), k(5, 3));
        std::swap(k(4, 5), k(5, 4));
      };
      auto to_tfel = [&k, &transpose] {
        TFEL_CONSTEXPR const auto cste = tfel::math::Cste<real>::sqrt2;
        k(0, 3) *= cste;
        k(1, 3) *= cste;
        k(2, 3) *= cste;
        k(0, 4) *= cste;
        k(1, 4) *= cste;
        k(2, 4) *= cste;
        k(0, 5) *= cste;
        k(1, 5) *= cste;
        k(2, 5) *= cste;
        k(3, 0) *= cste;
        k(3, 1) *= cste;
        k(3, 2) *= cste;
        k(4, 0) *= cste;
        k(4, 1) *= cste;
        k(4, 2) *= cste;
        k(5, 0) *= cste;
        k(5, 1) *= cste;
        k(5, 2) *= cste;
        k(3, 3) *= 2;
        k(3, 4) *= 2;
        k(3, 5) *= 2;
        k(4, 3) *= 2;
        k(4, 4) *= 2;
        k(4, 5) *= 2;
        k(5, 3) *= 2;
        k(5, 4) *= 2;
        k(5, 5) *= 2;
        transpose();
      };
      auto to_abaqus = [&k, &transpose] {
        TFEL_CONSTEXPR const auto icste = tfel::math::Cste<real>::isqrt2;
        k(0, 3) *= icste;
        k(1, 3) *= icste;
        k(2, 3) *= icste;
        k(0, 4) *= icste;
        k(1, 4) *= icste;
        k(2, 4) *= icste;
        k(0, 5) *= icste;
        k(1, 5) *= icste;
        k(2, 5) *= icste;
        k(3, 0) *= icste;
        k(3, 1) *= icste;
        k(3, 2) *= icste;
        k(4, 0) *= icste;
        k(4, 1) *= icste;
        k(4, 2) *= icste;
        k(5, 0) *= icste;
        k(5, 1) *= icste;
        k(5, 2) *= icste;
        k(3, 3) /= 2;
        k(3, 4) /= 2;
        k(3, 5) /= 2;
        k(4, 3) /= 2;
        k(4, 4) /= 2;
        k(4, 5) /= 2;
        k(5, 3) /= 2;
        k(5, 4) /= 2;
        k(5, 5) /= 2;
        transpose();
      };
      to_tfel();
      tfel::math::stensor<3u, stress> t;
      t.importTab(T);
      k = this->convertToAbaqusTangentModuli(k, t);
      to_abaqus();
    }  // end of
       // LogarithmicStrainHandler<3u,StressType>::convertToAbaqusTangentModuli

  }  // end of namespace material

}  // namespace tfel

#endif /* LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_IXX */
