/*! 
 * \file   mfront/include/MFront/MechanicalBehaviourSymmetryType.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 avril 2014
 */

#ifndef _LIB_MFRONT_MECHANICALBEHAVIOURSYMMETRYTYPE_H_
#define _LIB_MFRONT_MECHANICALBEHAVIOURSYMMETRYTYPE_H_ 

namespace mfront
{

  /*!
   * For the castem finite element code, its is mandatory to
   * distinguish isotropic and orthotropic behaviours
   */
  enum MechanicalBehaviourSymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum MechanicalBehaviourSymmetryType

} // end of namespace mfront

#endif /* _LIB_MFRONT_MECHANICALBEHAVIOURSYMMETRYTYPE_H */

