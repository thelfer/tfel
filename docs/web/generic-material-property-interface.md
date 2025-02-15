---
title: The generic interface for material properties
author: Thomas Helfer
date: 12/09/2018
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The `generic` interface for material properties generates functions
matching the following prototype:

~~~~{.cxx}
mfront_gmp_real (*)(mfront_gmp_OutputStatus* const,       // output status
                    const mfront_gmp_real* const,         // arguments
                    const mfront_gmp_size_type,           // number of arguments
                    const mfront_gmp_OutOfBoundsPolicy);  // out of bounds policy
~~~~

The `mfront_gmp_OutputStatus` structure and the
`mfront_gmp_OutOfBoundsPolicy` enumeration type are described in the
next paragraphs.

The arguments are passed using a continuous array. The number of
arguments is used to make some basic consistency checks. The list of
arguments can be retrieved using the `ExternalLibraryManager` class (See
Section @???).

If the material property declares parameters (and that the
`parameters_as_static_variables` DSL option was not specified), their
names and default values can be retrieved using the
`ExternalLibraryManager` class. The value of those parameters can also
be modified using the `ExternalLibraryManager` class (See Section @???).

> **Note: link with the `cyrano` interface for material properties**
>
> The `generic` interface for material properties is very similar to the
> `cyrano` interface. In practice, both interfaces shares a common base
> class (called `GenericMaterialPropertyInterfaceBase`).

### The `mfront_gmp_OutputStatus` structure

The `mfront_gmp_OutputStatus` structure describes the output status of
the evaluation of a material property.

This data structure is defined as follows:

~~~~{.cxx}
/*!
 * \brief this structure summarizes the exit status of a function conforming to
 * one of the `generic` material property interface.
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
  int status;
  //! \brief error number reported by the C library.
  int c_error_number;
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
  int bounds_status;
  //! \brief error message
  char msg[512];
} mfront_gmp_OutputStatus;  // end of struct mfront_gmp_OutputStatus
~~~~

### The `mfront_gmp_OutOfBoundsPolicy` enumeration type

The `mfront_gmp_OutOfBoundsPolicy` enumeration type is defined as follows:

~~~~{.cxx}
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
~~~~
