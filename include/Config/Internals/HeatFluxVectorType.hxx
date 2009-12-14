/*!
 * \file   HeatFluxVectorType.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 */

#ifndef _LIB_TFEL_HEATFLUXVECTORTYPE_H_
#define _LIB_TFEL_HEATFLUXVECTORTYPE_H_ 

#include"Math/tvector.hxx"
#include"Math/qt.hxx"

namespace tfel{ 
 
  namespace config {

    namespace internals{

      template<unsigned short N,typename T,bool use_qt>
      struct HeatFluxVectorType
      {
	typedef tfel::math::tvector<N,tfel::math::qt<tfel::math::HeatFluxDensity,T> > type;
      };

      template<unsigned short N,typename T>
      struct HeatFluxVectorType<N,T,false>
      {
	typedef tfel::math::tvector<N,T> type;
      };

      template<typename T,bool use_qt>
      struct HeatFluxVectorType<1u,T,use_qt>
      {
	typedef tfel::math::qt<tfel::math::HeatFluxDensity,tfel::math::qt<tfel::math::NoUnit,T> > type;
      };

      template<typename T>
      struct HeatFluxVectorType<1u,T,false>
      {
	typedef T type;
      };

    } // end of namespace internals

  } // end of namespace config

} // end of namespace tfel

#endif /* _LIB_TFEL_HEATFLUXVECTORTYPE_H */

