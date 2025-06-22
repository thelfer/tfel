/*!
 * \file   mfront/include/MFront/Aster/AsterInterface.hxx
 * \brief  This file implements the AsterInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTER_CALL_H_
#define LIB_MFRONT_ASTER_CALL_H_

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <type_traits>

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Exception/TFELException.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Material/MechanicalBehaviourTraits.hxx"
#include "TFEL/Material/MaterialException.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterConfig.hxx"
#include "MFront/Aster/AsterTraits.hxx"
#include "MFront/Aster/AsterException.hxx"
#include "MFront/Aster/AsterInterfaceExceptions.hxx"

namespace aster {

  /*!
   * forward declaration
   */
  template <
      AsterBehaviourType,
      tfel::material::ModellingHypothesis::Hypothesis,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class>
  struct AsterIsotropicBehaviourHandler;

  /*!
   * forward declaration
   */
  template <
      AsterBehaviourType,
      tfel::material::ModellingHypothesis::Hypothesis,
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class>
  struct AsterOrthotropicBehaviourHandler;

  /*!
   * \class  AsterInterface
   * \brief This class create an interface between a behaviour class
   * and the aster finite element code
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   *
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template <
      template <tfel::material::ModellingHypothesis::Hypothesis, typename, bool>
      class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterInterface
      : protected AsterInterfaceExceptions {
    TFEL_ASTER_INLINE2 static int exe(
        const AsterInt *const NTENS,
        const AsterReal *const DTIME,
        const AsterReal *const DROT,
        AsterReal *const DDSOE,
        const AsterReal *const STRAN,
        const AsterReal *const DSTRAN,
        const AsterReal *const TEMP,
        const AsterReal *const DTEMP,
        const AsterReal *const PROPS,
        const AsterInt *const NPROPS,
        const AsterReal *const PREDEF,
        const AsterReal *const DPRED,
        AsterReal *const STATEV,
        const AsterInt *const NSTATV,
        AsterReal *const STRESS,
        const AsterInt *const NUMMOD,
        const tfel::material::OutOfBoundsPolicy op,
        const StressFreeExpansionHandler &sfeh) {
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef ModellingHypothesis ModellingHypothesis;
      if (*NUMMOD == 2u) {
        /*!
         * L'hypothèse de déformations planes généralisées n'est pas
         * supportée dans Aster (version 12.x) Cette hypothèse doit
         * être appelée quand les contraintes planes sont traitées par
         * l'algorithme de De Borst.
         */
        using BV = Behaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN,
                             AsterReal, false>;
        const bool is_defined = MechanicalBehaviourTraits<BV>::is_defined;
        typedef typename std::conditional<
            is_defined,
            CallBehaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN>,
            Behaviour2DWrapper<AsterTraits<BV>::btype,
                               ModellingHypothesis::GENERALISEDPLANESTRAIN>>::
            type Handler;
        return Handler::exe(NTENS, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP,
                            DTEMP, PROPS, NPROPS, PREDEF, DPRED, STATEV, NSTATV,
                            STRESS, op, sfeh);
      } else if (*NUMMOD == 4u) {
        typedef Behaviour<ModellingHypothesis::AXISYMMETRICAL, AsterReal, false>
            BV;
        const bool is_defined = MechanicalBehaviourTraits<BV>::is_defined;
        typedef typename std::conditional<
            is_defined, CallBehaviour<ModellingHypothesis::AXISYMMETRICAL>,
            Behaviour2DWrapper<AsterTraits<BV>::btype,
                               ModellingHypothesis::AXISYMMETRICAL>>::type
            Handler;
        return Handler::exe(NTENS, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP,
                            DTEMP, PROPS, NPROPS, PREDEF, DPRED, STATEV, NSTATV,
                            STRESS, op, sfeh);
      } else if (*NUMMOD == 5u) {
        typedef Behaviour<ModellingHypothesis::PLANESTRESS, AsterReal, false>
            BV;
        const bool is_defined = MechanicalBehaviourTraits<BV>::is_defined;
        typedef typename std::conditional<
            is_defined, CallBehaviour<ModellingHypothesis::PLANESTRESS>,
            Behaviour2DWrapper<AsterTraits<BV>::btype,
                               ModellingHypothesis::PLANESTRESS>>::type Handler;
        return Handler::exe(NTENS, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP,
                            DTEMP, PROPS, NPROPS, PREDEF, DPRED, STATEV, NSTATV,
                            STRESS, op, sfeh);
      } else if (*NUMMOD == 6u) {
        typedef Behaviour<ModellingHypothesis::PLANESTRAIN, AsterReal, false>
            BV;
        const bool is_defined = MechanicalBehaviourTraits<BV>::is_defined;
        typedef typename std::conditional<
            is_defined, CallBehaviour<ModellingHypothesis::PLANESTRAIN>,
            Behaviour2DWrapper<AsterTraits<BV>::btype,
                               ModellingHypothesis::PLANESTRAIN>>::type Handler;
        return Handler::exe(NTENS, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP,
                            DTEMP, PROPS, NPROPS, PREDEF, DPRED, STATEV, NSTATV,
                            STRESS, op, sfeh);
      } else if (*NUMMOD == 3u) {
        using BV =
            Behaviour<ModellingHypothesis::TRIDIMENSIONAL, AsterReal, false>;
        const bool is_defined = MechanicalBehaviourTraits<BV>::is_defined;
        using Handler = typename std::conditional<
            is_defined, CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>,
            UnsupportedHypothesis>::type;
        return Handler::exe(NTENS, DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP,
                            DTEMP, PROPS, NPROPS, PREDEF, DPRED, STATEV, NSTATV,
                            STRESS, op, sfeh);
      } else {
        AsterInterfaceExceptions::displayUnsupportedHypothesisMessage();
        return -2;
      }
    }

   private:
    //
    struct UnsupportedHypothesis {
      TFEL_ASTER_INLINE2 static int exe(const AsterInt *const,
                                        const AsterReal *const,
                                        const AsterReal *const,
                                        AsterReal *const,
                                        const AsterReal *const,
                                        const AsterReal *const,
                                        const AsterReal *const,
                                        const AsterReal *const,
                                        const AsterReal *const,
                                        const AsterInt *const,
                                        const AsterReal *const,
                                        const AsterReal *const,
                                        AsterReal *const,
                                        const AsterInt *const,
                                        AsterReal *const,
                                        const tfel::material::OutOfBoundsPolicy,
                                        const StressFreeExpansionHandler &) {
        return -2;
      }
    };
    template <AsterBehaviourType,
              tfel::material::ModellingHypothesis::Hypothesis>
    struct Behaviour2DWrapper;

    template <tfel::material::ModellingHypothesis::Hypothesis H>
    struct Behaviour2DWrapper<aster::SMALLSTRAINSTANDARDBEHAVIOUR, H> {
      TFEL_ASTER_INLINE2 static int exe(
          const AsterInt *const,
          const AsterReal *const DTIME,
          const AsterReal *const DROT,
          AsterReal *const DDSOE,
          const AsterReal *const STRAN,
          const AsterReal *const DSTRAN,
          const AsterReal *const TEMP,
          const AsterReal *const DTEMP,
          const AsterReal *const PROPS,
          const AsterInt *const NPROPS,
          const AsterReal *const PREDEF,
          const AsterReal *const DPRED,
          AsterReal *const STATEV,
          const AsterInt *const NSTATV,
          AsterReal *const STRESS,
          const tfel::material::OutOfBoundsPolicy op,
          const StressFreeExpansionHandler &sfeh) {
        using namespace tfel::meta;
        using namespace tfel::math;
        using namespace tfel::material;
        using tfel::fsalgo::copy;
        const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
        AsterInt NTENS = 6u;
        AsterReal s[6u] = {0., 0., 0., 0., 0., 0.};
        AsterReal e[6u] = {0., 0., 0., 0., 0., 0.};
        AsterReal de[6u] = {0., 0., 0., 0., 0., 0.};
        AsterReal K[36u] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                            0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                            0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
        K[0u] = DDSOE[0];
        copy<StensorDimeToSize<N>::value>::exe(STRESS, s);
        copy<StensorDimeToSize<N>::value>::exe(STRAN, e);
        copy<StensorDimeToSize<N>::value>::exe(DSTRAN, de);
        int r = CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(
            &NTENS, DTIME, DROT, K, e, de, TEMP, DTEMP, PROPS, NPROPS, PREDEF,
            DPRED, STATEV, NSTATV, s, op, sfeh);
        if (r == 0) {
          AsterReduceTangentOperator<N>::exe(DDSOE, K);
          copy<StensorDimeToSize<N>::value>::exe(s, STRESS);
        }
        return r;
      }
    };

    template <tfel::material::ModellingHypothesis::Hypothesis H>
    struct Behaviour2DWrapper<aster::FINITESTRAINSTANDARDBEHAVIOUR, H> {
      TFEL_ASTER_INLINE2 static int exe(
          const AsterInt *const,
          const AsterReal *const DTIME,
          const AsterReal *const DROT,
          AsterReal *const DDSOE,
          const AsterReal *const STRAN,
          const AsterReal *const DSTRAN,
          const AsterReal *const TEMP,
          const AsterReal *const DTEMP,
          const AsterReal *const PROPS,
          const AsterInt *const NPROPS,
          const AsterReal *const PREDEF,
          const AsterReal *const DPRED,
          AsterReal *const STATEV,
          const AsterInt *const NSTATV,
          AsterReal *const STRESS,
          const tfel::material::OutOfBoundsPolicy op,
          const StressFreeExpansionHandler &sfeh) {
        using namespace std;
        using namespace tfel::meta;
        using namespace tfel::math;
        using namespace tfel::material;
        using tfel::fsalgo::copy;
        const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
        AsterInt NTENS = 6u;
        AsterReal s[6u] = {0., 0., 0., 0., 0., 0.};
        AsterReal F0[9u] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
        AsterReal F1[9u] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
        copy<StensorDimeToSize<N>::value>::exe(STRESS, s);
        copy<9u>::exe(STRAN, F0);
        copy<9u>::exe(DSTRAN, F1);
        int r = CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(
            &NTENS, DTIME, DROT, DDSOE, F0, F1, TEMP, DTEMP, PROPS, NPROPS,
            PREDEF, DPRED, STATEV, NSTATV, s, op, sfeh);
        if (r == 0) {
          copy<StensorDimeToSize<N>::value>::exe(s, STRESS);
        }
        return r;
      }
    };

    template <tfel::material::ModellingHypothesis::Hypothesis H>
    struct Behaviour2DWrapper<aster::COHESIVEZONEMODEL, H> {
      TFEL_ASTER_INLINE2 static int exe(
          const AsterInt *const,
          const AsterReal *const DTIME,
          const AsterReal *const DROT,
          AsterReal *const DDSOE,
          const AsterReal *const STRAN,
          const AsterReal *const DSTRAN,
          const AsterReal *const TEMP,
          const AsterReal *const DTEMP,
          const AsterReal *const PROPS,
          const AsterInt *const NPROPS,
          const AsterReal *const PREDEF,
          const AsterReal *const DPRED,
          AsterReal *const STATEV,
          const AsterInt *const NSTATV,
          AsterReal *const STRESS,
          const tfel::material::OutOfBoundsPolicy op,
          const StressFreeExpansionHandler &sfeh) {
        using tfel::fsalgo::copy;
        using tfel::material::ModellingHypothesis;
        AsterInt NTENS = 6u;
        AsterReal s[3u] = {0., 0., 0.};
        AsterReal u0[3u] = {0., 0., 0.};
        AsterReal du[3u] = {0., 0., 0.};
        AsterReal K[9u] = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
        K[0u] = DDSOE[0];
        copy<2u>::exe(STRESS, s);
        copy<2u>::exe(STRAN, u0);
        copy<2u>::exe(DSTRAN, du);
        int r = CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(
            &NTENS, DTIME, DROT, DDSOE, u0, du, TEMP, DTEMP, PROPS, NPROPS,
            PREDEF, DPRED, STATEV, NSTATV, s, op, sfeh);
        if (r == 0) {
          copy<2u>::exe(s, STRESS);
          DDSOE[0] = K[0];
          DDSOE[1] = K[1];
          DDSOE[2] = K[4];
          DDSOE[3] = K[5];
        }
        return r;
      }
    };  // end of Behaviour2DWrapper<aster::COHESIVEZONEMODEL,N>

    template <tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour {
      TFEL_ASTER_INLINE2 static int exe(
          const AsterInt *const,
          const AsterReal *const DTIME,
          const AsterReal *const DROT,
          AsterReal *const DDSOE,
          const AsterReal *const STRAN,
          const AsterReal *const DSTRAN,
          const AsterReal *const TEMP,
          const AsterReal *const DTEMP,
          const AsterReal *const PROPS,
          const AsterInt *const NPROPS,
          const AsterReal *const PREDEF,
          const AsterReal *const DPRED,
          AsterReal *const STATEV,
          const AsterInt *const NSTATV,
          AsterReal *const STRESS,
          const tfel::material::OutOfBoundsPolicy op,
          const StressFreeExpansionHandler &sfeh) {
        using namespace std;
        using tfel::material::MechanicalBehaviourTraits;
        typedef Behaviour<H, AsterReal, false> BV;
        typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
        typedef AsterTraits<BV> AsterTraits;
        try {
          typedef typename conditional<
              AsterTraits::type == aster::ISOTROPIC,
              AsterIsotropicBehaviourHandler<AsterTraits::btype, H, Behaviour>,
              AsterOrthotropicBehaviourHandler<AsterTraits::btype, H,
                                               Behaviour>>::type Handler;
          Handler::exe(DTIME, DROT, DDSOE, STRAN, DSTRAN, TEMP, DTEMP, PROPS,
                       NPROPS, PREDEF, DPRED, STATEV, NSTATV, STRESS, op, sfeh);
        } catch (const AsterException &e) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatAsterException(Traits::getName(), e);
          }
          return -2;
        } catch (const tfel::material::OutOfBoundsException &e) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatMaterialException(Traits::getName(),
                                                             e);
          }
          return -3;
        } catch (const tfel::material::DivergenceException &e) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatTFELException(Traits::getName(), e);
          }
          return -4;
        } catch (const tfel::material::MaterialException &e) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatStandardException(Traits::getName(),
                                                             e);
          }
          return -5;
        } catch (const tfel::exception::TFELException &e) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatTFELException(Traits::getName(), e);
          }
          return -6;
        } catch (const std::exception &e) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatStandardException(Traits::getName(),
                                                             e);
          }
          return -7;
        } catch (...) {
          if (AsterTraits::errorReportPolicy != ASTER_NOERRORREPORT) {
            AsterInterfaceExceptions::treatUnknownException(Traits::getName());
          }
          return -8;
        }
        return 0;
      }  // end of CallBehaviour::exe
    };   // end of struct CallBehaviour
  };     // end of struct AsterInterface

}  // end of namespace aster

#include "MFront/Aster/AsterBehaviourHandler.hxx"

#endif /* LIB_MFRONT_ASTER_CALL_H_ */
