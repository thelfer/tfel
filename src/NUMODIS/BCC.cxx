/*!
 * \file   src/NUMODIS/BCC.cxx
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

#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/BCC.hxx"

namespace numodis {

  //===============================================================
  // BCC::BCC
  //---------------------------------------------------------------
  //! Constructor.
  //---------------------------------------------------------------
  /*!
    Defines the Wigner-Seitz cell of the BCC structure and
    its contravariant basis.

    \param a lattice parameter (Angstrom)
  */
  //===============================================================
  BCC::BCC() {
    //------------
    // class name
    //------------
    _name = "BCC";
    //------------------------
    // lattice initialization
    //------------------------
    this->Init();
  }

  //===============================================================
  // BCC::Init
  //---------------------------------------------------------------
  //! set the lattice vectors from the lattice parameters
  //===============================================================
  void BCC::Init() {}

  BCC::~BCC() = default;

}  // end of namespace numodis
