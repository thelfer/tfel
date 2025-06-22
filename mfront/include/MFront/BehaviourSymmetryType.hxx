/*!
 * \file   mfront/include/MFront/BehaviourSymmetryType.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  10 avril 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURSYMMETRYTYPE_HXX
#define LIB_MFRONT_BEHAVIOURSYMMETRYTYPE_HXX

namespace mfront {

  /*!
   * For the castem finite element code, its is mandatory to
   * distinguish isotropic and orthotropic behaviours
   */
  enum BehaviourSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum BehaviourSymmetryType

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURSYMMETRYTYPE_HXX */
