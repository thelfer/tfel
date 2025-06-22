/*!
 * \file   CastemIsotropicBehaviour.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   20 sept. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMISOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_CASTEM_CASTEMISOTROPICBEHAVIOUR_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/CastemTraits.hxx"

namespace castem {

  template <CastemBehaviourType Type,
            tfel::material::ModellingHypothesis::Hypothesis H>
  struct CastemIsotropicOffset {
    static constexpr const unsigned short value = 4u;
  };

  template <>
  struct CastemIsotropicOffset<
      SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    static constexpr const unsigned short value = 5u;
  };

  template <>
  struct CastemIsotropicOffset<
      FINITESTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    static constexpr const unsigned short value = 5u;
  };

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMISOTROPICBEHAVIOUR_HXX_ */
