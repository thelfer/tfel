/*!
 * \file  mfront/include/MFront/Cyrano/CyranoGenericPlaneStressHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANO_CYRANOGENERICPLANESTRESSHANDLER_HXX
#define LIB_MFRONT_CYRANO_CYRANOGENERICPLANESTRESSHANDLER_HXX

#include <algorithm>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/tvector.hxx"

#ifndef LIB_MFRONT_CYRANO_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CYRANO_CALL_HXX */

namespace cyrano {

  /*!
   * \brief Generic handler for the plane stress hypothesis
   *
   * This allows behaviours written for the generalised plane stress
   * hypothesis to be used in plane stress computations.
   */
  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct CyranoGenericPlaneStressHandler : public CyranoInterfaceExceptions {
    /*!
     * Main entry point This mainly choose between a specific
     * handler. The choice is made from the nature of elastic
     * behaviour (isotropic or not).
     */
    TFEL_CYRANO_INLINE2 static void exe(
        const CyranoInt *const NTENS,
        const CyranoReal *const DTIME,
        const CyranoReal *const DROT,
        CyranoReal *const DDSOE,
        const CyranoReal *const STRAN,
        const CyranoReal *const DSTRAN,
        const CyranoReal *const TEMP,
        const CyranoReal *const DTEMP,
        const CyranoReal *const PROPS,
        const CyranoInt *const NPROPS,
        const CyranoReal *const PREDEF,
        const CyranoReal *const DPRED,
        CyranoReal *const STATEV,
        const CyranoInt *const NSTATV,
        CyranoReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op) {
      using namespace tfel::meta;
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::PLANESTRESS;
      //! a simple alias
      typedef CyranoTraits<Behaviour<H, CyranoReal, false>> Traits;
      typedef
          typename std::conditional<Traits::stype == cyrano::ISOTROPIC,
                                    TreatPlaneStressIsotropicBehaviour,
                                    TreatPlaneStressOrthotropicBehaviour>::type
              Handler;
      CyranoInterfaceExceptions::checkNTENSValue(
          *NTENS, Traits::ThermodynamicForceVariableSize);
      Handler::exe(DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
                   NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, op);
    }  // end of exe

   private:
    TFEL_CYRANO_INLINE2 static void checkNSTATV(const CyranoInt NSTATV) {
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef Behaviour<H, CyranoReal, false> BV;
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
    TFEL_CYRANO_INLINE2 static void exe(
        const CyranoReal c1,
        const CyranoReal c2,
        const CyranoReal c3,
        const CyranoReal *const DTIME,
        const CyranoReal *const DROT,
        CyranoReal *const DDSOE,
        const CyranoReal *const STRAN,
        const CyranoReal *const DSTRAN,
        const CyranoReal *const TEMP,
        const CyranoReal *const DTEMP,
        const CyranoReal *const PROPS,
        const CyranoInt *const NPROPS,
        const CyranoReal *const PREDEF,
        const CyranoReal *const DPRED,
        CyranoReal *const STATEV,
        const CyranoInt *const NSTATV,
        CyranoReal *const STRESS,
        const tfel::material::OutOfBoundsPolicy op) {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::GENERALISEDPLANESTRAIN;
      using tfel::fsalgo::copy;
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short NSTATV_ = Traits::internal_variables_nb + 1u;
      typedef typename std::conditional<
          (NSTATV_ < 20), tfel::math::tvector<NSTATV_, CyranoReal>,
          tfel::math::vector<CyranoReal>>::type SVector;
      CyranoGenericPlaneStressHandler::checkNSTATV(*NSTATV);
      unsigned int iter;
      const unsigned int iterMax = 50;

      CyranoReal eto[4];
      CyranoReal deto[4];
      CyranoReal s[4];
      SVector v;
      CyranoGenericPlaneStressHandler::resize(v, NSTATV_);
      CyranoReal dez;
      CyranoReal x[2];
      CyranoReal f[2];

      dez = c1 * DSTRAN[0] + c2 * DSTRAN[1];
      CyranoGenericPlaneStressHandler::template iter<
          GeneralisedPlaneStrainBehaviour>(
          DTIME, DROT, DDSOE, TEMP, DTEMP, PROPS, NPROPS, PREDEF, DPRED, STATEV,
          STRESS, STRAN, DSTRAN, dez, &v[0], s, eto, deto, op);
      x[1] = dez;
      f[1] = s[2];

      if (abs(c3 * s[2]) > 1.e-12) {
        dez -= c3 * s[2];
        CyranoGenericPlaneStressHandler::template iter<
            GeneralisedPlaneStrainBehaviour>(
            DTIME, DROT, DDSOE, TEMP, DTEMP, PROPS, NPROPS, PREDEF, DPRED,
            STATEV, STRESS, STRAN, DSTRAN, dez, &v[0], s, eto, deto, op);
      }

      iter = 2;
      while ((abs(c3 * s[2]) > 1.e-12) && (iter < iterMax)) {
        x[0] = x[1];
        f[0] = f[1];
        x[1] = dez;
        f[1] = s[2];
        dez -= (x[1] - x[0]) / (f[1] - f[0]) * s[2];
        CyranoGenericPlaneStressHandler::template iter<
            GeneralisedPlaneStrainBehaviour>(
            DTIME, DROT, DDSOE, TEMP, DTEMP, PROPS, NPROPS, PREDEF, DPRED,
            STATEV, STRESS, STRAN, DSTRAN, dez, &v[0], s, eto, deto, op);
        ++iter;
      }
      if (iter == iterMax) {
        throwPlaneStressMaximumNumberOfIterationsReachedException(
            Traits::getName());
      }
      copy<4>::exe(s, STRESS);
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
    TFEL_CYRANO_INLINE2 static void iter(
        const CyranoReal *const DTIME,
        const CyranoReal *const DROT,
        CyranoReal *const DDSOE,
        const CyranoReal *const TEMP,
        const CyranoReal *const DTEMP,
        const CyranoReal *const PROPS,
        const CyranoInt *const NPROPS,
        const CyranoReal *const PREDEF,
        const CyranoReal *const DPRED,
        const CyranoReal *const STATEV,
        const CyranoReal *const STRESS,
        const CyranoReal *const STRAN,
        const CyranoReal *const DSTRAN,
        const CyranoReal dez,
        CyranoReal *const v,
        CyranoReal *const s,
        CyranoReal *const eto,
        CyranoReal *const deto,
        const tfel::material::OutOfBoundsPolicy op) {
      using namespace tfel::material;
      using tfel::fsalgo::copy;
      const ModellingHypothesis::Hypothesis H =
          ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef Behaviour<H, CyranoReal, false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const CyranoInt nNSTATV = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = Traits::internal_variables_nb + 1u;

      copy<4>::exe(STRESS, s);
      copy<4>::exe(STRAN, eto);
      copy<4>::exe(DSTRAN, deto);
      eto[2] = STATEV[NSTATV_ - 1];
      deto[2] = dez;
      std::copy(STATEV, STATEV + NSTATV_, v);

      GeneralisedPlaneStrainBehaviour::exe(DTIME, DROT, DDSOE, eto, deto, TEMP,
                                           DTEMP, PROPS, NPROPS, PREDEF, DPRED,
                                           v, &nNSTATV, s, op);
    }

    struct TreatPlaneStressIsotropicBehaviour {
      TFEL_CYRANO_INLINE2 static void exe(
          const CyranoReal *const DTIME,
          const CyranoReal *const DROT,
          CyranoReal *const DDSOE,
          const CyranoReal *const STRAN,
          const CyranoReal *const DSTRAN,
          const CyranoReal *const TEMP,
          const CyranoReal *const DTEMP,
          const CyranoReal *const PROPS,
          const CyranoInt *const NPROPS,
          const CyranoReal *const PREDEF,
          const CyranoReal *const DPRED,
          CyranoReal *const STATEV,
          const CyranoInt *const NSTATV,
          CyranoReal *const STRESS,
          const tfel::material::OutOfBoundsPolicy op) {
        using namespace tfel::material;
        const ModellingHypothesis::Hypothesis H =
            ModellingHypothesis::GENERALISEDPLANESTRAIN;
        typedef CyranoIsotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR, H,
                                                Behaviour>
            BehaviourHandler;
        const CyranoReal y = PROPS[0];  // Young Modulus
        const CyranoReal n = PROPS[1];  // Poisson ratio
        const CyranoReal c1 = -n / (1 - n);
        const CyranoReal c3 = 1 / y;
        CyranoGenericPlaneStressHandler::template exe<BehaviourHandler>(
            c1, c1, c3, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
            NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, op);
      }  // end of exe
    };   // end of struct TreatPlanStressIsotropicBehaviour

    struct TreatPlaneStressOrthotropicBehaviour {
      TFEL_CYRANO_INLINE2 static void exe(
          const CyranoReal *const DTIME,
          const CyranoReal *const DROT,
          CyranoReal *const DDSOE,
          const CyranoReal *const STRAN,
          const CyranoReal *const DSTRAN,
          const CyranoReal *const TEMP,
          const CyranoReal *const DTEMP,
          const CyranoReal *const PROPS,
          const CyranoInt *const NPROPS,
          const CyranoReal *const PREDEF,
          const CyranoReal *const DPRED,
          CyranoReal *const STATEV,
          const CyranoInt *const NSTATV,
          CyranoReal *const STRESS,
          const tfel::material::OutOfBoundsPolicy op) {
        using namespace tfel::meta;
        using namespace tfel::material;
        const ModellingHypothesis::Hypothesis H =
            ModellingHypothesis::GENERALISEDPLANESTRAIN;
        using tfel::fsalgo::copy;
        typedef Behaviour<H, CyranoReal, false> BV;
        typedef MechanicalBehaviourTraits<BV> Traits;
        typedef CyranoOrthotropicBehaviourHandler<STANDARDSTRAINBASEDBEHAVIOUR,
                                                  H, Behaviour>
            BehaviourHandler;
        const unsigned short offset = CyranoTraits<BV>::propertiesOffset;
        const unsigned short nprops = Traits::material_properties_nb;
        const unsigned short NPROPS_ =
            offset + nprops == 0 ? 1u : offset + nprops;
        CyranoReal nPROPS[NPROPS_];
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
        nPROPS[12] = CyranoReal(0);
        copy<nprops>::exe(PROPS + 13, nPROPS + 13);

        // S11 = 1/E1
        const CyranoReal S11 = 1 / nPROPS[0];
        // S22 = 1/E2
        const CyranoReal S22 = 1 / nPROPS[1];
        // S22 = 1/E3
        const CyranoReal S33 = 1 / nPROPS[2];
        // S12 = -n12/E1
        const CyranoReal S12 = -nPROPS[3] / nPROPS[0];
        // S13 = -n13/E1
        const CyranoReal S13 = -nPROPS[5] / nPROPS[0];
        // S23 = -n23/E2
        const CyranoReal S23 = -nPROPS[4] / nPROPS[1];
        const CyranoReal detS = S11 * S22 * S33 + 2 * S23 * S13 * S12 -
                                S11 * S23 * S23 - S22 * S13 * S13 -
                                S33 * S12 * S12;
        const CyranoReal C22 = (S11 * S22 - S12 * S12) / detS;
        const CyranoReal C20 = (S12 * S23 - S13 * S22) / detS;
        const CyranoReal C21 = (S12 * S13 - S11 * S23) / detS;
        // Plane stress condition leads to :
        // Dezz = - (1/C22)*(C20*Dexx+C21*Deyy)
        const CyranoReal c1 = -C20 / C22;
        const CyranoReal c2 = -C21 / C22;
        // calling the resolution
        CyranoGenericPlaneStressHandler::template exe<BehaviourHandler>(
            c1, c2, S22, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP, DTEMP, nPROPS,
            NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, op);
      }  // end of exe
    };   // end of struct TreatPlanStressOrthotropicBehaviour

  };  // end of struct CyranoGenericPlaneStressHandlerTreatPlaneStrain

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANO_CYRANOGENERICPLANESTRESSHANDLER_HXX */
