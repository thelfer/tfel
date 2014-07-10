/*! 
 * \file  AsterOrthotropicBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 mars 2013
 */

#ifndef _LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_H_
#define _LIB_MFRONT_ASTERORTHOTROPICBEHAVIOUR_H_ 

#include"MFront/Aster/AsterConfig.hxx"

namespace aster
{

  template<unsigned short>
  struct AsterOrthotropicElasticPropertiesOffset;

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterOrthotropicElasticPropertiesOffset<1u>
  {
    static const unsigned short value = 6u;
  }; // end of struct AsterOrthotropicElasticPropertiesOffset<1u>

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterOrthotropicElasticPropertiesOffset<2u>
  {
    static const unsigned short value = 7u;
  }; // end of struct AsterOrthotropicElasticPropertiesOffset<2u>

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterOrthotropicElasticPropertiesOffset<3u>
  {
    static const unsigned short value = 9u;
  }; // end of struct AsterOrthotropicElasticPropertiesOffset<3u>

} // end of namespace aster

#endif /* _LIB_ASTERORTHOTROPICBEHAVIOUR_H */
