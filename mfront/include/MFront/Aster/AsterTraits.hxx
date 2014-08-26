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

  enum AsterSymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum AsterSymmetryType

  enum AsterErrorReportPolicy{
    ASTER_WRITEONSTDOUT,
    ASTER_NOERRORREPORT
  }; // end of enum AsterErrorReportPolicy

  template<typename Behaviour>
  struct AsterTraits{
    // size of the driving variable array (STRAN)
    static const unsigned short DrivingVariableSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static const unsigned short ThermodynamicForceVariableSize = 0u;
    static const AsterErrorReportPolicy errorReportPolicy = ASTER_WRITEONSTDOUT;
    static const bool requiresStiffnessTensor             = false;
    static const bool requiresThermalExpansionCoefficientTensor = false;
    static const unsigned short propertiesOffset          = 0u;
    // Type of the behaviour of the material (isotropic or orthotropic)
    static const AsterSymmetryType type   = aster::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static const AsterSymmetryType  etype = aster::ISOTROPIC;
  }; // end of class ASTERTraits

} // end of namespace aster  

#endif /* _LIB_MFRONT_ASTERTRAITS_HXX */

