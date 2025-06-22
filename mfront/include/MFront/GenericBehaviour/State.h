/*!
 * \file   include/MFront/GenericBehaviour/State.h
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

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_STATE_H
#define LIB_MFRONT_GENERICBEHAVIOUR_STATE_H

#include "MFront/GenericBehaviour/Types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief state of the material
 */
typedef struct {
  //! \brief value of the gradients
  double* gradients;
  //! \brief values of the thermodynamic_forces
  double* thermodynamic_forces;
  //! \brief values of the material properties
  double* material_properties;
  //! \brief values of the internal state variables
  double* internal_state_variables;
  /*!
   * \brief stored energy (computed by `@InternalEnergy` in `MFront` files).
   * This output is optional.
   */
  double* stored_energy;
  /*!
   * \brief dissipated energy (computed by `@DissipatedEnergy` in `MFront`
   * files). This output is optional.
   */
  double* dissipated_energy;
  //! \brief values of the external state variables
  double* external_state_variables;
} MFront_GB_State;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_STATE_H */
