/*!
 * \file   src/NUMODIS/Cubic.cxx
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

#include <algorithm>

#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/Cubic.hxx"
#include "NUMODIS/IPlane.hxx"
#include "NUMODIS/IBurgers.hxx"
#include "NUMODIS/GSystem.hxx"

namespace numodis {

  Cubic::Cubic(Cubic&&) = default;

  Cubic::Cubic(const Cubic&) = default;

  //===============================================================
  // Cubic::Symmetry
  //---------------------------------------------------------------
  //! Compute symmetric indices
  //---------------------------------------------------------------
  /*!
    \param k rank of the symmetry
    \return symmetric indices
  */
  //===============================================================
  std::vector<int> Cubic::Symmetry(int k,
                                   const std::vector<int>& indices) const {
    // permutation
    int p = k % 6;
    std::vector<int> permutation(3);
    for (std::vector<int>::size_type i = 0; i != 3; i++)
      permutation[i] = (p / 3 == 0 ? (i + p) % 3 : (p - i) % 3);

    // signes
    int s = k / 6;

    std::vector<int> symmetry(3);
    symmetry[0] =
        (s / 4 == 0 ? indices[permutation[0]] : -indices[permutation[0]]);
    symmetry[1] =
        ((s / 2) % 2 == 0 ? indices[permutation[1]] : -indices[permutation[1]]);
    symmetry[2] =
        (s % 2 == 0 ? indices[permutation[2]] : -indices[permutation[2]]);

    return symmetry;
  }

  int Cubic::ScalProduct(const IPlane& iplane, const IBurgers& iburgers) const {
    return numodis::math::iScalProduct(iplane.getIndex(), iburgers.getIndex());
  }

  int Cubic::ScalProduct(const IPlane& iplane,
                         const IDirection& idirection) const {
    return numodis::math::iScalProduct(iplane.getIndex(),
                                       idirection.getIndex());
  }

  int Cubic::getNsymmetries() const { return 48; }

  //===============================================================
  // Cubic::Symmetry
  //---------------------------------------------------------------
  //! Compute a symmetric glide system
  //---------------------------------------------------------------
  /*!
    \param k rank of the symmetry
    \return symmetric glide system
  */
  //===============================================================
  GSystem Cubic::Symmetry(int k, const GSystem& gsystem) const {
    return GSystem(
        IBurgers(this->Symmetry(k, gsystem.getIBurgers().getIndex())),
        IPlane(this->Symmetry(k, gsystem.getIPlane().getIndex())));
  }

  //===============================================================
  // Cubic::Cubic
  //---------------------------------------------------------------
  //! Constructor.
  //---------------------------------------------------------------
  /*!

    Initialization of generic parameters for cubic structures.


  */
  //===============================================================
  Cubic::Cubic() {
    //------
    // name
    //------
    _name = "Cubic";

    Init();
  }

  //===============================================================
  // Cubic::Init
  //---------------------------------------------------------------
  //! Initialize the parameters required to handle cubic crystals
  //===============================================================
  void Cubic::Init() {
    //    _ra0=1.0/_a0;
    //----------------------
    // basis initialization
    //----------------------
    _alattice.resize(3, Vect3());
    _alattice[0][0] = 1.0;
    _alattice[1][1] = 1.0;
    _alattice[2][2] = 1.0;

    _blattice.resize(3, Vect3());
    _blattice[0][0] = 1.0;
    _blattice[1][1] = 1.0;
    _blattice[2][2] = 1.0;

    _plattice.resize(3, Vect3());
    _plattice[0][0] = 1.0;
    _plattice[1][1] = 1.0;
    _plattice[2][2] = 1.0;
  }

  Vect3 Cubic::direction(const IDirection& idirection) const {
    //-------------------------------------
    // conversion to Crystallo coordinates
    //-------------------------------------
    Vect3 xdirection;
    xdirection[0] = idirection.getIndex()[0] * _alattice[0][0];
    xdirection[1] = idirection.getIndex()[1] * _alattice[1][1];
    xdirection[2] = idirection.getIndex()[2] * _alattice[2][2];
    //--------------------------
    // convert to a unit vector
    //--------------------------
    xdirection.Normalize();
    return xdirection;
  }  // end of Cubic::direction

  Vect3 Cubic::burgers_vector(const IBurgers& iburgers) const {
    //-------------------------------------
    // conversion to Crystallo coordinates
    //-------------------------------------
    Vect3 xburgers;
    xburgers[0] = iburgers.getIndex()[0] * _blattice[0][0];
    xburgers[1] = iburgers.getIndex()[1] * _blattice[1][1];
    xburgers[2] = iburgers.getIndex()[2] * _blattice[2][2];
    return xburgers;
  }

  Vect3 Cubic::normal(const IPlane& iplane) const {
    //-------------------------------------
    // conversion to Crystallo coordinates
    //-------------------------------------
    Vect3 xdirection;
    xdirection[0] = iplane.getIndex()[0] * _plattice[0][0];
    xdirection[1] = iplane.getIndex()[1] * _plattice[1][1];
    xdirection[2] = iplane.getIndex()[2] * _plattice[2][2];
    //--------------------------
    // convert to a unit vector
    //--------------------------
    xdirection.Normalize();
    return xdirection;
  }

  //===========================================================
  // Cubic::Norm2
  //-----------------------------------------------------------
  //! Return the squared magnitude of a Burgers vector
  //-----------------------------------------------------------
  /*!
    \param iburgers input IBurgers vector
    \return squared magnitude (unitless)
  */
  //===========================================================
  double Cubic::Norm2(const IBurgers& iburgers) const {
    return iburgers[0] * iburgers[0] + iburgers[1] * iburgers[1] +
           iburgers[2] * iburgers[2];
  }

  //===========================================================
  // Cubic::CrossProduct
  //-----------------------------------------------------------
  //! Compute the common direction of two planes
  //-----------------------------------------------------------
  /*!
    \param iplane1 first plane
    \param iplane2 second plane
    \param idirection common direction
  */
  //===========================================================
  void Cubic::CrossProduct(const IPlane& iplane1,
                           const IPlane& iplane2,
                           IDirection& idirection) const {
    std::vector<int> d(3);
    numodis::math::iCrossProduct(iplane1.getIndex(), iplane2.getIndex(), d);
    idirection.setIDirection(d);
  }

  //===========================================================
  // Cubic::CrossProduct
  //-----------------------------------------------------------
  //! Compute the cross product of a Burgers and a direction
  //-----------------------------------------------------------
  /*!

    Compute iburgers x idirection

    This method returns 0 if iburgers and idirection are collinear.

    \param iburgers indices of the Burgers vector
    \param idirection indices of the direction
    \param iplane indices of the corresponding plane
  */
  //===========================================================
  void Cubic::CrossProduct(const IBurgers& iburgers,
                           const IDirection& idirection,
                           IPlane& iplane) const {
    std::vector<int> gplane(3);

    numodis::math::iCrossProduct(iburgers.getIndex(), idirection.getIndex(),
                                 gplane);

    iplane.setIPlane(gplane);
  }

  //===========================================================
  // Cubic::CrossProduct
  //-----------------------------------------------------------
  //! Compute the cross product of two directions
  //-----------------------------------------------------------
  /*!

    Compute idirection0 x idirection1

    This method returns 0 if the two directions are collinear.

    \param idirection0 first direction
    \param idirection1 second direction
    \param idirection2 idirection0 x idirection1

  */
  //===========================================================
  void Cubic::CrossProduct(const IDirection& idirection0,
                           const IDirection& idirection1,
                           IDirection& idirection2) const {
    std::vector<int> dir2(3);

    numodis::math::iCrossProduct(idirection0.getIndex(), idirection1.getIndex(),
                                 dir2);

    idirection2.setIDirection(dir2);
  }

  //===============================================================
  // Cubic::SamePlaneFamily
  //---------------------------------------------------------------
  //! Return true if two planes belong to the same family
  //---------------------------------------------------------------
  /*!
    \param iplane0 first plane
    \param iplane1 second plane
    \return true if there is a rotation leading from iburgers0 to iburgers1
  */
  //===============================================================
  bool Cubic::SamePlaneFamily(const IPlane& iplane0,
                              const IPlane& iplane1) const {
    // sign changes are allowed for cubic crystals
    std::vector<int> ipl0sorted(numodis::math::abs(iplane0.getIndex()));
    std::vector<int> ipl1sorted(numodis::math::abs(iplane1.getIndex()));

    // indices permatutions are allowed for cubic crystals
    numodis::math::iSortVector3FirstValue(ipl0sorted);
    numodis::math::iSortVector3FirstValue(ipl1sorted);

    // compare the glide planes
    return numodis::math::iCollinear(ipl0sorted, ipl1sorted);
  }

  //===============================================================
  // Cubic::SameBurgersFamily
  //---------------------------------------------------------------
  //! Return true if two Burgers vector belong to the same family
  //---------------------------------------------------------------
  /*!
    \param iburgers0 first Burgers vector
    \param iburgers1 second Burgers vector
    \return true if there is a rotation leading from iburgers0 to iburgers1
  */
  //===============================================================
  bool Cubic::SameBurgersFamily(const IBurgers& iburgers0,
                                const IBurgers& iburgers1) const {
    // sign changes are allowed for cubic crystals
    std::vector<int> ibur0sorted = numodis::math::abs(iburgers0.getIndex());
    std::vector<int> ibur1sorted = numodis::math::abs(iburgers1.getIndex());

    // indices permatutions are allowed for cubic crystals
    numodis::math::iSortVector3FirstValue(ibur0sorted);
    numodis::math::iSortVector3FirstValue(ibur1sorted);

    // compare the Burgers vectors
    for (unsigned i = 0; i != 3; i++)
      if (ibur0sorted[i] != ibur1sorted[i]) return false;

    return true;
  }

  //===============================================================
  // Cubic::SameGlideSystem
  //---------------------------------------------------------------
  //! Determines a glide system belongs to a given familly
  //---------------------------------------------------------------
  /*!
    This routine determines whether a glide system defined as
    \f${iplane1}[iburgers1]\f$ belongs to a glide system family
    defined as \f$\{iburgers0\}<iburgers0>\f$.

    This routine overloads the one defined in Crystallo. It
    takes advantage of the cubic symetry of the crystal:
    - signs can change
    - all the permutations are allowed

    \BUG A DEPLACER DANS GSYSTEM

    \param iplane0 reference glide system
    \param iburgers0 reference burgers vector
    \param iplane1 a specific glide system
    \param iburgers1 a specific burgers vector
    \return true if \f$()[]\f$ belongs to \f$\{\}<>\f$
  */
  //===============================================================
  bool Cubic::SameGlideSystem(const IPlane& iplane0,
                              const IBurgers& iburgers0,
                              const IPlane& iplane1,
                              const IBurgers& iburgers1) const {
    //-----------------------------------------------
    // 1st test: is iburgers1 contained in iplane1?
    //-----------------------------------------------
    if (numodis::math::iScalProduct(iplane1.getIndex(), iburgers1.getIndex()) !=
        0)
      return false;

    //-----------------------------
    // 2nd test: same glide plane?
    //-----------------------------
    if (!this->SamePlaneFamily(iplane0, iplane1)) return false;

    //-------------------------------
    // 3rd test: same Burgers vector
    //-------------------------------
    return this->SameBurgersFamily(iburgers0, iburgers1);
  }

  //=====================================================================
  // Cubic::GenerateEquivalentIndices
  //---------------------------------------------------------------------
  //! Provide a list of equivalent indices
  //---------------------------------------------------------------------
  /*!

    Note: we assume ind0 has 3 terms for cubic crystallography

    Note: we do not consider -ind0 as a possibility

    \param ind0 indices of reference
    \param planes equivalent indices

  */
  //=====================================================================
  void Cubic::GenerateEquivalentIndices(
      const std::vector<int>& ind0,
      std::vector<std::vector<int>>& equivalent) const {
    //----------------
    // initialization
    //----------------
    equivalent.clear();
    equivalent.reserve(24);

    //--------------------------------
    // absolute values of the indices
    //--------------------------------
    std::vector<int> indices(3);
    for (unsigned i = 0; i < 3; i++) indices[i] = abs(ind0[i]);

    //--------------------
    // sort these indices
    //--------------------
    sort(indices.begin(), indices.end());

    //-------------------------------
    // consider all the permutations
    //-------------------------------
    std::vector<int> permutation(3);
    std::vector<unsigned> nonzero(3);
    do {
      // count (and store the indices of) the non-nuls terms
      unsigned count = 0;
      for (unsigned i = 0; i < 3; i++)
        if (indices[i] != 0) nonzero[count++] = i;

      // 1st possibility
      for (unsigned i = 0; i < 3; i++) permutation[i] = indices[i];
      equivalent.push_back(permutation);

      // add other possibilities if necessary
      switch (count) {
        case 2:  // 2 indices are different from 0 => 2 possibilities must be
                 // considered

          // 2nd plane
          permutation[nonzero[1]] = -permutation[nonzero[1]];
          equivalent.push_back(permutation);

          break;

        case 3:  // 3 indices are different from 0 => 4 possibilities must be
                 // considered

          // 2nd plane
          permutation[2] = -permutation[2];
          equivalent.push_back(permutation);

          // 3rd plane
          permutation[1] = -permutation[1];
          equivalent.push_back(permutation);

          // 4th plane
          permutation[2] = -permutation[2];
          equivalent.push_back(permutation);

          break;
      }

    } while (next_permutation(indices.begin(), indices.end()));
  }

  //===============================================================
  // Cubic::GenerateOrthogonalVector
  //---------------------------------------------------------------
  //! Find a vector v orthogonal to u
  //---------------------------------------------------------------
  /*!
    \param u a vector (input)
    \param v a vector orthonal to u (output)
  */
  //===============================================================
  void Cubic::GenerateOrthogonalVector(const IPlane& u, IDirection& v) const {
    //---------------
    // intialization
    //---------------
    int norm2 = 0;
    std::vector<int> vrandom(3);

    //--------------------
    // while not found...
    //--------------------
    while (norm2 == 0) {
      // generate a random vector
      for (unsigned i = 0; i < 3; i++) vrandom[i] = rand();

      // compute a vector normal to plane and vrandom
      CrossProduct(IPlane(vrandom), u, v);

      // norm of this vector
      norm2 = v.getIndex()[0] * v.getIndex()[0] +
              v.getIndex()[1] * v.getIndex()[1] +
              v.getIndex()[2] * v.getIndex()[2];
    }
  }

  Cubic::~Cubic() = default;

}  // end of namespace numodis
