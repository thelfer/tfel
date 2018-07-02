/*!
 * \file   include/MFront/GenericBehaviour/BehaviourData.h
 * \brief
 * \author Thomas Helfer
 * \date   02/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
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
typedef struct {
  //! \brief time increment
  double *const dt;
  //! \brief consistent tangent operator
  double *const K;
  //! \brief state at the beginning of the time step
  MFront_GB_State s0;
  //! \brief state at the end of the time step
  MFront_GB_State s1;
} MFront_GB_BehaviourData;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_BEHAVIOURDATA_H */
