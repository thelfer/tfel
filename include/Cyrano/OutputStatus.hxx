/*!
 * \file   OutputStatus.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_CYRANO_OUTPUTSTATUS_HXX
#define LIB_CYRANO_OUTPUTSTATUS_HXX

#include "Cyrano/Types.hxx"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief this structure summarizes the exit status of a function conforming to
 * one of the `Cyrano`'
 * interface.
 */
typedef struct {
  /*!
   * \brief exit status
   *
   * The exit status is zero if the result has been correctly evaluated.
   *
   * If the exit status is 1, a result has been computed, but it must be used
   * with caution. This is typically used to report that one argument was out of
   * its bounds.
   *
   * All negative values indicates that the result is not usable. For a material
   * property, thereturned is `nan`.
   *
   * For a material property, a negative value has the following meaning:
   *
   * - If the exit status is -1, an argument was out of its physical bounds, or
   *   out of its bounds and a strict out of bounds policy is declared.
   * - If the exit status is -2, a C++ exception was thrown. If the exception
   *   was a child of `std::exception`, the content of the string returned by
   *   the `what` method is copyied in the `message` field. Otherwise, the
   *   message field contains the "unknown exception" string.
   * - If the exit status is -3, an error occured in the `C` library, i.e. the
   *   `errno` value was set to a non zero value during the computation.
   *   The value of `errno` corresponding to the error is stored to in the
   *   `c_error_number` field of this structure. The string returned by
   *   `strerrno` is returned. Note that the `errno` value is always reset to
   *   the  value it had before the call.
   * - If the exit status is -4, the computed value is invalid (either \nan`,
   *   `inf`, or `-inf`).
   * - If the exit status is -5, the number of arguments is invalid.
   */
  CyranoIntegerType status;
  //! \brief error number reported by the C library.
  CyranoIntegerType c_error_number;
  /*!
   * \brief bounds status
   * This status has the following meaning:
   * - zero means that no argument was outside its bounds or its physical
   * bounds.
   * - a negative values means that one argument went beyond its physical
   * bounds.
   *   The absolute value gives the rank of this argument (here the rank starts
   * at 1).
   * - a positive value means that one argument went beyond its bounds.
   *   The value gives the rank of this argument (here the rank starts at 1).
   */
  CyranoIntegerType bounds_status;
  //! \brief error message
  char msg[512];
} CyranoOutputStatus;  // end of struct CyranoMaterialPropertyOutputStatus

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_CYRANO_OUTPUTSTATUS_HXX */
