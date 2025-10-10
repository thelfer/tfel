/*!
 * \file   OutOfBoundsPolicy.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_CYRANO_OUTOFBOUNDSPOLICY_HXX
#define LIB_CYRANO_OUTOFBOUNDSPOLICY_HXX

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief available out of bounds policies
 */
typedef enum {
  CYRANO_NONE_POLICY,       /*!<
                             * With this policy, nothing is done if the arguments are
                             * out of their bounds
                             * (checks are not even performed).
                             */
  CYRANO_WARNING_POLICY,    /*!<
                             * With this policy, checks on the arguments are
                             * performed. If one argument
                             * if out of its bounds, this will be reported in the
                             * output status and an
                             * appropriate error message will be reported. The
                             * computations are however
                             * performed.
                             */
  CYRANO_STRICT_POLICY      /*!<
                             * With this policy, checks on the arguments are
                             * performed. If one argument
                             * if out of its bounds, this will be reported in the
                             * output status and an
                             * appropriate error message will be reported.
                             */
} CyranoOutOfBoundsPolicy;  // end of CyranoOutOfBoundsPolicy

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_CYRANO_OUTOFBOUNDSPOLICY_HXX */
