/*!
 * \file   src/NUMODIS/Crystallo.cxx  
 * \brief    
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>

#include "NUMODIS/Math/Utilities.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/Crystallo.hxx"

namespace numodis
{

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
				 const IPlane& iplane) const
  {
    if (this->ScalProduct(iplane,iburgers)!=0){
      std::ostringstream os;
      os << "Crystallo::InitGSystem: ill-defined glide system"
	 << "'" << iburgers << iplane << "'";
      throw(std::runtime_error(os.str()));
    }
    return GSystem(iburgers,iplane);
  }

  Vect3 Crystallo::normal(const IPlane& iplane) const
  {
    // rotation
    Vect3 xdirection;
    for(unsigned i=0; i<this->_alattice.size(); i++)
      xdirection+=static_cast<double>(iplane[i])*(this->_plattice[i]);
    // convert to a unit vector
    xdirection.Normalize();
    return xdirection;
  }
  
  Vect3 Crystallo::burgers_vector(const IBurgers& iburgers) const
  {
    Vect3 xburgers;
    for(unsigned i=0; i<_blattice.size(); i++){
      xburgers+=(double)(iburgers.getIndex()[i])*_blattice[i];
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
					    const GSystem& gsystem2) const
  {    

    // same glide plane?
    if(gsystem1.getIPlane()==gsystem2.getIPlane())
      {
	    
	if(Coincide(gsystem1.getIBurgers(),gsystem2.getIBurgers())!=0)
	  {

	    // self junction
	    throw -1;
	  }
	else
	  {
	    // add Burgers vectors
	    IBurgers iburgers3a(gsystem1.getIBurgers()+gsystem2.getIBurgers());
	    IBurgers iburgers3b(gsystem1.getIBurgers()-gsystem2.getIBurgers());
		    
	    // consider the smallest one
	    IBurgers ijunctionBurgers( this->Norm2(iburgers3a)<this->Norm2(iburgers3b) ? iburgers3a : iburgers3b );

	    // return new glide system
	    return this->InitGSystem(ijunctionBurgers,gsystem1.getIPlane());		    
	  }	    	    
      }
    else
      {

	if(Coincide(gsystem1.getIBurgers(),gsystem2.getIBurgers())!=0)
	  {
		    
	    // colinear situation
	    throw -2;
	  }
	else
	  {
	    // compute junction direction
	    IDirection ijunction(this->getNindices());;
	    this->CrossProduct(gsystem1.getIPlane(),gsystem2.getIPlane(),ijunction);

	    // add Burgers vectors
	    IBurgers iburgers3a(gsystem1.getIBurgers()+gsystem2.getIBurgers());
	    IBurgers iburgers3b(gsystem1.getIBurgers()-gsystem2.getIBurgers());
		    
	    // consider the smallest one
	    IBurgers ijunctionBurgers( this->Norm2(iburgers3a)<this->Norm2(iburgers3b) ? iburgers3a : iburgers3b );
		    
	    // compute the glide plane
	    IPlane ijunctionPlane(this->getNindices());
	    this->CrossProduct(ijunctionBurgers,ijunction,ijunctionPlane);

	    // return new glide system
	    return this->InitGSystem(ijunctionBurgers,ijunctionPlane);
		    
	  }
      }
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
  void Crystallo::GenerateEquivalentGSystems(const GSystem& gsystem,
					     std::vector<GSystem>& allGSystems) const
  {

    // generate all planes
    std::vector<IPlane> allPlanes;
    this->GenerateEquivalentPlanes(gsystem.getIPlane(),allPlanes);

    // generate all the other Burgers vector
    std::vector<IBurgers> allBurgers;
    this->GenerateEquivalentIBurgers(gsystem.getIBurgers(),allBurgers);
    
    // generate all glide systems
    for(decltype(allPlanes.size()) i=0; i!=allPlanes.size(); i++)
      for(decltype(allBurgers.size()) j=0; j!=allBurgers.size(); j++)
	if(this->ScalProduct(allPlanes[i],allBurgers[j])==0)
	  allGSystems.push_back(GSystem(allBurgers[j],allPlanes[i]));
				      
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
				  const IBurgers& iburgers1) const
  {
    return (iplane1==iplane0 && Coincide(iburgers0,iburgers1)!=0);
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
  void Crystallo::GenerateEquivalentIBurgers(const IBurgers& iburgers,
					     std::vector<IBurgers>& eqiburgers) const
  {

    //----------------
    // initialization
    //----------------
    std::vector<int> indices(iburgers.getIndex());
    std::vector<std::vector<int> > equivalent;

    //-----------------------------
    // generate equivalent indices
    //-----------------------------
    this->GenerateEquivalentIndices(indices,equivalent);

    //---------------------
    // convert to iburgers
    //---------------------
    eqiburgers.clear();
    eqiburgers.reserve(equivalent.size());
    for(unsigned i=0; i<equivalent.size(); i++)
      eqiburgers.push_back(IBurgers(equivalent[i]));

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
					   std::vector<IPlane>& planes) const
  {

    //----------------
    // initialization
    //----------------
    std::vector<int> indices(iplane.getIndex());
    std::vector<std::vector<int> > equivalent;

    //-----------------------------
    // generate equivalent indices
    //-----------------------------
    this->GenerateEquivalentIndices(indices,equivalent);

    //-------------------
    // convert to planes
    //-------------------
    planes.clear();
    planes.reserve(equivalent.size());
    for(unsigned i=0; i<equivalent.size(); i++)
      planes.push_back(IPlane(equivalent[i]));

  }

  Crystallo::~Crystallo() = default;
  
} // end of namespace numodis
