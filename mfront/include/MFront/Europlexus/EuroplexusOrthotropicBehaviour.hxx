/*!
 * \file  mfront/include/MFront/Europlexus/EuroplexusOrthotropicBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 20 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EPX_ORTHOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_EPX_ORTHOTROPICBEHAVIOUR_HXX

#include "MFront/Europlexus/EuroplexusConfig.hxx"

namespace epx {

  template <unsigned short>
  struct EuroplexusOrthotropicElasticPropertiesOffset;

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusOrthotropicElasticPropertiesOffset<1u> {
    static constexpr unsigned short value = 6u;
  };  // end of struct EuroplexusOrthotropicElasticPropertiesOffset<1u>

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusOrthotropicElasticPropertiesOffset<2u> {
    static constexpr unsigned short value = 7u;
  };  // end of struct EuroplexusOrthotropicElasticPropertiesOffset<2u>

  template <>
  struct MFRONT_EPX_VISIBILITY_EXPORT
      EuroplexusOrthotropicElasticPropertiesOffset<3u> {
    static constexpr unsigned short value = 9u;
  };  // end of struct EuroplexusOrthotropicElasticPropertiesOffset<3u>

}  // end of namespace epx

#endif /* LIB_MFRONT_EPX_ORTHOTROPICBEHAVIOUR_HXX */
