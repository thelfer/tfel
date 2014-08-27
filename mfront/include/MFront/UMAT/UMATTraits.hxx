/*!
 * \file   UMATTraits.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 fév 2007
 */

#ifndef _LIB_MFRONT_UMATTRAITS_HXX_
#define _LIB_MFRONT_UMATTRAITS_HXX_ 

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
    static const UMATBehaviourType btype = umat::SMALLSTRAINSTANDARDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static const unsigned short DrivingVariableSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static const unsigned short ThermodynamicForceVariableSize = 0u;
    static const bool useTimeSubStepping = false;
    static const bool doSubSteppingOnInvalidResults  = false;
    static const unsigned short maximumSubStepping   = 0u;
    static const bool requiresStiffnessTensor        = false;
    static const bool requiresThermalExpansionCoefficientTensor = false;
    static const unsigned short propertiesOffset       = 0u;
    static const unsigned short material_properties_nb = 0u;
    static const unsigned short internal_variables_nb  = 0u;
    static const UMATSymmetryType stype = umat::ISOTROPIC;
  }; // end of class UMATTraits

} // end of namespace umat  

#endif /* _LIB_MFRONT_UMATTRAITS_HXX */

