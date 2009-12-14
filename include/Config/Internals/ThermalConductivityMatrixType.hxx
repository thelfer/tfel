/*!
 * \file   ThermalConductivityMatrixType.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 */

#ifndef _LIB_TFEL_THERMALCONDUCTIVITYMATRIXTYPE_H_
#define _LIB_TFEL_THERMALCONDUCTIVITYMATRIXTYPE_H_ 

#include"Math/tmatrix.hxx"
#include"Math/qt.hxx"

namespace tfel{ 
 
  namespace config {

    namespace internals{

      template<unsigned short N,typename T,bool use_qt>
      struct ThermalConductivityMatrixType
      {
	typedef tfel::math::tmatrix<N,N,tfel::math::qt<tfel::math::ThermalConductivity,T> > type;
      };

      template<unsigned short N,typename T>
      struct ThermalConductivityMatrixType<N,T,false>
      {
	typedef tfel::math::tmatrix<N,N,T> type;
      };

      template<typename T,bool use_qt>
      struct ThermalConductivityMatrixType<1u,T,use_qt>
      {
	typedef tfel::math::qt<tfel::math::ThermalConductivity,T> type;
      };

      template<typename T>
      struct ThermalConductivityMatrixType<1u,T,false>
      {
	typedef T type;
      };

    } // end of namespace internals

  } // end of namespace config

} // end of namespace tfel

#endif /* _LIB_TFEL_THERMALCONDUCTIVITYMATRIXTYPE_H */

