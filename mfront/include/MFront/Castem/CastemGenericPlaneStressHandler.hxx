/*!
 * \file  mfront/include/MFront/Castem/CastemGenericPlaneStressHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMGENERICPLANESTRESSHANDLER_H_
#define LIB_MFRONT_CASTEM_CASTEMGENERICPLANESTRESSHANDLER_H_

#include <cmath>
#include <algorithm>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/tvector.hxx"

#ifndef LIB_MFRONT_CASTEM_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CASTEM_CALL_HXX */

namespace castem {

  /*!
   * \brief Generic handler for the plane stress hypothesis
   *
   * This allows behaviours written for the generalised plane stress
   * hypothesis to be used in plane stress computations.
   */
  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CastemGenericPlaneStressHandler : public CastemInterfaceExceptions {
    /*!
     * Main entry point This mainly choose between a specific
     * handler. The choice is made from the nature of elastic
     * behaviour (isotropic or not).
     */
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      constexpr const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::PLANESTRESS;
      //! a simple alias
      using BV = Behaviour<H, CastemReal, false>;
      using MTraits = MechanicalBehaviourTraits<BV>;
      using Traits = CastemTraits<BV>;
      typedef
          typename std::conditional<Traits::stype == castem::ISOTROPIC,
                                    TreatPlaneStressIsotropicBehaviour,
                                    TreatPlaneStressOrthotropicBehaviour>::type
              Handler;
      if (std::abs(*DDSDDE) > 0) {
        throwTangentOperatorNotAvailableThroughGenericPlaneStressHandler(
            MTraits::getName());
      }
      Handler::exe(DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
                   NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op,
                   sfeh);
    }  // end of exe

   private:
    TFEL_CASTEM_INLINE2 static void checkNSTATV(const CastemInt NSTATV) {
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef Behaviour<H, CastemReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short NSTATV_ = Traits::internal_variables_nb + 1u;
      const bool is_defined_ = Traits::is_defined;
      // Test if the nb of state variables matches Behaviour requirements
      if ((NSTATV_ != NSTATV) && is_defined_) {
        throwUnMatchedNumberOfStateVariables(Traits::getName(), NSTATV_,
                                             NSTATV);
      }
    }  // end of checkNSTATV

    template <typename GeneralisedPlaneStrainBehaviour>
    TFEL_CASTEM_INLINE2 static void exe(
        const CastemReal c1,
        const CastemReal c2,
        const CastemReal c3,
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        CastemReal *const STATEV,
        const CastemInt *const NSTATV,
        CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::GENERALISEDPLANESTRAIN;
      using tfel::fsalgo::copy;
      typedef Behaviour<H, CastemReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short NSTATV_ = Traits::internal_variables_nb + 1u;
      typedef typename std::conditional<
          (NSTATV_ < 20), tfel::math::tvector<NSTATV_, CastemReal>,
          tfel::math::vector<CastemReal>>::type SVector;
      CastemGenericPlaneStressHandler::checkNSTATV(*NSTATV);
      unsigned int iter;
      const unsigned int iterMax = 50;

      CastemReal eto[4];
      CastemReal deto[4];
      CastemReal s[4];
      SVector v;
      CastemGenericPlaneStressHandler::resize(v, NSTATV_);
      CastemReal dez;
      CastemReal x[2];
      CastemReal f[2];

      dez = c1 * DSTRAN[0] + c2 * DSTRAN[1];
      CastemGenericPlaneStressHandler::template iter<
          GeneralisedPlaneStrainBehaviour>(DTIME, DROT, DDSDDE, TEMP, DTEMP,
                                           PROPS, NPROPS, PREDEF, DPRED, STATEV,
                                           STRESS, PNEWDT, STRAN, DSTRAN, dez,
                                           &v[0], s, eto, deto, op, sfeh);
      x[1] = dez;
      f[1] = s[2];

      if (abs(c3 * s[2]) > 1.e-12) {
        dez -= c3 * s[2];
        CastemGenericPlaneStressHandler::template iter<
            GeneralisedPlaneStrainBehaviour>(
            DTIME, DROT, DDSDDE, TEMP, DTEMP, PROPS, NPROPS, PREDEF, DPRED,
            STATEV, STRESS, PNEWDT, STRAN, DSTRAN, dez, &v[0], s, eto, deto, op,
            sfeh);
      }

      iter = 2;
      while ((abs(c3 * s[2]) > 1.e-12) && (iter < iterMax)) {
        x[0] = x[1];
        f[0] = f[1];
        x[1] = dez;
        f[1] = s[2];
        dez -= (x[1] - x[0]) / (f[1] - f[0]) * s[2];
        CastemGenericPlaneStressHandler::template iter<
            GeneralisedPlaneStrainBehaviour>(
            DTIME, DROT, DDSDDE, TEMP, DTEMP, PROPS, NPROPS, PREDEF, DPRED,
            STATEV, STRESS, PNEWDT, STRAN, DSTRAN, dez, &v[0], s, eto, deto, op,
            sfeh);
        ++iter;
      }
      if (iter == iterMax) {
        throwPlaneStressMaximumNumberOfIterationsReachedException(
            Traits::getName());
      }
      copy<4>::exe(s, STRESS);
      STRESS[2] = 0;
      std::copy(v.begin(), v.end(), STATEV);
      STATEV[*NSTATV - 1] += dez;
    }  // end of exe

    template <unsigned short N, typename T>
    static void resize(tfel::math::tvector<N, T> &, const unsigned short) {}

    template <typename T>
    static void resize(tfel::math::vector<T> &v, const unsigned short n) {
      v.resize(n);
    }

    template <typename GeneralisedPlaneStrainBehaviour>
    TFEL_CASTEM_INLINE2 static void iter(
        const CastemReal *const DTIME,
        const CastemReal *const DROT,
        CastemReal *const DDSDDE,
        const CastemReal *const TEMP,
        const CastemReal *const DTEMP,
        const CastemReal *const PROPS,
        const CastemInt *const NPROPS,
        const CastemReal *const PREDEF,
        const CastemReal *const DPRED,
        const CastemReal *const STATEV,
        const CastemReal *const STRESS,
        CastemReal *const PNEWDT,
        const CastemReal *const STRAN,
        const CastemReal *const DSTRAN,
        const CastemReal dez,
        CastemReal *const v,
        CastemReal *const s,
        CastemReal *const eto,
        CastemReal *const deto,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::material;
      using tfel::fsalgo::copy;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef Behaviour<H, CastemReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const CastemInt nNSTATV = Traits::internal_variables_nb == 0
                                    ? 1
                                    : Traits::internal_variables_nb;
      const unsigned short NSTATV_ = Traits::internal_variables_nb + 1u;
      copy<4>::exe(STRESS, s);
      copy<4>::exe(STRAN, eto);
      copy<4>::exe(DSTRAN, deto);
      eto[2] = STATEV[NSTATV_ - 1];
      deto[2] = dez;
      std::copy(STATEV, STATEV + NSTATV_, v);
      GeneralisedPlaneStrainBehaviour::exe(DTIME, DROT, DDSDDE, eto, deto, TEMP,
                                           DTEMP, PROPS, NPROPS, PREDEF, DPRED,
                                           v, &nNSTATV, s, PNEWDT, op, sfeh);
    }

    struct TreatPlaneStressIsotropicBehaviour {
      TFEL_CASTEM_INLINE2 static void exe(
          const CastemReal *const DTIME,
          const CastemReal *const DROT,
          CastemReal *const DDSDDE,
          const CastemReal *const STRAN,
          const CastemReal *const DSTRAN,
          const CastemReal *const TEMP,
          const CastemReal *const DTEMP,
          const CastemReal *const PROPS,
          const CastemInt *const NPROPS,
          const CastemReal *const PREDEF,
          const CastemReal *const DPRED,
          CastemReal *const STATEV,
          const CastemInt *const NSTATV,
          CastemReal *const STRESS,
          CastemReal *const PNEWDT,
          const tfel::material::OutOfBoundsPolicy op,
          const StressFreeExpansionHandler &sfeh) {
        using namespace tfel::material;
        using BehaviourHandler = CastemIsotropicBehaviourHandler<
            SMALLSTRAINSTANDARDBEHAVIOUR,
            ModellingHypothesis::GENERALISEDPLANESTRAIN, Behaviour>;
        using BV = Behaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN,
                             CastemReal, false>;
        constexpr const auto offset = CastemTraits<BV>::propertiesOffset;
        constexpr const auto nprops =
            MechanicalBehaviourTraits<BV>::material_properties_nb;
        static_assert(offset == 4u, "invalid offset value");
        CastemReal nPROPS[offset + nprops];
        nPROPS[0] = PROPS[0];
        nPROPS[1] = PROPS[1];
        nPROPS[2] = PROPS[2];
        nPROPS[3] = PROPS[3];
        // skipping the plate width
        tfel::fsalgo::copy<nprops>::exe(PROPS + 5, nPROPS + 4);
        const CastemReal y = PROPS[0];  // Young Modulus
        const CastemReal n = PROPS[1];  // Poisson ratio
        const CastemReal c1 = -n / (1 - n);
        const CastemReal c3 = 1 / y;
        const CastemInt nb = *NPROPS - 1;
        CastemGenericPlaneStressHandler::template exe<BehaviourHandler>(
            c1, c1, c3, DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP, nPROPS,
            &nb, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op, sfeh);
      }  // end of exe
    };   // end of struct TreatPlanStressIsotropicBehaviour

    struct TreatPlaneStressOrthotropicBehaviour {
      TFEL_CASTEM_INLINE2 static void exe(
          const CastemReal *const DTIME,
          const CastemReal *const DROT,
          CastemReal *const DDSDDE,
          const CastemReal *const STRAN,
          const CastemReal *const DSTRAN,
          const CastemReal *const TEMP,
          const CastemReal *const DTEMP,
          const CastemReal *const PROPS,
          const CastemInt *const NPROPS,
          const CastemReal *const PREDEF,
          const CastemReal *const DPRED,
          CastemReal *const STATEV,
          const CastemInt *const NSTATV,
          CastemReal *const STRESS,
          CastemReal *const PNEWDT,
          const tfel::material::OutOfBoundsPolicy op,
          const StressFreeExpansionHandler &sfeh) {
        using namespace tfel::meta;
        using namespace tfel::material;
        constexpr const auto H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
        typedef Behaviour<H, CastemReal, false> BV;
        typedef CastemOrthotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
                                                  H, Behaviour>
            BehaviourHandler;
        constexpr const auto offset = CastemTraits<BV>::propertiesOffset;
        constexpr const auto nprops =
            MechanicalBehaviourTraits<BV>::material_properties_nb;
        static_assert(offset == 13u, "invalid offset value");
        CastemReal nPROPS[offset + nprops];
        nPROPS[0] = PROPS[0];
        nPROPS[1] = PROPS[1];
        nPROPS[2] = PROPS[6];
        nPROPS[3] = PROPS[2];
        nPROPS[4] = PROPS[7];
        nPROPS[5] = PROPS[8];
        nPROPS[6] = PROPS[3];
        nPROPS[7] = PROPS[4];
        nPROPS[8] = PROPS[5];
        nPROPS[9] = PROPS[9];
        nPROPS[10] = PROPS[10];
        nPROPS[11] = PROPS[11];
        // thermal expansion in the third direction
        nPROPS[12] = CastemReal(0);
        // skipping the plate width
        tfel::fsalgo::copy<nprops>::exe(PROPS + 13, nPROPS + 13);
        // S11 = 1/E1
        const CastemReal S11 = 1 / nPROPS[0];
        // S22 = 1/E2
        const CastemReal S22 = 1 / nPROPS[1];
        // S22 = 1/E3
        const CastemReal S33 = 1 / nPROPS[2];
        // S12 = -n12/E1
        const CastemReal S12 = -nPROPS[3] / nPROPS[0];
        // S13 = -n13/E1
        const CastemReal S13 = -nPROPS[5] / nPROPS[0];
        // S23 = -n23/E2
        const CastemReal S23 = -nPROPS[4] / nPROPS[1];
        const CastemReal detS = S11 * S22 * S33 + 2 * S23 * S13 * S12 -
                                S11 * S23 * S23 - S22 * S13 * S13 -
                                S33 * S12 * S12;
        const CastemReal C22 = (S11 * S22 - S12 * S12) / detS;
        const CastemReal C20 = (S12 * S23 - S13 * S22) / detS;
        const CastemReal C21 = (S12 * S13 - S11 * S23) / detS;
        // Plane stress condition leads to :
        // Dezz = - (1/C22)*(C20*Dexx+C21*Deyy)
        const CastemReal c1 = -C20 / C22;
        const CastemReal c2 = -C21 / C22;
        // calling the resolution
        CastemGenericPlaneStressHandler::template exe<BehaviourHandler>(
            c1, c2, S22, DTIME, DROT, DDSDDE, STRAN, DSTRAN, TEMP, DTEMP,
            nPROPS, NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, PNEWDT, op,
            sfeh);
      }  // end of exe
    };   // end of struct TreatPlanStressOrthotropicBehaviour

  };  // end of struct CastemGenericPlaneStressHandlerTreatPlaneStrain

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMGENERICPLANESTRESSHANDLER_H_ */
