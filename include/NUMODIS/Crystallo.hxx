/*!
 * \file   include/NUMODIS/Crystallo.hxx
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

#ifndef LIB_NUMEODIS_CRYSTALLO_HXX
#define LIB_NUMEODIS_CRYSTALLO_HXX

#include <iosfwd>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "NUMODIS/Config.hxx"
#include "NUMODIS/IBurgers.hxx"
#include "NUMODIS/IPlane.hxx"
#include "NUMODIS/IDirection.hxx"

namespace numodis {

  struct GSystem;

  //===============================================================
  // Class Crystallo
  //---------------------------------------------------------------
  //! Generic crystallography handler.
  //---------------------------------------------------------------
  /*!
    Crystallo handles all the crystallographic operations and
    data. Specific handlers dedicated to particular crystallographic
    structures may be found in the sub-directory "crystallo". They
    can be used polymorphically as Crystallo objects.
  */
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT Crystallo {
    Crystallo();

    Crystallo(Crystallo&&);

    Crystallo(const Crystallo&);

    GSystem InitGSystem(const IBurgers& iburgers, const IPlane& iplane) const;

    virtual Vect3 direction(const IDirection&) const;

    virtual Vect3 normal(const IPlane&) const;

    virtual Vect3 burgers_vector(const IBurgers&) const;

    virtual double Norm2(const IBurgers& iburgers) const = 0;

    virtual GSystem Symmetry(int k, const GSystem& gsystem) const = 0;

    virtual int getNsymmetries() const = 0;

    virtual bool SameGlideSystem(const IPlane& iplane0,
                                 const IBurgers& iburgers0,
                                 const IPlane& iplane1,
                                 const IBurgers& iburgers1) const;

    virtual void GenerateEquivalentIBurgers(
        const IBurgers& iburgers, std::vector<IBurgers>& eqiburgers) const;

    virtual void GenerateEquivalentPlanes(const IPlane& iplane,
                                          std::vector<IPlane>& planes) const;

    virtual void GenerateEquivalentGSystems(
        const GSystem& gsystem, std::vector<GSystem>& allGSystems) const;

    virtual void GenerateEquivalentIndices(
        const std::vector<int>& ind0,
        std::vector<std::vector<int>>& equivalent) const = 0;

    virtual GSystem ComputeJunctionGSystem(const GSystem& gsystem1,
                                           const GSystem& gsystem2) const;

    //===========================================================
    // Crystallo::PrintName
    //-----------------------------------------------------------
    //! Returns the name of the Crystallo handler
    //-----------------------------------------------------------
    /*! \return the %Crystallo handler                         */
    //===========================================================
    const std::string& PrintName() const { return _name; }

    //===========================================================
    // Crystallo::getNindices
    //-----------------------------------------------------------
    //! Returns the number of indices used for discrete coordinates
    //-----------------------------------------------------------
    /*! \return number of indices (default=3)                  */
    //===========================================================
    virtual int getNindices() const { return 3; }

    //===========================================================
    // Crystallo::Collinear
    //-----------------------------------------------------------
    //! Determine whether two directions are collinear or not
    //-----------------------------------------------------------
    /*!

      \param idirection1 first direction
      \param idirection2 second direction
      \return true if collinear, false otherwise

    */
    //===========================================================
    bool Collinear(const IDirection& idirection1,
                   const IDirection& idirection2) const {
      return numodis::math::iCollinear(idirection1.getIndex(),
                                       idirection2.getIndex());
    }

    //===========================================================
    // Crystallo::CrossProduct
    //-----------------------------------------------------------
    //! Compute the cross product of two plane normals
    //-----------------------------------------------------------
    /*!

      Compute iplane0 x iplane1

      This method returns 0 if iplane0 and iplane1 are collinear.

      \param iplane0 index of the first plane
      \param iplane1 index of the second plane
      \param idirection common direction

    */
    //===========================================================
    virtual void CrossProduct(const IPlane& iplane0,
                              const IPlane& iplane1,
                              IDirection& idirection) const = 0;

    //===========================================================
    // Crystallo::CrossProduct
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
    virtual void CrossProduct(const IBurgers& iburgers,
                              const IDirection& idirection,
                              IPlane& iplane) const = 0;

    //===========================================================
    // Crystallo::ScalProduct
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
                            const IBurgers& iburgers) const = 0;

    //===========================================================
    // Crystallo::ScalProduct
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
                            const IDirection& idirection) const = 0;

    //===========================================================
    // Crystallo::GenerateOrthogonalVector
    //-----------------------------------------------------------
    //! Find a vector v orthogonal to u
    //-----------------------------------------------------------
    /*!
      \param u a vector (input)
      \param v a vector orthonal to u (output)
    */
    //===========================================================
    virtual void GenerateOrthogonalVector(const IPlane& u,
                                          IDirection& v) const = 0;

    //===========================================================
    // Crystallo::SamePlaneFamily
    //-----------------------------------------------------------
    //! Return true if two planes are equivalent
    //-----------------------------------------------------------
    /*!

      Example for cubic crystals:
      (110) is equivalent to (-101)

      \param iplane0 first plane
      \param iplane1 second plane

    */
    //===========================================================
    virtual bool SamePlaneFamily(const IPlane& iplane0,
                                 const IPlane& iplane1) const = 0;

    //===========================================================
    // Crystallo::SameBurgersFamily
    //-----------------------------------------------------------
    //! Return true if two Burgers are equivalent
    //-----------------------------------------------------------
    /*!

      Example for cubic crystals:
      [110] is equivalent to [-101]

      \param iburgers0 first Burgers vector
      \param iburgers1 second Burgers vector

    */
    //===========================================================
    virtual bool SameBurgersFamily(const IBurgers& iburgers0,
                                   const IBurgers& iburgers1) const = 0;

    const std::string getName() const { return _name; }

    virtual ~Crystallo();

   protected:
    //! \brief name of the class
    std::string _name;
    //! \brief basis used to compute atomic positions and directions
    std::vector<Vect3> _alattice;
    //! \brief basis used to compute Burgers' vector
    std::vector<Vect3> _blattice;
    //! \brief basis used to compute plane normal
    std::vector<Vect3> _plattice;
  };

}  // end of namespace numodis

#endif /* LIB_NUMEODIS_CRYSTALLO_HXX */
