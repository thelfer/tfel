/*!
 * \file  mfront/include/MFront/ThermodynamicForce.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_THERMODYNAMICFORCE_H_
#define LIB_MFRONT_THERMODYNAMICFORCE_H_

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * structure describes the thermodynamic force associated with a
   * driving variable.
   */
  struct MFRONT_VISIBILITY_EXPORT ThermodynamicForce {
    ThermodynamicForce() = default;
    ThermodynamicForce(ThermodynamicForce&&) = default;
    ThermodynamicForce(const ThermodynamicForce&) = default;
    ThermodynamicForce& operator=(ThermodynamicForce&&) = default;
    ThermodynamicForce& operator=(const ThermodynamicForce&) = default;
    ~ThermodynamicForce() noexcept;
    //! name of the thermodynamic force
    std::string name;
    //! type of the thermodynamic force
    std::string type;
  };  // end of struct ThermodynamicForce

}  // end of namespace mfront

#endif /* LIB_MFRONT_THERMODYNAMICFORCE_H_ */
