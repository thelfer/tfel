/*! 
 * \file   mfront/include/MFront/BehaviourSymmetryType.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 avril 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_BEHAVIOURSYMMETRYTYPE_H_
#define _LIB_MFRONT_BEHAVIOURSYMMETRYTYPE_H_ 

namespace mfront
{

  /*!
   * For the castem finite element code, its is mandatory to
   * distinguish isotropic and orthotropic behaviours
   */
  enum BehaviourSymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum BehaviourSymmetryType

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURSYMMETRYTYPE_H */

