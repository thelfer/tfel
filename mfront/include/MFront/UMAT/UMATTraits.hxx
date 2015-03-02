/*!
 * \file   mfront/include/MFront/UMAT/UMATTraits.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 fév 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_UMATTRAITS_HXX_
#define LIB_MFRONT_UMATTRAITS_HXX_ 

namespace umat{

  enum UMATBehaviourType{
    SMALLSTRAINSTANDARDBEHAVIOUR,
    FINITESTRAINSTANDARDBEHAVIOUR,
    COHESIVEZONEMODEL
  }; // end of enum UMATBehaviourType

  enum UMATSymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum UMATSymmetryType

  template<typename Behaviour>
  struct UMATTraits{
    static constexpr UMATBehaviourType btype = umat::SMALLSTRAINSTANDARDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short DrivingVariableSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static constexpr unsigned short ThermodynamicForceVariableSize = 0u;
    static constexpr bool useTimeSubStepping = false;
    static constexpr bool doSubSteppingOnInvalidResults  = false;
    static constexpr unsigned short maximumSubStepping   = 0u;
    static constexpr bool requiresStiffnessTensor        = false;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short propertiesOffset       = 0u;
    static constexpr unsigned short material_properties_nb = 0u;
    static constexpr unsigned short internal_variables_nb  = 0u;
    static constexpr UMATSymmetryType stype = umat::ISOTROPIC;
  }; // end of class UMATTraits

} // end of namespace umat  

#endif /* LIB_MFRONT_UMATTRAITS_HXX_ */

