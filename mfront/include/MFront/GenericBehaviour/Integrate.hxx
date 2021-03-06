/*!
 * \file   MFront/GenericBehaviour/Integrate.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_INTEGRATE_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_INTEGRATE_HXX

#include <cstring>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "TFEL/Material/MechanicalBehaviourTraits.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/GenericBehaviour/BehaviourData.h"

namespace mfront::gb {

  /*!
   * \brief export the tangent operator used by some generic behaviours.
   * \tparam StressType: numeric type used
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType>
  void exportTangentOperator(tfel::math::base_type<StressType>* const v,
                             const real K) {
    tfel::math::map<StressType>(v) = K;
  }  // end of exportTangentOperator

  /*!
   * \brief export the tangent operator used by some generic behaviours.
   * \tparam StressType: numeric type used
   * \tparam N: size of the array containing the tangent operator
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType, unsigned short N>
  void exportTangentOperator(tfel::math::base_type<StressType>* const v,
                             const tfel::math::tvector<N, StressType>& K) {
    tfel::math::map<tfel::math::tvector<N, StressType>>(v) = K;
  }  // end of exportTangentOperator

  /*!
   * \brief export the tangent operator used by cohesive zone models.
   * \tparam StressType: numeric type used
   * \tparam N: space dimension
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType, unsigned short N>
  void exportTangentOperator(tfel::math::base_type<StressType>* const v,
                             const tfel::math::tmatrix<N, N, StressType>& K) {
    tfel::math::map<tfel::math::tmatrix<N, N, StressType>>(v) = K;
  }  // end of exportTangentOperator

  /*!
   * \brief export the tangent operator used by some generic behaviours.
   * \tparam StressType: numeric type used
   * \tparam N: space dimension
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType, unsigned short N>
  void exportTangentOperator(tfel::math::base_type<StressType>* const v,
                             const tfel::math::t2tost2<N, StressType>& K) {
    tfel::math::map<tfel::math::t2tost2<N, StressType>>(v) = K;
  }  // end of exportTangentOperator

  /*!
   * \brief export the tangent operator used by some generic behaviours.
   * \tparam StressType: numeric type used
   * \tparam N: space dimension
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType, unsigned short N>
  void exportTangentOperator(tfel::math::base_type<StressType>* const v,
                             const tfel::math::t2tot2<N, StressType>& K) {
    tfel::math::map<tfel::math::t2tot2<N, StressType>>(v) = K;
  }  // end of exportTangentOperator
  /*!
   * \brief export the tangent operator used by standard small
   * strain behaviours.
   * \tparam StressType: numeric type used
   * \tparam N: space dimension
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType, unsigned short N>
  void exportTangentOperator(tfel::math::base_type<StressType>* const v,
                             const tfel::math::st2tost2<N, StressType>& K) {
    tfel::math::map<tfel::math::st2tost2<N, StressType>>(v) = K;
  }  // end of exportTangentOperator

  /*!
   * \brief export the tangent operator used by finite strain
   * strain behaviours.
   * \tparam StressType: numeric type used
   * \tparam N: space dimension
   * \param[out] v: exported values
   * \param[in] K: computed tangent operator
   */
  template <typename StressType, unsigned short N>
  void exportTangentOperator(
      tfel::math::base_type<StressType>* const v,
      const tfel::material::FiniteStrainBehaviourTangentOperator<N, StressType>&
          K) {
    if (K.template is<tfel::math::t2tot2<N, StressType>>()) {
      const auto& k = K.template get<tfel::math::t2tot2<N, StressType>>();
      tfel::math::map<tfel::math::t2tot2<N, StressType>>(v) = k;
    } else if (K.template is<tfel::math::t2tot2<N, StressType>*>()) {
      const auto& k = *(K.template get<tfel::math::t2tot2<N, StressType>*>());
      tfel::math::map<tfel::math::t2tot2<N, StressType>>(v) = k;
    } else if (K.template is<tfel::math::t2tost2<N, StressType>>()) {
      const auto& k = K.template get<tfel::math::t2tost2<N, StressType>>();
      tfel::math::map<tfel::math::t2tost2<N, StressType>>(v) = k;
    } else if (K.template is<tfel::math::t2tost2<N, StressType>*>()) {
      const auto& k = *(K.template get<tfel::math::t2tost2<N, StressType>*>());
      tfel::math::map<tfel::math::t2tost2<N, StressType>>(v) = k;
    } else if (K.template is<tfel::math::st2tost2<N, StressType>>()) {
      const auto& k = K.template get<tfel::math::st2tost2<N, StressType>>();
      tfel::math::map<tfel::math::st2tost2<N, StressType>>(v) = k;
    } else if (K.template is<tfel::math::st2tost2<N, StressType>*>()) {
      const auto& k = *(K.template get<tfel::math::st2tost2<N, StressType>*>());
      tfel::math::map<tfel::math::st2tost2<N, StressType>>(v) = k;
    } else {
      tfel::raise(
          "mfront::gb::exportTangentOperator: "
          "unsupported tangent operator type");
    }
  }  // end of exportTangentOperator

  /*!
   * \brief compute the prediction operator
   * \tparam Behaviour: class implementing the behaviour
   * \param[in,out] b: behaviour
   * \param[in,out] d: behaviour data
   * \param[in,out] smflag: consistent tangent operator expected
   */
  template <typename Behaviour>
  int computePredictionOperator(Behaviour& b,
                                mfront_gb_BehaviourData& d,
                                const typename Behaviour::SMFlag f) {
    const auto smt = [&d] {
      if (-1.5 < d.K[0]) {
        return Behaviour::ELASTIC;
      } else if ((-2.5 < d.K[0]) && (d.K[0] < -1.5)) {
        return Behaviour::SECANTOPERATOR;
      }
      return Behaviour::TANGENTOPERATOR;
    }();
    b.computePredictionOperator(f, smt);
    exportTangentOperator(d.K, b.getTangentOperator());
    return 1;
  }  // end of computePredictionOperator

  //! structure in charge of calling the computeInternalEnergy method
  struct InternalEnergyComputer {
    /*!
     * \brief call the `computeInternalEnergy` method
     * \param[in] d: behaviour data
     * \param[in] b: behaviour
     */
    template <typename Behaviour>
    static void exe(mfront_gb_BehaviourData& d, const Behaviour& b) {
      *(d.s1.stored_energy) = *(d.s0.stored_energy);
      b.computeInternalEnergy(*(d.s1.stored_energy));
    }
  };  // end of struct InternalEnergyComputer
  //! structure in charge of calling the computeDissipatedEnergy method
  struct DissipatedEnergyComputer {
    /*!
     * \brief call the `computeDissipatedEnergy` method
     * \param[in] d: behaviour data
     * \param[in] b: behaviour
     */
    template <typename Behaviour>
    static void exe(mfront_gb_BehaviourData& d, const Behaviour& b) {
      *(d.s1.dissipated_energy) = *(d.s0.dissipated_energy);
      b.computeDissipatedEnergy(*(d.s1.dissipated_energy));
    }  // end of exe
  };   // end of struct DissipatedEnergyComputer
  //! place holder for tag dispatching
  struct DoNothingEnergyComputer {
    //! \brief empty function
    template <typename Behaviour>
    static void exe(mfront_gb_BehaviourData&, const Behaviour&) {}
  };  // end of struct DoNothingEnergyComputer

  /*!
   * \brief a simple function to report integration failure when
   * an exception is thrown.
   */
  inline void reportIntegrationFailure(mfront_gb_BehaviourData& d) {
    constexpr std::size_t bsize = 511;
    if (d.error_message == nullptr) {
      return;
    }
    try {
      throw;
    } catch (std::exception& e) {
      std::strncpy(d.error_message, e.what(), bsize);
    } catch (...) {
      const char* error = "unknown exception";
      std::strncpy(d.error_message, error, bsize);
    }
    d.error_message[bsize] = '\0';
  }  // end of reportIntegrationFailure

  /*!
   * \brief integrate the behaviour over a time step
   * \tparam Behaviour: class implementing the behaviour
   * \param[in,out] d: behaviour data
   * \param[in] f: choice of consistent tangent operator
   * \param[in] p: out of bounds policy
   */
  template <typename Behaviour>
  int integrate(mfront_gb_BehaviourData& d,
                const typename Behaviour::SMFlag f,
                const tfel::material::OutOfBoundsPolicy p) {
    using MTraits = tfel::material::MechanicalBehaviourTraits<Behaviour>;
    using IEnergyComputer =
        typename std::conditional<MTraits::hasComputeInternalEnergy,
                                  InternalEnergyComputer,
                                  DoNothingEnergyComputer>::type;
    using DEnergyComputer =
        typename std::conditional<MTraits::hasComputeDissipatedEnergy,
                                  DissipatedEnergyComputer,
                                  DoNothingEnergyComputer>::type;
    using speed = typename Behaviour::speed;
    using massdensity = typename Behaviour::massdensity;
    using time = typename Behaviour::time;
    Behaviour b(d);
    b.setOutOfBoundsPolicy(p);
    auto&& rdt = tfel::math::map<time>(d.rdt);
    try {
      b.initialize();
      b.checkBounds();
      const auto bs = d.K[0] > 50;
      const auto Ke = bs ? d.K[0] - 100 : d.K[0];
      if (Ke < -0.25) {
        computePredictionOperator(b, d, f);
        if (bs) {
          tfel::math::map<speed>(d.speed_of_sound) =
              b.computeSpeedOfSound(massdensity(*(d.s0.mass_density)));
        }
      }
      const auto smt = [&Ke] {
        if (Ke < 0.5) {
          return Behaviour::NOSTIFFNESSREQUESTED;
        } else if ((0.5 < Ke) && (Ke < 1.5)) {
          return Behaviour::ELASTIC;
        } else if ((1.5 < Ke) && (Ke < 2.5)) {
          return Behaviour::SECANTOPERATOR;
        } else if ((2.5 < Ke) && (Ke < 3.5)) {
          return Behaviour::TANGENTOPERATOR;
        }
        return Behaviour::CONSISTENTTANGENTOPERATOR;
      }();
      auto tsf = b.computeAPrioriTimeStepScalingFactor(rdt);
      rdt = tsf.second;
      if (!tsf.first) {
        return -1;
      }
      const auto r = b.integrate(f, smt);
      if (r == Behaviour::FAILURE) {
        rdt = b.getMinimalTimeStepScalingFactor();
        return -1;
      }
      const auto atsf = b.computeAPosterioriTimeStepScalingFactor(rdt);
      if (rdt > atsf.second) {
        rdt = atsf.second;
      }
      if (!atsf.first) {
        return -1;
      }
      b.exportStateData(d.s1);
      if (Ke > 0.5) {
        exportTangentOperator(d.K, b.getTangentOperator());
      }
      IEnergyComputer::exe(d, b);
      DEnergyComputer::exe(d, b);
      if (bs) {
        tfel::math::map<speed>(d.speed_of_sound) =
            b.computeSpeedOfSound(massdensity(*(d.s1.mass_density)));
      }
    } catch (...) {
      reportIntegrationFailure(d);
      rdt = b.getMinimalTimeStepScalingFactor();
      return -1;
    }
    return rdt < time(0.99) ? 0 : 1;
  }  // end of integrate

}  // end of namespace mfront::gb

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_INTEGRATE_HXX */
