/*! 
 * \file  mfront/include/MFront/Ansys/AnsysOrthotropicBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 mars 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ANSYSORTHOTROPICBEHAVIOUR_HXX
#define LIB_MFRONT_ANSYSORTHOTROPICBEHAVIOUR_HXX 

#include"MFront/Ansys/AnsysConfig.hxx"

namespace ansys
{

  template<unsigned short>
  struct AnsysOrthotropicElasticPropertiesOffset;

  template<>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysOrthotropicElasticPropertiesOffset<2u>
  {
    static constexpr unsigned short value = 7u;
  }; // end of struct AnsysOrthotropicElasticPropertiesOffset<2u>

  template<>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysOrthotropicElasticPropertiesOffset<3u>
  {
    static constexpr unsigned short value = 9u;
  }; // end of struct AnsysOrthotropicElasticPropertiesOffset<3u>

  template<unsigned short>
  struct AnsysOrthotropicAxesOffset;

  template<>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysOrthotropicAxesOffset<2u>
  {
    static constexpr unsigned short value = 2u;
  }; // end of struct AnsysOrthotropicAxesOffset<2u>

  template<>
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysOrthotropicAxesOffset<3u>
  {
    static constexpr unsigned short value = 6u;
  }; // end of struct AnsysOrthotropicAxesOffset<3u>
  
} // end of namespace ansys

#endif /* LIB_MFRONT_ANSYSORTHOTROPICBEHAVIOUR_HXX */
