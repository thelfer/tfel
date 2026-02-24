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

//! \brief state of the material at the end of the time step
typedef struct {
  //! \brief value of the gradients
  const mfront_gb_real* gradients;
  //! \brief values of the thermodynamic_forces
  mfront_gb_real* thermodynamic_forces;
  //! \brief volumetric mass density in the reference configuration
  const mfront_gb_real* mass_density;
  //! \brief values of the material properties
  const mfront_gb_real* material_properties;
  //! \brief values of the internal state variables
  mfront_gb_real* internal_state_variables;
  /*!
   * \brief stored energy (computed by `@InternalEnergy` in `MFront` files).
   * This output is optional.
   */
  mfront_gb_real* stored_energy;
  /*!
   * \brief dissipated energy (computed by `@DissipatedEnergy` in `MFront`
   * files). This output is optional.
   */
  mfront_gb_real* dissipated_energy;
  //! \brief values of the external state variables
  const mfront_gb_real* external_state_variables;
} mfront_gb_State;

//! \brief state of the material at the beginning of the time step
typedef struct {
  //! \brief value of the gradients
  const mfront_gb_real* gradients;
  //! \brief values of the thermodynamic_forces
  const mfront_gb_real* thermodynamic_forces;
  //! \brief volumetric mass density in the reference configuration
  const mfront_gb_real* mass_density;
  //! \brief values of the material properties
  const mfront_gb_real* material_properties;
  //! \brief values of the internal state variables
  const mfront_gb_real* internal_state_variables;
  /*!
   * \brief stored energy (computed by `@InternalEnergy` in `MFront` files).
   * This output is optional.
   */
  const mfront_gb_real* stored_energy;
  /*!
   * \brief dissipated energy (computed by `@DissipatedEnergy` in `MFront`
   * files). This output is optional.
   */
  const mfront_gb_real* dissipated_energy;
  //! \brief values of the external state variables
  const mfront_gb_real* external_state_variables;
} mfront_gb_InitialState;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_STATE_H */
