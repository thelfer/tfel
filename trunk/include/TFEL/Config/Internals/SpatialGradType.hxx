/*!
 * \file   SpatialGradType.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 */

#ifndef _LIB_TFEL_SPATIALGRADTYPE_H_
#define _LIB_TFEL_SPATIALGRADTYPE_H_ 

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/qt.hxx"

namespace tfel{ 
 
  namespace config {

    namespace internals{

      template<unsigned short N,typename T,bool use_qt>
      struct SpatialGradType
      {
	typedef tfel::math::tvector<N,tfel::math::qt<tfel::math::InvLength,T> > type;
      };

      template<unsigned short N,typename T>
      struct SpatialGradType<N,T,false>
      {
	typedef tfel::math::tvector<N,T> type;
      };

      template<typename T,bool use_qt>
      struct SpatialGradType<1u,T,use_qt>
      {
	typedef tfel::math::qt<tfel::math::InvLength,T> type;
      };

      template<typename T>
      struct SpatialGradType<1u,T,false>
      {
	typedef T type;
      };

    } // end of namespace internals

  } // end of namespace config

} // end of namespace tfel


#endif /* _LIB_TFEL_SPATIALGRADTYPE_H */

