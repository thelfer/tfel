/*!
 * \file   OutOfBoundsPolicy.h
 * \brief
 * \author Thomas Helfer
 * \date   03/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with linking exception
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_GENERIC_MATERIALPROPERTY_OUTOFBOUNDSPOLICY_H
#define LIB_GENERIC_MATERIALPROPERTY_OUTOFBOUNDSPOLICY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief available out of bounds policies
 */
typedef enum {
  GENERIC_MATERIALPROPERTY_NONE_POLICY,    /*!<
                                            * With this policy, nothing is done if
                                            * the arguments are    out of their
                                            * bounds    (checks are not even
                                            * performed).
                                            */
  GENERIC_MATERIALPROPERTY_WARNING_POLICY, /*!<
                                            * With this policy, checks on the
                                            * arguments are performed. If one
                                            * argument if out of its bounds,
                                            * this will be reported in the
                                            * output status and an
                                            * appropriate error message will be
                                            * reported. The computations are
                                            * however performed.
                                            */
  GENERIC_MATERIALPROPERTY_STRICT_POLICY   /*!<
                                            * With this policy, checks on the
                                            * arguments are   performed. If one
                                            * argument   if out of its bounds,
                                            * this   will be reported in the
                                            * output   status and an   appropriate
                                            * error   message will be reported.
                                            */
} mfront_gmp_OutOfBoundsPolicy;  // end of mfront_gmp_OutOfBoundsPolicy

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_GENERIC_MATERIALPROPERTY_OUTOFBOUNDSPOLICY_H */
