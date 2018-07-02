/*!
 * \file   include/MFront/GenericBehaviour/State.h
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

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_STATE_H
#define LIB_MFRONT_GENERICBEHAVIOUR_STATE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief state of the material
 */
typedef struct{
  /*!
   * \brief gradients
   */
  double *const gradients;
  //! \brief values of the fluxes
  double *const fluxes;
  //! \brief values of the material properties
  const double *const material properties;
  //! \brief values of the internal state variables
  const double *const internal_state_variables;
  //! \brief values of the external state variables
  const double *const external_state_variables;
} MFront_GB_State;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_STATE_H */
