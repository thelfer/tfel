/*!
 * \file   include/MFront/GenericBehaviour/BehaviourData.h
 * \brief
 * \author Thomas Helfer
 * \date   02/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with linking exception
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_H
#define LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_H

#include "MFront/GenericBehaviour/Types.h"
#include "MFront/GenericBehaviour/State.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief structure passed to the behaviour integration
 */
#ifndef MFRONT_GB_BEHAVIOURDATA_FORWARD_DECLARATION
typedef struct mfront_gb_BehaviourData mfront_gb_BehaviourData;
#endif

/*!
 * \brief structure passed to the behaviour integration
 */
struct mfront_gb_BehaviourData {
  /*!
   * \brief pointer to a buffer used to store error message
   *
   * \note This pointer can be nullptr.  If not null, the pointer must
   * point to a buffer which is at least 512 characters wide (longer
   * error message are truncated).  This caller must ensure
   * thread-safety (i.e. each thread shall have its own buffer).
   */
  char* error_message;
  //! \brief time increment
  mfront_gb_real dt;
  /*!
   * \brief the stiffness matrix.
   *
   * On input, the first element of K (K[0]) must contain the type of type
   * of computation to be performed.
   *
   * If K[0] is greater than 50, the speed of sound is computed.
   *
   * Let Ke be equal to:
   *
   * - K[0] - 100 if K[0] is greater than 50
   * - K[0] otherwise.
   *
   * If Ke is negative, only the prediction operator is computed and
   * no behaviour integration is performed.
   *
   * Ke has the following meaning:
   *
   * - if Ke is lower than -2.5, the tangent operator must be
   *   computed.
   * - if Ke is in [-2.5:-1.5]: the secant operator must be
   *   computed.
   * - if Ke is in [-1.5:-0.5]: the elastic operator must be
   *   computed.
   * - if Ke is in [-0.5:0.5]: the behaviour integration is
   *   performed, but no stiffness matrix.
   * - if Ke is in [0.5:1.5]: the elastic operator must be
   *   computed.
   * - if Ke is in [1.5:2.5]: the secant operator must be
   *   computed.
   * - if Ke is in [2.5:3.5]: the secant operator must be
   *   computed.
   * - if Ke is greater than 3.5, the consistent tangent operator
   *   must be computed.
   *
   * Other values of K are meant to store behaviour's option. This
   * is currently only meaningful for finite strain behaviours.
   *
   * For finite strain behaviours, K[1] holds the stress measure
   * used:
   *
   * - if K[1] < 0.5, the Cauchy stress is used
   * - if 0.5 < K[1] < 1.5, the second Piola-Kirchoff stress is used
   * - if 1.5 < K[1] < 2.5, the first Piola-Kirchoff stress is used
   *
   * For finite strain behaviours, K[2] holds the consistent tangent
   * operator returned by the behaviour:
   *
   * - if K[2]<0.5, the derivative of the Cauchy stress with respect
   *   to the deformation gradient is returned
   * - if 0.5<K[2]<1.5, the derivative of the second Piola-Kirchoff
   *   stress with respect to the Green-Lagrange strain
   *   is returned
   * - if 1.5<K[2]<2.5, the derivative of the first Piola-Kirchoff
   *   stress with respect to the deformation gradient is returned
   */
  mfront_gb_real* K;
  //! \brief proposed time step increment increase factor
  mfront_gb_real* rdt;
  //! \brief speed of sound (only computed if requested)
  mfront_gb_real* speed_of_sound;
  //! \brief state at the beginning of the time step
  mfront_gb_InitialState s0;
  //! \brief state at the end of the time step
  mfront_gb_State s1;
};

#ifdef __cplusplus

namespace mfront::gb {

  //! \brief a simple alias
  using BehaviourData = ::mfront_gb_BehaviourData;

}  // end of namespace mfront::gb

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_H */
