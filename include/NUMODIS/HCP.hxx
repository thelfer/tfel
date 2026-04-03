/*!
 * \file   include/NUMODIS/HCP.hxx
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

#ifndef NUMEODIS_HCP_HXX
#define NUMEODIS_HCP_HXX

#include <iosfwd>
#include "NUMODIS/Config.hxx"
#include "NUMODIS/Crystallo.hxx"

namespace numodis {

  //===============================================================
  // Class HCP
  //---------------------------------------------------------------
  //! HCP crystallography handler using the Wigner-Seitz cell.
  //---------------------------------------------------------------
  /*!
    HCP handles hexagonal closed-packed structures
    using the Wigner-Seitz cell and the standard 4 indices
    notation.

    The unit vectors \f$\mathbf{a}_1\f$,\f$\mathbf{a}_2\f$,
    \f$\mathbf{a}_3\f$ and \f$\mathbf{a}_4\f$ are defined
    as follows:

    \image html HCP_4i.png "HCP_4i unit vectors"
    \image latex HCP_4i.eps "HCP_4i unit vectors"

    The discrete position \f$\mathbf{r}\f$ of a node is defined
    by a quadruplet of integer \f$(i,j,k,l)\f$ such that:
    \f[
    \mathbf{r} = i \mathbf{a}_1 + j \mathbf{a}_2
    + k \mathbf{a}_3 + l \mathbf{a}_4
    \f]

    \ingroup ClassCrystallo

  */
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT HCP : public Crystallo {
    HCP();

    HCP(HCP&&);

    HCP(const HCP&);

    double Norm2(const IBurgers&) const override;

    int getNsymmetries() const override;

    GSystem Symmetry(int, const GSystem&) const override;

    void GenerateEquivalentIBurgers(const IBurgers&,
                                    std::vector<IBurgers>&) const override;

    void GenerateEquivalentPlanes(const IPlane&,
                                  std::vector<IPlane>&) const override;

    void GenerateEquivalentIndices(
        const std::vector<int>&, std::vector<std::vector<int>>&) const override;

    void GenerateOrthogonalVector(const IPlane&, IDirection&) const override;

    bool SamePlaneFamily(const IPlane&, const IPlane&) const override;

    bool SameBurgersFamily(const IBurgers&, const IBurgers&) const override;

    bool SameGlideSystem(const IPlane&,
                         const IBurgers&,
                         const IPlane&,
                         const IBurgers&) const override;

    void CrossProduct(const IPlane&, const IPlane&, IDirection&) const override;

    void CrossProduct(const IBurgers&,
                      const IDirection&,
                      IPlane&) const override;

    ~HCP() override;

    //===========================================================
    // HCP::getNindices
    //-----------------------------------------------------------
    //! Returns the number of indices used for discrete coordinates
    //-----------------------------------------------------------
    /*! \return number of indices                              */
    //===========================================================
    int getNindices() const override;

    //===========================================================
    // HCP::ScalProduct
    //-----------------------------------------------------------
    //! Compute the scalar product of a IPlane * IBurgers
    //-----------------------------------------------------------
    /*!

      This method returns 0 if a Burgers vector is included in a plane

      \param iplane indices of the glide plane
      \param iburgers indices of the Burgers vector
      \return scalar product

    */
    //===========================================================
    int ScalProduct(const IPlane& iplane,
                    const IBurgers& iburgers) const override;

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
    int ScalProduct(const IPlane& iplane,
                    const IDirection& idirection) const override;

   private:
    void CrossProduct(const std::vector<int>& ivector0,
                      const std::vector<int>& ivector1,
                      std::vector<int>& ivector2) const;

    std::vector<int> Symmetry(int k, const std::vector<int>& indices) const;

    //! ratio c/a
    double _ratio;
  };

}  // end of namespace numodis

#endif
