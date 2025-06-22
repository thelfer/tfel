/*!
 * \file  mfront/include/MFront/Abaqus/AbaqusOrthotropicBehaviour.hxx
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

#ifndef LIB_MFRONT_ABAQUSORTHOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_ABAQUSORTHOTROPICBEHAVIOUR_HXX

#include "MFront/Abaqus/AbaqusConfig.hxx"

namespace abaqus {

  template <unsigned short>
  struct AbaqusOrthotropicElasticPropertiesOffset;

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusOrthotropicElasticPropertiesOffset<2u> {
    static constexpr unsigned short value = 7u;
  };  // end of struct AbaqusOrthotropicElasticPropertiesOffset<2u>

  template <>
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT
      AbaqusOrthotropicElasticPropertiesOffset<3u> {
    static constexpr unsigned short value = 9u;
  };  // end of struct AbaqusOrthotropicElasticPropertiesOffset<3u>

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUSORTHOTROPICBEHAVIOUR_HXX */
