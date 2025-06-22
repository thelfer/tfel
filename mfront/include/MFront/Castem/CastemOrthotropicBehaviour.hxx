/*!
 * \file   mfront/include/MFront/Castem/CastemOrthotropicBehaviour.hxx
 * \brief  This file declares the CastemOrthotropicBehaviour class
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMORTHOTROPICBEHAVIOUR_H_
#define LIB_MFRONT_CASTEMORTHOTROPICBEHAVIOUR_H_

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/CastemTraits.hxx"

namespace castem {

  /*!
   * This behaviour tells which behaviour is used
   * \param[in] Type : behaviour type
   * \param[in] N    : spatial dimension
   */
  template <unsigned short N>
  struct CastemOrthotropicOffsetBase;

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemOrthotropicOffsetBase<1u> {
    // 6 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static constexpr unsigned short value = 10u;
  };  // end of struct CastemOrthotropicOffsetBase<1u>

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemOrthotropicOffsetBase<2u> {
    // 7 elastic coefficients
    // 2 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static constexpr unsigned short value = 13u;
  };  // end of struct CastemOrthotropicOffsetBase<2u>

  template <>
  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemOrthotropicOffsetBase<3u> {
    // 9 elastic coefficients
    // 6 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static constexpr unsigned short value = 19u;
  };  // end of struct CastemOrthotropicOffsetBase<3u>

  template <CastemBehaviourType Type,
            tfel::material::ModellingHypothesis::Hypothesis H>
  struct CastemOrthotropicOffset;

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct CastemOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR, H>
      : public CastemOrthotropicOffsetBase<
            tfel::material::ModellingHypothesisToSpaceDimension<H>::value> {};

  template <>
  struct CastemOrthotropicOffset<
      SMALLSTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    // 7 elastic coefficients
    // 2 components of direction
    //   mass density
    // 2 thermal expansion coefficients
    // 1 axial dimension
    static constexpr unsigned short value = 13u;
  };

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct CastemOrthotropicOffset<FINITESTRAINSTANDARDBEHAVIOUR, H>
      : public CastemOrthotropicOffsetBase<
            tfel::material::ModellingHypothesisToSpaceDimension<H>::value> {};

  template <>
  struct CastemOrthotropicOffset<
      FINITESTRAINSTANDARDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    // 7 elastic coefficients
    // 2 components of direction
    //   mass density
    // 2 thermal expansion coefficients
    // 1 axial dimension
    static constexpr unsigned short value = 13u;
  };

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMORTHOTROPICBEHAVIOUR_H_ */
