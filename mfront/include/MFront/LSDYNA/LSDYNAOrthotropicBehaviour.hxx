/*!
 * \file  mfront/include/MFront/LSDYNA/LSDYNAOrthotropicBehaviour.hxx
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

#ifndef LIB_MFRONT_LSDYNAORTHOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_LSDYNAORTHOTROPICBEHAVIOUR_HXX

#include "MFront/LSDYNA/LSDYNAConfig.hxx"

namespace lsdyna {

  template <unsigned short>
  struct LSDYNAOrthotropicElasticPropertiesOffset;

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAOrthotropicElasticPropertiesOffset<2u> {
    static constexpr unsigned short value = 7u;
  };  // end of struct LSDYNAOrthotropicElasticPropertiesOffset<2u>

  template <>
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT
      LSDYNAOrthotropicElasticPropertiesOffset<3u> {
    static constexpr unsigned short value = 9u;
  };  // end of struct LSDYNAOrthotropicElasticPropertiesOffset<3u>

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNAORTHOTROPICBEHAVIOUR_HXX */
