/*!
 * \file   CyranoTraits.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANOTRAITS_HXX_
#define _LIB_MFRONT_CYRANOTRAITS_HXX_ 

namespace cyrano{

  enum CyranoSymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum CyranoSymmetryType

  template<typename Behaviour>
  struct CyranoTraits{
    static const bool useTimeSubStepping = false;
    static const bool doSubSteppingOnInvalidResults  = false;
    static const unsigned short maximumSubStepping   = 0u;
    static const bool requiresStiffnessOperator      = false;
    static const bool requiresThermalExpansionTensor = false;
    static const unsigned short propertiesOffset     = 0u;
    static const CyranoSymmetryType stype = cyrano::ISOTROPIC;
  }; // end of class CyranoTraits

} // end of namespace cyrano  

#endif /* _LIB_MFRONT_CYRANOTRAITS_HXX */

