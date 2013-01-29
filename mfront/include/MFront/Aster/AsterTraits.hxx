/*!
 * \file   ASTERTraits.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 fév 2007
 */

#ifndef _LIB_MFRONT_ASTERTRAITS_HXX_
#define _LIB_MFRONT_ASTERTRAITS_HXX_ 

namespace aster{

  enum AsterBehaviourType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum ASTERBehaviourType

  template<typename Behaviour>
  struct AsterTraits{
    static const bool requiresStiffnessTensor        = false;
    static const bool requiresThermalExpansionTensor = false;
    static const unsigned short propertiesOffset     = 0u;
    static const AsterBehaviourType type = aster::ISOTROPIC;
  }; // end of class ASTERTraits

} // end of namespace aster  

#endif /* _LIB_MFRONT_ASTERTRAITS_HXX */

