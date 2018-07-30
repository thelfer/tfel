/*!
 * \file   include/MFront/GenericBehaviour/Result.h
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

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_RESULT_H
#define LIB_MFRONT_GENERICBEHAVIOUR_RESULT_H

#include "MFront/GenericBehaviour/State.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief structure passed to the behaviour integration
 */
#ifndef MFRONT_GB_RESULT_FORWARD_DECLARATION
  typedef struct MFront_GB_Result MFront_GB_Result;
#endif

  struct MFront_GB_Result {
    //! \brief time increment
    double dt;
    //! \brief consistent tangent operator
    double * K;
    //! \brief proposed time step increment increase factor
    double rdt;
    /*!
     * \brief out of bounds policy
     * Valid values are the following:
     * - 0: none
     * - 1: warning
     * - 2: strict
     */
    int op;
    //! \brief state at the beginning of the time step
    MFront_GB_State s0;
    //! \brief state at the end of the time step
    MFront_GB_State s1;
  };

#ifdef __cplusplus

  namespace mfront{

    namespace gb{

      //! a simple alias
      using Result = ::MFront_GB_Result;

    } // end of namespace gb
    
  } // end of namespace mfront

#endif
  
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_RESULT_H */
