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

#include "MFront/GenericBehaviour/State.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief structure passed to the behaviour integration
 */
#ifndef MFRONT_GB_BEHAVIOURDATA_FORWARD_DECLARATION
  typedef struct MFront_GB_BehaviourData MFront_GB_BehaviourData;
#endif

  /*!
   * \brief structure 
   */
  struct MFront_GB_BehaviourData {
    /*!
     * \brief time increment
     */
    double dt;
    /*!
     * \brief the stiffness matrix.
     * On input, the first element of K must contain the type of type
     * of stiffness matrix expected. If this value is negative, only
     * the prediction operator is computed. This value has the
     * following meaning:
     * - if K[0] is lower than -2.5, the tangent operator must be
     *   computed.
     * - if K[0] is in [-2.5:-1.5]: the secant operator must be
     *   computed.
     * - if K[0] is in [-1.5:-0.5]: the elastic operator must be
     *   computed.
     * - if K[0] is in [-0.5:0.5]: the behaviour integration is
     *   performed, but no stiffness matrix.
     * - if K[0] is in [0.5:1.5]: the elastic operator must be
     *   computed.
     * - if K[0] is in [1.5:2.5]: the secant operator must be
     *   computed.
     * - if K[0] is in [2.5:3.5]: the secant operator must be
     *   computed.
     * - if K[0] is greater than 3.5, the consistent tangent operator
     *   must be computed.
     *
     * Other values of K are meant to store behaviour's option. This
     * is currently only meaningful for finite strain behaviours.
     *
     * For finite strain behaviours, K[1] holds the stress measure
     * used:
     * - if K[1] < 0.5, the Cauchy stress is used
     * - if 0.5 < K[1] < 1.5, the second Piola-Kirchoff stress is used
     * - if 1.5 < K[1] < 2.5, the first Piola-Kirchoff stress is used
     *
     * For finite strain behaviours, K[2] holds the consitent tangent
     * operator returned by the behaviour:
     * - if K[2]<0.5, the derivative of the Cauchy stress with respect
     *   to the deformation gradient is returned
     * - if 0.5<K[2]<1.5, the derivative of the second Piola-Kirchoff
     *   stress with respect to the Green-Lagrange strain
     *   is returned
     * - if 1.5<K[2]<2.5, the derivative of the first Piola-Kirchoff
     *   stress with respect to the deformation gradient is returned
     */
    double * K;
    /*!
     * \brief proposed time step increment increase factor
     */
    double rdt;
    /*!
     * \brief state at the beginning of the time step
     */
    MFront_GB_State s0;
    /*!
     * \brief state at the end of the time step
     */
    MFront_GB_State s1;
  };

#ifdef __cplusplus

  namespace mfront{

    namespace gb{

      //! a simple alias
      using BehaviourData = ::MFront_GB_BehaviourData;

    } // end of namespace gb
    
  } // end of namespace mfront

#endif
  
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_H */
