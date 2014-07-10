/*!
 * \file   JacobianType.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 */

#ifndef _LIB_TFEL_JACOBIANTYPE_H_
#define _LIB_TFEL_JACOBIANTYPE_H_ 

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/Forward/qt.hxx"

namespace tfel{ 
 
  namespace config {

    namespace internals{

      template<unsigned short N,typename T,bool use_qt>
      struct JacobianType
      {
	typedef tfel::math::tmatrix<N,N,tfel::math::qt<tfel::math::NoUnit,T> > type;
      };

      template<unsigned short N,typename T>
      struct JacobianType<N,T,false>
      {
	typedef tfel::math::tmatrix<N,N,T> type;
      };

      template<typename T,bool use_qt>
      struct JacobianType<1u,T,use_qt>
      {
	typedef tfel::math::qt<tfel::math::NoUnit,T> type;
      };

      template<typename T>
      struct JacobianType<1u,T,false>
      {
	typedef T type;
      };

    } // end of namespace internals

  } // end of namespace config

} // end of namespace tfel

#endif /* _LIB_TFEL_JACOBIANTYPE_H */

