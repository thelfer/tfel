/*!
 * \file   src/NUMODIS/GSystem.cxx
 * \brief
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "NUMODIS/Crystallo.hxx"
#include "NUMODIS/GSystem.hxx"

namespace numodis {

  //===============================================================
  // GSystem::~GSystem
  //---------------------------------------------------------------
  //! Destructor
  //===============================================================
  GSystem::~GSystem() = default;

  //===============================================================
  // GSystem :: Operator ==
  //---------------------------------------------------------------
  //! Determines whether two GSystems are "identical" or not
  //---------------------------------------------------------------
  /*!
    This routine compares the Burgers vector and the glide plane

    \return true or false
  */
  //===============================================================
  bool operator==(const GSystem& gsystem1, const GSystem& gsystem2) {
    //--------------------------
    // compare the glide planes
    //--------------------------
    if (gsystem1.getIPlane() != gsystem2.getIPlane()) return false;

    //-----------------------------
    // compare the Burgers vectors
    //-----------------------------
    return Coincide(gsystem1.getIBurgers(), gsystem2.getIBurgers()) != 0;
  }

  //===============================================================
  // GSystem: operator<<
  //---------------------------------------------------------------
  //! Print the content of a GSystem
  //---------------------------------------------------------------
  /*!
    \param os ouput stream
    \param gsystem GSystem to be printed in os
  */
  //===============================================================
  std::ostream& operator<<(std::ostream& os, const GSystem& gsystem) {
    os << gsystem.getIPlane() << " " << gsystem.getIBurgers();

    return os;
  }

}  // namespace numodis
