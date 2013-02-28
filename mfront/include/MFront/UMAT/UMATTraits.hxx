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
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum UMATBehaviourType

  template<typename Behaviour>
  struct UMATTraits{
    static const bool useTimeSubStepping = false;
    static const bool doSubSteppingOnInvalidResults  = false;
    static const unsigned short maximumSubStepping   = 0u;
    static const bool requiresStiffnessTensor        = false;
    static const bool requiresThermalExpansionTensor = false;
    static const unsigned short propertiesOffset     = 0u;
    static const UMATBehaviourType type = umat::ISOTROPIC;
  }; // end of class UMATTraits

} // end of namespace umat  

#endif /* _LIB_MFRONT_UMATTRAITS_HXX */

