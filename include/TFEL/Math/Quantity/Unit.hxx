/*!
 * \file   include/TFEL/Math/Quantity/Unit.hxx
 * \brief  This file declares the classes Unit, GenerateUnit, AddUnit_, SubUnit_, PowUnit_ and RootUnit_.
 * \author Helfer Thomas
 * \date   07 Jun 2006
 */

#ifndef _LIB_TFEL_UNIT_H_
#define _LIB_TFEL_UNIT_H_ 

#include<string>
#include<sstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/IntToType.hxx"
#include"TFEL/Math/Forward/qt.hxx"

namespace tfel{

  namespace math{

    template<typename N1,typename N2,typename N3,typename N4,typename N5,
	     typename N6,typename N7,typename D1,typename D2,typename D3,
	     typename D4,typename D5,typename D6,typename D7>
    struct Unit;

    template<int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     unsigned int D1,unsigned int D2,unsigned int D3,
	     unsigned int D4,unsigned int D5,unsigned int D6,
	     unsigned int D7>
    struct Unit<tfel::meta::Int2Type<N1>,
		tfel::meta::Int2Type<N2>,
		tfel::meta::Int2Type<N3>,
		tfel::meta::Int2Type<N4>,
		tfel::meta::Int2Type<N5>,
		tfel::meta::Int2Type<N6>,
		tfel::meta::Int2Type<N7>,
		tfel::meta::UInt2Type<D1>,
		tfel::meta::UInt2Type<D2>,
		tfel::meta::UInt2Type<D3>,
		tfel::meta::UInt2Type<D4>,
		tfel::meta::UInt2Type<D5>,
		tfel::meta::UInt2Type<D6>,
		tfel::meta::UInt2Type<D7> >
    {
      static std::string getName(void);
    };

    template<typename A, typename B>
    struct AddUnit_;
    
    template<typename A, typename B>
    struct SubUnit_;

    template<int N,unsigned int D,typename A>
    struct PowUnit_;
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Quantity/Unit.ixx"

#endif /* _LIB_TFEL_UNIT_H */

