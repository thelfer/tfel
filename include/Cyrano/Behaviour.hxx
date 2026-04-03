/*!
 * \file   include/Cyrano/Behaviour.hxx
 * \brief
 * \author Thomas/Helfer
 * \date   14/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_CYRANO_BEHAVIOUR_HXX
#define LIB_CYRANO_BEHAVIOUR_HXX

#include "Cyrano/Types.hxx"
#include "Cyrano/OutputStatus.hxx"
#include "Cyrano/OutOfBoundsPolicy.hxx"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * a simple alias. This is the prototype of the external
 * behaviours used by the `Cyrano` fuel performance code.
 */
typedef void(TFEL_ADDCALL_PTR CyranoBehaviourPtr)(
    const CyranoIntegerType *const, /* nombre de composantes des contraintes  */
    const CyranoRealType *const, /* incrément de temps                     */
    const CyranoRealType *const, /* matrice de passage du repère élement
                                    au repère global                       */
    CyranoRealType *const,       /* matrice tangente                       */
    const CyranoRealType *const, /* tenseur des déformations               */
    const CyranoRealType *const, /* tenseur des incréments de déformations */
    const CyranoRealType *const, /* température au début du pas            */
    const CyranoRealType *const, /* incrément de température               */
    const CyranoRealType *const, /* propriétés matériau                    */
    const CyranoIntegerType *const, /* nombre de propriétés matériau          */
    const CyranoRealType *const,    /* variables externes */
    const CyranoRealType *const,    /* incréments des variables externes */
    CyranoRealType *const,          /* variables internes   */
    const CyranoIntegerType *const, /* nombre de variables internes */
    CyranoRealType *const,          /* tenseur des contraintes */
    const CyranoIntegerType *const, /* entier définissant le type de calcul  */
    CyranoIntegerType *const);      /* code sortie */

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_CYRANO_BEHAVIOUR_HXX */
