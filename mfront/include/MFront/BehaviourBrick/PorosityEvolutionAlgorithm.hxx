/*!
 * \file   include/MFront/BehaviourBrick/PorosityEvolutionAlgorithm.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/07/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BBRICK_POROSITYEVOLUTIONALGORITHM_HXX
#define LIB_MFRONT_BBRICK_POROSITYEVOLUTIONALGORITHM_HXX

namespace mfront::bbrick {

  /*!
   * \brief Algorithm used to handle the porosity evolution
   */
  enum struct PorosityEvolutionAlgorithm {
    STANDARD_IMPLICIT_SCHEME,  //!< \brief monolithich approach
    STAGGERED_SCHEME           //!< \brief staggered approach
  };                           // end of enum struct PorosityEvolutionAlgorithm

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BBRICK_POROSITYEVOLUTIONALGORITHM_HXX */
