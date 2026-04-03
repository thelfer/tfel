/*!
 * \file   src/NUMODIS/FCC.cxx
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
#include "NUMODIS/FCC.hxx"

namespace numodis {

  //===============================================================
  // FCC::FCC
  //---------------------------------------------------------------
  //! Constructor.
  //===============================================================
  FCC::FCC() {
    //------
    // name
    //------
    _name = "FCC";

    //------------------------
    // lattice initialization
    //------------------------
    this->Init();
  }

  FCC::FCC(FCC&&) = default;

  FCC::FCC(const FCC&) = default;

  //===============================================================
  // FCC::Init
  //---------------------------------------------------------------
  //! set the lattice vectors from the lattice parameters
  //===============================================================
  void FCC::Init() {
    // //--------------
    // // atomic basis
    // //--------------
    // _alattice[0][0]= _a0/2.;
    // _alattice[0][1]= 0.;
    // _alattice[0][2]= 0.;

    // _alattice[1][0]= 0.;
    // _alattice[1][1]= _a0/2.;
    // _alattice[1][2]= 0.;

    // _alattice[2][0]= 0.;
    // _alattice[2][1]= 0.;
    // _alattice[2][2]= _a0/2.;

    // //---------------
    // // Burgers basis
    // //---------------

    // // Note: a0/6 has been chosen to handle 1/6<211> partial
    // //       dislocations in this FCC structure

    // _blattice[0][0]= _a0/6.;
    // _blattice[0][1]= 0.;
    // _blattice[0][2]= 0.;

    // _blattice[1][0]= 0.;
    // _blattice[1][1]= _a0/6.;
    // _blattice[1][2]= 0.;

    // _blattice[2][0]= 0.;
    // _blattice[2][1]= 0.;
    // _blattice[2][2]= _a0/6.;

    // //-------------
    // // plane basis
    // //-------------
    // _plattice[0][0]= _a0/2.;
    // _plattice[0][1]= 0.;
    // _plattice[0][2]= 0.;

    // _plattice[1][0]= 0.;
    // _plattice[1][1]= _a0/2.;
    // _plattice[1][2]= 0.;

    // _plattice[2][0]= 0.;
    // _plattice[2][1]= 0.;
    // _plattice[2][2]= _a0/2.;
  }

}  // end of namespace numodis
