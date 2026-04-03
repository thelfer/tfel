/*!
 * \file   src/NUMODIS/Crystallo.cxx
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

#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>

#include "TFEL/Raise.hxx"
#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/Crystallo.hxx"

namespace numodis {

  Crystallo::Crystallo() = default;

  Crystallo::Crystallo(Crystallo&&) = default;

  Crystallo::Crystallo(const Crystallo&) = default;

  //===============================================================
  // Crystallo::InitGSystem
  //---------------------------------------------------------------
  //! Construct a crystallographic verified glide system
  //---------------------------------------------------------------
  /*!
    \param iburgers a Burgers vector
    \param iplane a glide plane
    \return a glide system
  */
  //===============================================================
  GSystem Crystallo::InitGSystem(const IBurgers& iburgers,
                                 const IPlane& iplane) const {
    if (this->ScalProduct(iplane, iburgers) != 0) {
      std::ostringstream os;
      os << "Crystallo::InitGSystem: ill-defined glide system"
         << "'" << iburgers << iplane << "'";
      tfel::raise(os.str());
    }
    return GSystem(iburgers, iplane);
  }

  Vect3 Crystallo::direction(const IDirection& idirection) const {
    // rotation
    Vect3 xdirection;
    for (unsigned i = 0; i < _alattice.size(); i++) {
      xdirection +=
          static_cast<double>(idirection.getIndex()[i]) * _alattice[i];
    }
    // convert to a unit vector
    xdirection.Normalize();
    return xdirection;
  }

  Vect3 Crystallo::normal(const IPlane& iplane) const {
    // rotation
    Vect3 xdirection;
    for (unsigned i = 0; i < this->_alattice.size(); i++)
      xdirection += static_cast<double>(iplane[i]) * (this->_plattice[i]);
    // convert to a unit vector
    xdirection.Normalize();
    return xdirection;
  }

  Vect3 Crystallo::burgers_vector(const IBurgers& iburgers) const {
    Vect3 xburgers;
    for (unsigned i = 0; i < _blattice.size(); i++) {
      xburgers += static_cast<double>(iburgers.getIndex()[i]) * _blattice[i];
    }
    return xburgers;
  }

  //=====================================================================
  // Crystallo::ComputeJunctionGSystem
  //---------------------------------------------------------------------
  //! Compute the glide system of a junction between two glide systems
  //---------------------------------------------------------------------
  /*!
    \param gsystem1 first glide system
    \param gsystem2 second glide system
    \return output glide system
  */
  //=====================================================================
  GSystem Crystallo::ComputeJunctionGSystem(const GSystem& gsystem1,
                                            const GSystem& gsystem2) const {
    // same glide plane?
    if (gsystem1.getIPlane() == gsystem2.getIPlane()) {
      if (Coincide(gsystem1.getIBurgers(), gsystem2.getIBurgers()) != 0) {
        // self junction
        throw -1;
      }
      // add Burgers vectors
      IBurgers iburgers3a(gsystem1.getIBurgers() + gsystem2.getIBurgers());
      IBurgers iburgers3b(gsystem1.getIBurgers() - gsystem2.getIBurgers());

      // consider the smallest one
      IBurgers ijunctionBurgers(
          this->Norm2(iburgers3a) < this->Norm2(iburgers3b) ? iburgers3a
                                                            : iburgers3b);

      // return new glide system
      return this->InitGSystem(ijunctionBurgers, gsystem1.getIPlane());
    }
    if (Coincide(gsystem1.getIBurgers(), gsystem2.getIBurgers()) != 0) {
      // colinear situation
      throw -2;
    }
    // compute junction direction
    IDirection ijunction(this->getNindices());
    this->CrossProduct(gsystem1.getIPlane(), gsystem2.getIPlane(), ijunction);

    // add Burgers vectors
    IBurgers iburgers3a(gsystem1.getIBurgers() + gsystem2.getIBurgers());
    IBurgers iburgers3b(gsystem1.getIBurgers() - gsystem2.getIBurgers());

    // consider the smallest
    Vect3 xjunction = this->direction(ijunction);
    Vect3 xburgers3a = this->burgers_vector(iburgers3a);
    Vect3 xburgers3b = this->burgers_vector(iburgers3b);
    double E3a = xburgers3a.SquareLength() *
                 (1 - 0.3 * pow(xjunction.Dot(xburgers3a.UnitVector()), 2));
    double E3b = xburgers3b.SquareLength() *
                 (1 - 0.3 * pow(xjunction.Dot(xburgers3b.UnitVector()), 2));

    IBurgers ijunctionBurgers(E3a < E3b ? iburgers3a : iburgers3b);

    // compute the glide plane
    IPlane ijunctionPlane(this->getNindices());
    this->CrossProduct(ijunctionBurgers, ijunction, ijunctionPlane);

    // return new glide system
    return this->InitGSystem(ijunctionBurgers, ijunctionPlane);
  }

  //=====================================================================
  // Crystallo::GenerateEquivalentGSystems
  //---------------------------------------------------------------------
  //! Provide a vector of all equivalent glide systems
  //---------------------------------------------------------------------
  /*!
    \param gsystem reference glide system
    \param allGSystems all equivalet glide systems
  */
  //=====================================================================
  void Crystallo::GenerateEquivalentGSystems(
      const GSystem& gsystem, std::vector<GSystem>& allGSystems) const {
    // generate all planes
    std::vector<IPlane> allPlanes;
    this->GenerateEquivalentPlanes(gsystem.getIPlane(), allPlanes);

    // generate all the other Burgers vector
    std::vector<IBurgers> allBurgers;
    this->GenerateEquivalentIBurgers(gsystem.getIBurgers(), allBurgers);

    // generate all glide systems
    for (const auto& p : allPlanes) {
      for (const auto& b : allBurgers) {
        if (this->ScalProduct(p, b) == 0) {
          allGSystems.push_back(GSystem(b, p));
        }
      }
    }
  }

  //===============================================================
  // Crystallo::SameGlideSystem
  //---------------------------------------------------------------
  //! Determines a glide system belongs to a given familly
  //---------------------------------------------------------------
  /*!
    This routine determines whether a glide system defined as
    \f${iplane1}[iburgers1]\f$ belongs to a glide system family
    defined as \f${iburgers0}<iburgers0>\f$.

    This routine is virtual. By default, this routine does not
    include any symetry.

    \param iplane0 reference glide system
    \param iburgers0 reference burgers vector
    \param iplane1 a specific glide system
    \param iburgers1 a specific burgers vector
    \return true if \f$()[]\f$ belongs to \f${}<>\f$
  */
  //===============================================================
  bool Crystallo::SameGlideSystem(const IPlane& iplane0,
                                  const IBurgers& iburgers0,
                                  const IPlane& iplane1,
                                  const IBurgers& iburgers1) const {
    return (iplane1 == iplane0 && Coincide(iburgers0, iburgers1) != 0);
  }

  //=====================================================================
  // Crystallo::GenerateEquivalentIBurgers
  //---------------------------------------------------------------------
  //! Provide a list of IBurgers equivalent to a given IBurgers
  //---------------------------------------------------------------------
  /*!
    \param iburgers original IBurgers
    \param planes equivalent IBurgers
  */
  //=====================================================================
  void Crystallo::GenerateEquivalentIBurgers(
      const IBurgers& iburgers, std::vector<IBurgers>& eqiburgers) const {
    //----------------
    // initialization
    //----------------
    std::vector<int> indices(iburgers.getIndex());
    std::vector<std::vector<int>> equivalent;

    //-----------------------------
    // generate equivalent indices
    //-----------------------------
    this->GenerateEquivalentIndices(indices, equivalent);

    //---------------------
    // convert to iburgers
    //---------------------
    eqiburgers.clear();
    eqiburgers.reserve(equivalent.size());
    for (const auto& e : equivalent) {
      eqiburgers.push_back(IBurgers(e));
    }
  }

  //=====================================================================
  // Crystallo::GenerateEquivalentPlanes
  //---------------------------------------------------------------------
  //! Provide a list of IPlanes equivalent to a given IPlane
  //---------------------------------------------------------------------
  /*!
    \param iplane original IPlane
    \param planes equivalent iplanes
  */
  //=====================================================================
  void Crystallo::GenerateEquivalentPlanes(const IPlane& iplane,
                                           std::vector<IPlane>& planes) const {
    //----------------
    // initialization
    //----------------
    std::vector<int> indices(iplane.getIndex());
    std::vector<std::vector<int>> equivalent;

    //-----------------------------
    // generate equivalent indices
    //-----------------------------
    this->GenerateEquivalentIndices(indices, equivalent);

    //-------------------
    // convert to planes
    //-------------------
    planes.clear();
    planes.reserve(equivalent.size());
    for (const auto& p : equivalent) {
      planes.push_back(IPlane(p));
    }
  }

  Crystallo::~Crystallo() = default;

}  // end of namespace numodis
