/*!
 * \file  mfront/include/MFront/Ansys/AnsysOrthotropicBehaviour.hxx
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

#ifndef LIB_MFRONT_ANSYSORTHOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_ANSYSORTHOTROPICBEHAVIOUR_HXX

#include "MFront/Ansys/AnsysConfig.hxx"

namespace ansys {
  /*!
   * \brief A metafunction returning the number of elastic properties
   * as a function of the space dimension
   * \tparam space dimension
   */
  template <unsigned short>
  struct AnsysOrthotropicElasticPropertiesOffset;
  /*!
   * \brief Partial specialisation of the
   * `AnsysOrthotropicElasticPropertiesOffset` in 2D
   */
  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysOrthotropicElasticPropertiesOffset<2u> {
    //! the result of the metafunction
    static constexpr unsigned short value = 7u;
  };  // end of struct AnsysOrthotropicElasticPropertiesOffset<2u>
  /*!
   *  \brief Partial specialisation of the
   * `AnsysOrthotropicElasticPropertiesOffset` in 3D
   */
  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT
      AnsysOrthotropicElasticPropertiesOffset<3u> {
    //! the result of the metafunction
    static constexpr unsigned short value = 9u;
  };  // end of struct AnsysOrthotropicElasticPropertiesOffset<3u>
  /*!
   * \brief A metafunction returning the offset the orthotropic axes
   * in the list of state variables.
   * \tparam space dimension
   */
  template <unsigned short>
  struct AnsysOrthotropicAxesOffset;
  /*!
   * \brief Partial specialisation of the `AnsysOrthotropicAxesOffset`
   * in 2D
   */
  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysOrthotropicAxesOffset<2u> {
    //! the result of the metafunction
    static constexpr unsigned short value = 2u;
  };  // end of struct AnsysOrthotropicAxesOffset<2u>
  /*!
   * \brief Partial specialisation of the `AnsysOrthotropicAxesOffset`
   * in 3D
   */
  template <>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysOrthotropicAxesOffset<3u> {
    //! the result of the metafunction
    static constexpr unsigned short value = 6u;
  };  // end of struct AnsysOrthotropicAxesOffset<3u>

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYSORTHOTROPICBEHAVIOUR_HXX */
