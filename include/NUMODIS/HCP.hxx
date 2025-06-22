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

    virtual double Norm2(const IBurgers& iburgers) const override;

    virtual int getNsymmetries() const override;

    virtual GSystem Symmetry(int k, const GSystem& gsystem) const override;

    virtual void GenerateEquivalentIBurgers(
        const IBurgers& iburgers,
        std::vector<IBurgers>& eqiburgers) const override;

    virtual void GenerateEquivalentPlanes(
        const IPlane& iplane, std::vector<IPlane>& planes) const override;

    virtual void GenerateEquivalentIndices(
        const std::vector<int>& ind0,
        std::vector<std::vector<int>>& equivalent) const override;

    virtual void GenerateOrthogonalVector(const IPlane& u,
                                          IDirection& v) const override;

    virtual bool SamePlaneFamily(const IPlane& iplane0,
                                 const IPlane& iplane1) const override;

    virtual bool SameBurgersFamily(const IBurgers& iburgers0,
                                   const IBurgers& iburgers1) const override;

    virtual bool SameGlideSystem(const IPlane& iplane0,
                                 const IBurgers& iburgers0,
                                 const IPlane& iplane1,
                                 const IBurgers& iburgers1) const override;

    virtual void CrossProduct(const IPlane& iplane0,
                              const IPlane& iplane1,
                              IDirection& idirection) const override;

    virtual void CrossProduct(const IBurgers& iburgers,
                              const IDirection& idirection,
                              IPlane& iplane) const override;

    virtual ~HCP();

    //===========================================================
    // HCP::getNindices
    //-----------------------------------------------------------
    //! Returns the number of indices used for discrete coordinates
    //-----------------------------------------------------------
    /*! \return number of indices                              */
    //===========================================================
    virtual int getNindices() const override;

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
