/*!
 * \file  include/TFEL/System/ExternalFunctionsPrototypes.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 26 jan 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX
#define LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif /* __cplusplus */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define TFEL_ADDCALL_PTR __cdecl *
#else
#define TFEL_ADDCALL_PTR *
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */

#ifdef __cplusplus
extern "C" {
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */

#ifdef TFEL_ARCH32
typedef int CyranoIntegerType;
typedef int CastemIntegerType;
typedef int AsterIntegerType;
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */
#ifdef TFEL_ARCH64
#ifdef _WIN64
typedef long long CyranoIntegerType;
typedef long long CastemIntegerType;
typedef long long AsterIntegerType;
#else
typedef long CyranoIntegerType;
typedef long CastemIntegerType;
typedef long AsterIntegerType;
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */

typedef int AbaqusIntegerType;
typedef int AnsysIntegerType;
typedef int EuroplexusIntegerType;
typedef int CalculiXIntegerType;

typedef double AbaqusRealType;
typedef double AnsysRealType;
typedef double CyranoRealType;
typedef double CastemRealType;
typedef double AsterRealType;
typedef double EuroplexusRealType;
typedef double CalculiXRealType;

#ifdef __cplusplus
}
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */

#ifdef __cplusplus

namespace tfel {

  namespace system {

    extern "C" {

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CastemFunctionPtr)(const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction0Ptr)();

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction1Ptr)(double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction2Ptr)(double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction3Ptr)(double, double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction4Ptr)(double,
                                                   double,
                                                   double,
                                                   double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction5Ptr)(
        double, double, double, double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction6Ptr)(
        double, double, double, double, double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction7Ptr)(
        double, double, double, double, double, double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction8Ptr)(
        double, double, double, double, double, double, double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction9Ptr)(
        double, double, double, double, double, double, double, double, double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction10Ptr)(double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction11Ptr)(double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction12Ptr)(double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction13Ptr)(double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction14Ptr)(double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR CFunction15Ptr)(double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double,
                                                    double);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction0Ptr)();

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction1Ptr)(const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction2Ptr)(const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction3Ptr)(const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction4Ptr)(const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction5Ptr)(const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction6Ptr)(const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction7Ptr)(const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction8Ptr)(const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction9Ptr)(const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *,
                                                         const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction10Ptr)(const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction11Ptr)(const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction12Ptr)(const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction13Ptr)(const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction14Ptr)(const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *);

    //! a simple alias
    typedef double(TFEL_ADDCALL_PTR FortranFunction15Ptr)(const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *,
                                                          const double *);
    /*!
     * a simple alias. This is the prototype of the external
     * functions used by the pleiades version of the castem finite
     * element solver for the castem version prior to 2014.
     */
    typedef void(TFEL_ADDCALL_PTR CyranoFctPtr)(
        const CyranoIntegerType
            *const, /* nombre de composantes des contraintes  */
        const CyranoRealType *const, /* incrément de temps */
        const CyranoRealType *const, /* matrice de passage du repère élement
                                        au repère global */
        CyranoRealType *const, /* matrice tangente                       */
        const CyranoRealType *const, /* tenseur des déformations */
        const CyranoRealType
            *const, /* tenseur des incréments de déformations */
        const CyranoRealType *const,    /* température au début du pas    */
        const CyranoRealType *const,    /* incrément de température    */
        const CyranoRealType *const,    /* propriétés matériau    */
        const CyranoIntegerType *const, /* nombre de propriétés matériau */
        const CyranoRealType *const,    /* variables externes */
        const CyranoRealType *const,    /* incréments des variables externes */
        CyranoRealType *const,          /* variables internes   */
        const CyranoIntegerType *const, /* nombre de variables internes */
        CyranoRealType *const,          /* tenseur des contraintes */
        const CyranoIntegerType
            *const,                /* entier définissant le type de calcul  */
        CyranoIntegerType *const); /* code sortie */

    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * pleiades version of the castem finite element solver
     */
    typedef void(TFEL_ADDCALL_PTR CastemFctPtr)(
        CastemRealType *const, /* stress                   */
        CastemRealType *const, /* internal state variables */
        CastemRealType *const, /* tangent operator         */
        CastemRealType *const,
        CastemRealType *const,
        CastemRealType *const,
        CastemRealType *const,
        CastemRealType *const,
        CastemRealType *const,
        CastemRealType *const,
        const CastemRealType *const, /* strain tensor    */
        const CastemRealType *const, /* strain increment */
        const CastemRealType *const,
        const CastemRealType *const, /* time increment   */
        const CastemRealType *const, /* temperature      */
        const CastemRealType *const, /* temperature increment    */
        const CastemRealType *const, /* external state variables */
        const CastemRealType *const, /* external state variables increments   */
        const char *const,
        const CastemIntegerType *const,
        const CastemIntegerType *const,
        const CastemIntegerType *const, /* number of components of tensors */
        const CastemIntegerType *const, /* number of internal state variables */
        const CastemRealType *const, /* material properties                   */
        const CastemIntegerType *const, /* number of material properties */
        const CastemRealType *const,
        const CastemRealType *const, /* rotation matrix                       */
        CastemRealType *const,       /* estimation of the next time increment */
        const CastemRealType *const,
        const CastemRealType *const,
        const CastemRealType *const,
        const CastemIntegerType *const,
        const CastemIntegerType *const,
        const CastemIntegerType *const,
        const CastemIntegerType *const,
        const CastemIntegerType *const,
        CastemIntegerType *const,
        const int /* hidden fortran parameter */);
    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * pleiades version of the castem finite element solver
     */
    typedef void(TFEL_ADDCALL_PTR AbaqusFctPtr)(
        AbaqusRealType *const, /* stress                   */
        AbaqusRealType *const, /* internal state variables */
        AbaqusRealType *const, /* tangent operator         */
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        const AbaqusRealType *const, /* strain tensor    */
        const AbaqusRealType *const, /* strain increment */
        const AbaqusRealType *const,
        const AbaqusRealType *const, /* time increment   */
        const AbaqusRealType *const, /* temperature      */
        const AbaqusRealType *const, /* temperature increment    */
        const AbaqusRealType *const, /* external state variables */
        const AbaqusRealType *const, /* external state variables increments   */
        const char *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const, /* number of components of tensors */
        const AbaqusIntegerType *const, /* number of internal state variables */
        const AbaqusRealType *const, /* material properties                   */
        const AbaqusIntegerType *const, /* number of material properties */
        const AbaqusRealType *const,
        const AbaqusRealType *const, /* rotation matrix                       */
        AbaqusRealType *const,       /* estimation of the next time increment */
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        AbaqusIntegerType *const,
        const int /* hidden fortran parameter */);
    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * pleiades version of the castem finite element solver
     */
    typedef void(TFEL_ADDCALL_PTR AnsysFctPtr)(const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysIntegerType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               AnsysRealType *const,
                                               AnsysRealType *const,
                                               AnsysRealType *const,
                                               AnsysRealType *const,
                                               AnsysRealType *const,
                                               AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const,
                                               const AnsysRealType *const);
    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * abaqus explicit solver
     */
    typedef void(TFEL_ADDCALL_PTR AbaqusExplicitFctPtr)(
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusIntegerType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const char *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        const AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        AbaqusRealType *const,
        const int /* hidden fortran parameter */);
    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * Code_Aster finite element solver
     */
    typedef void(TFEL_ADDCALL_PTR AsterFctPtr)(
        AsterRealType *const,       /* stress                   */
        AsterRealType *const,       /* internal state variables */
        AsterRealType *const,       /* tangent operator         */
        const AsterRealType *const, /* strain tensor    */
        const AsterRealType *const, /* strain increment */
        const AsterRealType *const, /* time increment   */
        const AsterRealType *const, /* temperature      */
        const AsterRealType *const, /* temperature increment    */
        const AsterRealType *const, /* external state variables */
        const AsterRealType *const, /* external state variables increments   */
        const AsterIntegerType *const, /* number of components of tensors */
        const AsterIntegerType *const, /* number of internal state variables */
        const AsterRealType *const, /* material properties                   */
        const AsterIntegerType *const, /* number of material properties */
        const AsterRealType *const, /* rotation matrix                       */
        AsterRealType *const,       /* estimation of the next time increment */
        const AsterIntegerType *const);
    //! a simple alias
    using AsterIntegrationErrorMessageFctPtr = const char *(*)();
    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * pleiades version of the castem finite element solver
     */
    typedef void(TFEL_ADDCALL_PTR EuroplexusFctPtr)(
        EuroplexusIntegerType
            *const,                 // status of the integration, 0 on success
        EuroplexusRealType *const,  // Cauchy stress
        EuroplexusRealType *const,  // state variables
        EuroplexusRealType *const,  // stiffness tensor
        EuroplexusRealType *const,  // time step scaling factor
        EuroplexusIntegerType *const,        // broken indicator
        char *const,                         // error message
        const EuroplexusIntegerType *const,  // number of state variables
        const EuroplexusIntegerType *const,  // modelling hypothesis
        const EuroplexusRealType *const,     // time increment
        const EuroplexusRealType
            *const,  // deformation gradient at the beginning of the time step
        const EuroplexusRealType
            *const,  // deformation gradient at the end of the time step
        const EuroplexusRealType *const,     // rotation matrix
        const EuroplexusRealType *const,     // material properties
        const EuroplexusIntegerType *const,  // number of material properties
        const EuroplexusRealType *const,     // temperature
        const EuroplexusRealType *const,     // increment of temperature
        const EuroplexusRealType *const,     // external state variables
        const EuroplexusRealType *const,  // external state variables increments
        const EuroplexusIntegerType
            *const);  // number of external state variables
    /*!
     * a simple alias.
     * This is the prototype of the external functions used by the
     * pleiades version of the castem finite element solver
     */
    typedef void(TFEL_ADDCALL_PTR CalculiXFctPtr)(
        const char *const,
        const CalculiXIntegerType *const,
        const CalculiXIntegerType *const,
        const CalculiXIntegerType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXIntegerType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        const CalculiXIntegerType *const,
        const CalculiXIntegerType *const,
        const CalculiXIntegerType *const,
        const CalculiXIntegerType *const,
        const CalculiXRealType *const,
        CalculiXRealType *const,
        CalculiXRealType *const,
        CalculiXRealType *const,
        const CalculiXIntegerType *const,
        const CalculiXRealType *const,
        const CalculiXRealType *const,
        CalculiXRealType *const,
        const CalculiXIntegerType *const,
        const int /* hidden fortran parameter */);
    }

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */

#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */
