/*!
 * \file   src/NUMODIS/HCP.cxx
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

#include <cmath>
#include <algorithm>
#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/HCP.hxx"

namespace numodis {

  //===============================================================
  // HCP::HCP
  //---------------------------------------------------------------
  //! Constructor.
  //---------------------------------------------------------------
  /*!
    Defines the HCP Wigner-Seitz cell and its contravariant
    basis.  The length of the c axis is taken as
    \f$ \sqrt{8/3} \f$.
  */
  //===============================================================
  HCP::HCP() : _ratio(1.632993162) {
    //------------
    // class name
    //------------
    _name = "HCP";

    //---------------
    // atom basis
    //---------------
    _alattice.resize(4, Vect3());
    _alattice[0][0] = sqrt(3.) / 2.;
    _alattice[0][1] = 1 / 2.;
    _alattice[0][2] = 0.;

    _alattice[1][0] = -sqrt(3.) / 2.;
    _alattice[1][1] = 1 / 2.;
    _alattice[1][2] = 0.;

    _alattice[2][0] = 0.;
    _alattice[2][1] = -1.;
    _alattice[2][2] = 0.;

    _alattice[3][0] = 0.;
    _alattice[3][1] = 0.;
    _alattice[3][2] = _ratio;

    //---------------
    // Burgers basis
    //---------------
    _blattice.resize(4, Vect3());
    _blattice[0][0] = sqrt(3.) / 2.;
    _blattice[0][1] = 1 / 2.;
    _blattice[0][2] = 0.;

    _blattice[1][0] = -sqrt(3.) / 2.;
    _blattice[1][1] = 1 / 2.;
    _blattice[1][2] = 0.;

    _blattice[2][0] = 0.;
    _blattice[2][1] = -1.;
    _blattice[2][2] = 0.;

    _blattice[3][0] = 0.;
    _blattice[3][1] = 0.;
    _blattice[3][2] = _ratio;

    //-------------
    // plane basis
    //-------------
    _plattice.resize(4, Vect3());
    _plattice[0][0] = sqrt(3.) / 12.;
    _plattice[0][1] = 1. / 12.;
    _plattice[0][2] = 0.;

    _plattice[1][0] = -sqrt(3.) / 12;
    _plattice[1][1] = 1. / 12.;
    _plattice[1][2] = 0.;

    _plattice[2][0] = 0.;
    _plattice[2][1] = -1. / 6.;
    _plattice[2][2] = 0.;

    _plattice[3][0] = 0.;
    _plattice[3][1] = 0.;
    _plattice[3][2] = 1. / (4. * _ratio);
  }

  HCP::HCP(HCP&&) = default;

  HCP::HCP(const HCP&) = default;

  int HCP::getNsymmetries() const { return 96; }

  int HCP::getNindices() const { return 4; }

  int HCP::ScalProduct(const IPlane& iplane, const IBurgers& iburgers) const {
    return numodis::math::iScalProduct(iplane.getIndex(), iburgers.getIndex());
  }

  //===========================================================
  // HCP::ScalProduct
  //-----------------------------------------------------------
  //! Compute the scalar product of a IPlane * IDirection
  //-----------------------------------------------------------
  /*!

    This method returns 0 if a discrete direction is included in a plane

    \param iplane indices of the glide plane
    \param idirection indices of a discrete vector
    \return scalar product

  */
  //===========================================================
  int HCP::ScalProduct(const IPlane& iplane,
                       const IDirection& idirection) const {
    return numodis::math::iScalProduct(iplane.getIndex(),
                                       idirection.getIndex());
  }

  //===============================================================
  // HCP::Symmetry
  //---------------------------------------------------------------
  //! Compute symmetric indices
  //---------------------------------------------------------------
  /*!
    \param k rank of the symmetry
    \return symmetric indices
  */
  //===============================================================
  std::vector<int> HCP::Symmetry(int k, const std::vector<int>& indices) const {
    // permutation
    int p = k % 6;
    std::vector<int> permutation(3);
    for (int i = 0; i != 3; i++)
      permutation[i] = (p / 3 == 0 ? (i + p) % 3 : (p - i) % 3);

    // signes
    int s = (k % 48) / 6;

    std::vector<int> symmetry(4);
    symmetry[0] =
        (s / 4 == 0 ? indices[permutation[0]] : -indices[permutation[0]]);
    symmetry[1] =
        ((s / 2) % 2 == 0 ? indices[permutation[1]] : -indices[permutation[1]]);
    symmetry[2] =
        (s % 2 == 0 ? indices[permutation[2]] : -indices[permutation[2]]);
    symmetry[3] = (k / 48 == 0 ? indices[3] : -indices[3]);

    return symmetry;
  }

  //===============================================================
  // HCP::Symmetry
  //---------------------------------------------------------------
  //! Compute a symmetric glide system
  //---------------------------------------------------------------
  /*!
    \param k rank of the symmetry
    \return symmetric glide system
  */
  //===============================================================
  GSystem HCP::Symmetry(int k, const GSystem& gsystem) const {
    return GSystem(
        IBurgers(this->Symmetry(k, gsystem.getIBurgers().getIndex())),
        IPlane(this->Symmetry(k, gsystem.getIPlane().getIndex())));
  }

  //===========================================================
  // HCP::Norm2
  //-----------------------------------------------------------
  //! Return the squared magnitude of a Burgers vector
  //-----------------------------------------------------------
  /*!
    \param iburgers input IBurgers vector
    \return squared magnitude (unitless)
  */
  //===========================================================
  double HCP::Norm2(const IBurgers& iburgers) const {
    Vect3 xburgers;
    for (size_t i = 0; i != 4; i++) {
      xburgers += static_cast<double>(iburgers[i]) * _blattice[i];
    }
    return xburgers.SquareLength();
  }

  //===========================================================
  // HCP::CrossProduct
  //-----------------------------------------------------------
  //! Compute the intersection of two vectors
  //-----------------------------------------------------------
  /*!

    \param ivector0 index of the first vector
    \param ivector1 index of the second vector
    \param ivector2 index of the cross-product

  */
  //===========================================================
  void HCP::CrossProduct(const std::vector<int>& ivector0,
                         const std::vector<int>& ivector1,
                         std::vector<int>& ivector2) const {
    std::vector<int> index0(3);
    std::vector<int> index1(3);
    std::vector<int> idir3(3);

    //------------------------------------------------------
    // convert indices from 4 indices notation to 3 indices
    //------------------------------------------------------
    index0[0] = ivector0[0];
    index0[1] = ivector0[1];
    index0[2] = ivector0[3];

    index1[0] = ivector1[0];
    index1[1] = ivector1[1];
    index1[2] = ivector1[3];

    //------------------------------------------
    // compute the cross product with 3 indices
    //------------------------------------------
    numodis::math::iCrossProduct(index0, index1, idir3);

    //-----------------------------------------------------
    // converts to 4 indices notation
    //-----------------------------------------------------
    ivector2[0] = 2 * idir3[0] - idir3[1];
    ivector2[1] = 2 * idir3[1] - idir3[0];
    ivector2[2] = -(ivector2[0] + ivector2[1]);
    ivector2[3] = 3 * idir3[2];
  }

  //===========================================================
  // HCP::CrossProduct
  //-----------------------------------------------------------
  //! Compute the intersection of two planes
  //-----------------------------------------------------------
  /*!

    \param iplane0 first plane
    \param iplane1 second plane
    \param idirection common direction

  */
  //===========================================================
  void HCP::CrossProduct(const IPlane& iplane0,
                         const IPlane& iplane1,
                         IDirection& idirection) const {
    std::vector<int> id(4);

    this->CrossProduct(iplane0.getIndex(), iplane1.getIndex(), id);

    idirection.setIDirection(id);
  }

  //===========================================================
  // HCP::CrossProduct
  //-----------------------------------------------------------
  //! Compute the indices of a plane normal to a Burgers and a direction
  //-----------------------------------------------------------
  /*!

    \param iburgers a Burgers vector
    \param idirection a direction
    \param iplane the plane orthogonal to the Burgers and the direction

  */
  //===========================================================
  void HCP::CrossProduct(const IBurgers& iburgers,
                         const IDirection& idirection,
                         IPlane& iplane) const {
    std::vector<int> igp(4);

    this->CrossProduct(iburgers.getIndex(), idirection.getIndex(), igp);

    iplane.setIPlane(igp);
  }

  //=====================================================================
  // HCP::GenerateEquivalentIBurgers
  //---------------------------------------------------------------------
  //! Provide a list of IBurgers equivalent to a given IBurgers
  //---------------------------------------------------------------------
  /*!
    \param iburgers original IBurgers
    \param eqBurgers equivalent IBurgers
  */
  //=====================================================================
  void HCP::GenerateEquivalentIBurgers(const IBurgers& iburgers,
                                       std::vector<IBurgers>& eqBurgers) const {
    // copy and sort indices
    std::vector<int> indices(iburgers.getIndex());
    sort(indices.begin(), indices.begin() + 3);

    // consider all permutations
    do {
      // consider a possible plane
      IBurgers newBurgers(indices);

      // check if it is new
      bool isNew = true;
      for (size_t i = 0; i != eqBurgers.size(); i++)
        if (Coincide(newBurgers, eqBurgers[i]) != 0) {
          isNew = false;
          break;
        }

      // push if it is new
      if (isNew) eqBurgers.push_back(newBurgers);

      // opposite sign for c axis (if needed)
      if (newBurgers[3] != 0) {
        // consider another possible Burgers
        newBurgers[3] = -newBurgers[3];

        // check if it is new
        isNew = true;
        for (std::vector<IBurgers>::size_type i = 0; i != eqBurgers.size(); ++i)
          if (Coincide(newBurgers, eqBurgers[i]) != 0) {
            isNew = false;
            break;
          }

        // push if it is new
        if (isNew) eqBurgers.push_back(newBurgers);
      }

    }

    while (next_permutation(indices.begin(), indices.begin() + 3));
  }

  //=====================================================================
  // HCP::GenerateEquivalentPlanes
  //---------------------------------------------------------------------
  //! Provide a list of IPlanes equivalent to a given IPlane
  //---------------------------------------------------------------------
  /*!
    \param iplane original IPlane
    \param planes equivalent iplanes
  */
  //=====================================================================
  void HCP::GenerateEquivalentPlanes(const IPlane& iplane,
                                     std::vector<IPlane>& planes) const {
    // copy and sort indices
    std::vector<int> indices(iplane.getIndex());
    sort(indices.begin(), indices.begin() + 3);

    // consider all permutations
    do {
      // consider a possible plane
      IPlane newPlane(indices);

      // check if it is new
      bool isNew = true;
      for (size_t i = 0; i != planes.size(); i++)
        if (newPlane == planes[i]) {
          isNew = false;
          break;
        }

      // push if it is new
      if (isNew) planes.push_back(newPlane);

    } while (next_permutation(indices.begin(), indices.begin() + 3));
  }

  //=====================================================================
  // HCP::GenerateEquivalentIndices
  //---------------------------------------------------------------------
  //! Provide a list of equivalent indices
  //---------------------------------------------------------------------
  /*!
    \param ind0 original indices
    \param equivalent equivalent indices
  */
  //=====================================================================
  void HCP::GenerateEquivalentIndices(
      const std::vector<int>& ind0,
      std::vector<std::vector<int>>& equivalent) const {
    //----------------
    // initialization
    //----------------
    equivalent.clear();
    equivalent.reserve(8);

    //----------------------------
    // get the indices of ind0
    //----------------------------
    std::vector<int> indices(4);
    for (unsigned i = 0; i < 4; i++) indices[i] = ind0[i];

    //------------------------------
    // sort the first three indices
    //------------------------------
    sort(indices.begin(), indices.begin() + 3);

    //------------------------
    // number of null indices
    //------------------------
    int nzero = 0;
    for (size_t i = 0; i != 3; i++)
      if (indices[i] == 0) nzero++;

    //------------------------------------------------------
    // consider all the permutations of first three indices
    //------------------------------------------------------
    std::vector<int> permutation(4);
    do {
      // 1st possibility
      permutation[0] = indices[0];
      permutation[1] = indices[1];
      permutation[2] = indices[2];
      permutation[3] = indices[3];

      equivalent.push_back(permutation);

      // 2nd possibility (if necessary)
      if (permutation[3] != 0 && nzero != 3) {
        permutation[3] = -permutation[3];
        equivalent.push_back(permutation);
      }

    } while (next_permutation(indices.begin(), indices.begin() + 3));
  }

  //===============================================================
  // HCP::GenerateOrthogonalVector
  //---------------------------------------------------------------
  //! Find a vector v orthogonal to u
  //---------------------------------------------------------------
  /*!
    \param u a vector (input)
    \param v a vector orthonal to u (output)
  */
  //===============================================================
  void HCP::GenerateOrthogonalVector(const IPlane& u, IDirection& v) const {
    //---------------
    // intialization
    //---------------
    int norm2 = 0;
    std::vector<int> vrandom(4);

    //--------------------
    // while not found...
    //--------------------
    while (norm2 == 0) {
      // generate a random vector
      for (unsigned i = 0; i < 4; i++) vrandom[i] = rand();

      // compute a vector normal to plane and vrandom
      CrossProduct(IPlane(vrandom), u, v);

      // norm of this vector
      norm2 = v.getIndex()[0] * v.getIndex()[0] +
              v.getIndex()[1] * v.getIndex()[1] +
              v.getIndex()[2] * v.getIndex()[2] +
              v.getIndex()[3] * v.getIndex()[3];
    }
  }

  //===============================================================
  // HCP::SamePlaneFamily
  //---------------------------------------------------------------
  //! Return true if two planes belong to the same family
  //---------------------------------------------------------------
  /*!
    \param iplane0 first plane
    \param iplane1 second plane
    \return true if there is a rotation leading from iburgers0 to iburgers1
  */
  //===============================================================
  bool HCP::SamePlaneFamily(const IPlane& iplane0,
                            const IPlane& iplane1) const {
    // sign changes are allowed for hexagonal crystals
    std::vector<int> ipl0sorted(iplane0.getIndex());
    std::vector<int> ipl1sorted(iplane1.getIndex());
    for (unsigned i = 0; i != 4; i++) ipl0sorted[i] = abs(ipl0sorted[i]);
    for (unsigned i = 0; i != 4; i++) ipl1sorted[i] = abs(ipl1sorted[i]);

    // indices permatutions are allowed for cubic crystals for the first 3
    // indices
    numodis::math::iSortVector3FirstValue(ipl0sorted);
    numodis::math::iSortVector3FirstValue(ipl1sorted);

    // compare the glide planes
    return numodis::math::iCollinear(ipl0sorted, ipl1sorted);
  }

  //===============================================================
  // HCP::SameBurgersFamily
  //---------------------------------------------------------------
  //! Return true if two Burgers vector belong to the same family
  //---------------------------------------------------------------
  /*!
    \param iburgers0 first Burgers vector
    \param iburgers1 second Burgers vector
    \return true if there is a rotation leading from iburgers0 to iburgers1
  */
  //===============================================================
  bool HCP::SameBurgersFamily(const IBurgers& iburgers0,
                              const IBurgers& iburgers1) const {
    // sign changes are allowed for hexagonal crystals
    std::vector<int> ibur0sorted(iburgers0.getIndex());
    std::vector<int> ibur1sorted(iburgers1.getIndex());
    for (unsigned i = 0; i != 4; i++) ibur0sorted[i] = abs(ibur0sorted[i]);
    for (unsigned i = 0; i != 4; i++) ibur1sorted[i] = abs(ibur1sorted[i]);

    // indices permatutions are allowed for hexagonal crystals for the first 3
    // indices
    numodis::math::iSortVector3FirstValue(ibur0sorted);
    numodis::math::iSortVector3FirstValue(ibur1sorted);

    // compare the Burgers vectors
    for (unsigned i = 0; i != 4; i++)
      if (ibur0sorted[i] != ibur1sorted[i]) return false;

    return true;
  }

  //===============================================================
  // HCP::SameGlideSystem
  //---------------------------------------------------------------
  //! Determines a glide system belongs to a given familly
  //---------------------------------------------------------------
  /*!
    This routine determines whether a glide system defined as
    \f${iplane1}[iburgers1]\f$ belongs to a glide system family
    defined as \f${iburgers0}<iburgers0>\f$.

    This routine overloads the one defined in Crystallo. It
    takes advantage of the cubic symetry of the crystal:
    - signs can change
    - all the permutations are allowed for the three first indices

    \param iplane0 reference glide system
    \param iburgers0 reference burgers vector
    \param iplane1 a specific glide system
    \param iburgers1 a specific burgers vector
    \return true if \f$()[]\f$ belongs to \f${}<>\f$
  */
  //===============================================================
  bool HCP::SameGlideSystem(const IPlane& iplane0,
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

  HCP::~HCP() = default;

}  // end of namespace numodis
