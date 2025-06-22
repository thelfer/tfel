/*!
 * \file   include/TFEL/Metaprogramming/InvalidType.hxx
 * \brief  This file declares InvalidType
 * \author Helfer Thomas
 * \date   28 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_INVALIDTYPE_H_
#define LIB_TFEL_INVALIDTYPE_H_

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace meta {

    /*!
     * \brief InvalidType is a type that is declared but not defined.
     * InvalidType is declared but not defined.
     * It is safe to make a typedef to InvalidType but it cannot be
     * instanciated. This type is generally used to give default and
     * invalid values to fields in traits class.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    struct InvalidType;

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_INVALIDTYPE_H_ */
