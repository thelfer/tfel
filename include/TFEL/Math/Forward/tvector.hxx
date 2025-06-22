/*!
 * \file  include/TFEL/Math/Forward/tvector.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_TVECTOR_H_
#define LIB_TFEL_MATH_FORWARD_TVECTOR_H_

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    /*
     * a base for tvector or classes acting like tvector.
     */
    template <typename Child, unsigned short N, typename T>
    struct tvector_base;

    /*
     * a class representing tiny vectors of fixed size.
     */
    template <unsigned short N, typename T>
    struct tvector;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_TVECTOR_H_ */
