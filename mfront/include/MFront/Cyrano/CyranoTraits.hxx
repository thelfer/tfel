/*!
 * \file   mfront/include/MFront/Cyrano/CyranoTraits.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   21/02/2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANOTRAITS_HXX
#define LIB_MFRONT_CYRANOTRAITS_HXX

namespace cyrano {

  enum CyranoSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum CyranoSymmetryType

  template <typename Behaviour>
  struct CyranoTraits {
    static constexpr bool useTimeSubStepping = false;
    static constexpr bool doSubSteppingOnInvalidResults = false;
    static constexpr unsigned short maximumSubStepping = 0u;
    static constexpr bool requiresStiffnessTensor = false;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short material_properties_nb = 0u;
    static constexpr unsigned short internal_variables_nb = 0u;
    static constexpr unsigned short propertiesOffset = 0u;
    static constexpr unsigned short elasticPropertiesOffset = 0u;
    static constexpr CyranoSymmetryType stype = cyrano::ISOTROPIC;
  };  // end of class CyranoTraits

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANOTRAITS_HXX */
