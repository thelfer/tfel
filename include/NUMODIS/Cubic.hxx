/*!
 * \file   include/NUMODIS/Cubic.hxx
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

#ifndef NUMEODIS_CUBIC_HXX
#define NUMEODIS_CUBIC_HXX

#include <iosfwd>

#include "NUMODIS/Config.hxx"
#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/Crystallo.hxx"

namespace numodis {

  //===============================================================
  // Class Cubic
  //---------------------------------------------------------------
  //! Cubic handles crystallographic operations for cubic crystals
  //---------------------------------------------------------------
  /*!

    Cubic provides generic tools common to all cubic structures
    such as FCC or BCC structures. In particular, Cubic takes
    advantage of the symmetry and the orthogonality of the
    cubic lattice to perform its calculations.

    \ingroup ClassCrystallo
  */
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT Cubic : public Crystallo {
    Cubic();

    Cubic(Cubic&&);

    Cubic(const Cubic&);

    virtual Vect3 direction(const IDirection&) const override;

    virtual Vect3 burgers_vector(const IBurgers&) const override;

    virtual Vect3 normal(const IPlane&) const override;

    virtual double Norm2(const IBurgers& iburgers) const override;

    virtual int getNsymmetries() const override;

    virtual GSystem Symmetry(int k, const GSystem& gsystem) const override;

    virtual void GenerateEquivalentIndices(
        const std::vector<int>& ind0,
        std::vector<std::vector<int>>& equivalent) const override;

    virtual void GenerateOrthogonalVector(const IPlane& u,
                                          IDirection& v) const override;

    virtual void CrossProduct(const IPlane& iplane1,
                              const IPlane& iplane2,
                              IDirection& idirection) const override;

    virtual void CrossProduct(const IBurgers& iburgers,
                              const IDirection& idirection,
                              IPlane& iplane) const override;

    virtual void CrossProduct(const IDirection& idirection0,
                              const IDirection& idirection1,
                              IDirection& idirection2) const;

    //===========================================================
    // Cubic::ScalProduct
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
    virtual int ScalProduct(const IPlane& iplane,
                            const IBurgers& iburgers) const override;

    //===========================================================
    // Cubic::ScalProduct
    //-----------------------------------------------------------
    //! Compute the scalar product of a IPlane * IDirection
    //-----------------------------------------------------------
    /*!

      This method returns 0 if a discrete direction is included in a plane

      \param iplane indices of the glide plane
      \param idirection indices of a discrete direction
      \return scalar product

    */
    //===========================================================
    virtual int ScalProduct(const IPlane& iplane,
                            const IDirection& idirection) const override;

    //===========================================================
    // Cubic::ScalProduct
    //-----------------------------------------------------------
    //! Compute the scalar product of two IDirections
    //-----------------------------------------------------------
    /*!

      This method returns 0 if these two directions are orthogonal

      \param idirection0 first direction
      \param idirection1 second direction
      \return scalar product

    */
    //===========================================================
    int ScalProduct(const IDirection& idirection0,
                    const IDirection& idirection1) const {
      return numodis::math::iScalProduct(idirection0.getIndex(),
                                         idirection1.getIndex());
    }

    virtual bool SamePlaneFamily(const IPlane& iplane0,
                                 const IPlane& iplane1) const override;

    virtual bool SameBurgersFamily(const IBurgers& iburgers0,
                                   const IBurgers& iburgers1) const override;

    virtual bool SameGlideSystem(const IPlane& iplane0,
                                 const IBurgers& iburgers0,
                                 const IPlane& iplane1,
                                 const IBurgers& iburgers1) const override;

    virtual ~Cubic() override;

   protected:
    virtual void Init();

    std::vector<int> Symmetry(int k, const std::vector<int>& indices) const;
  };

}  // end of namespace numodis

#endif
