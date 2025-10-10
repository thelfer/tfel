/*!
 * \file  mfront/include/MFront/Aster/AsterOrthotropicBehaviour.hxx
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

#ifndef LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_HXX

#include "MFront/Aster/AsterConfig.hxx"

namespace aster {

  template <unsigned short>
  struct AsterOrthotropicElasticPropertiesOffset;

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterOrthotropicElasticPropertiesOffset<1u> {
    static constexpr unsigned short value = 6u;
  };  // end of struct AsterOrthotropicElasticPropertiesOffset<1u>

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterOrthotropicElasticPropertiesOffset<2u> {
    static constexpr unsigned short value = 7u;
  };  // end of struct AsterOrthotropicElasticPropertiesOffset<2u>

  template <>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
      AsterOrthotropicElasticPropertiesOffset<3u> {
    static constexpr unsigned short value = 9u;
  };  // end of struct AsterOrthotropicElasticPropertiesOffset<3u>

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_HXX */
