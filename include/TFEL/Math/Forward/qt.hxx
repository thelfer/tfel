/*! 
 * \file  qt.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 jan. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_QT_H_
#define _LIB_TFEL_MATH_FORWARD_QT_H_ 

#include"TFEL/Metaprogramming/IntToType.hxx"

namespace tfel
{

  namespace math
  {

    /*
     * \class qt
     * \brief This class describes numbers with unit.
     * qt must be has efficient and simple to use that standard numerical types.
     * \param unit, the unit of the qt.
     * \param T, the underlying numerical type.
     * \pre T must be a fundamental numerical type.
     * \see unit_samples.hxx, IsFundamentalNumericType
     * \author Helfer Thomas
     * \date   06 Jun 2006
     */
    template<typename unit,typename T>
    class qt;

    /*!
     * structure describing an unit
     */
    template<typename N1,typename N2,typename N3,typename N4,typename N5,
	     typename N6,typename N7,typename D1,typename D2,typename D3,
	     typename D4,typename D5,typename D6,typename D7>
    struct Unit;

    // A simple alias
    typedef Unit<tfel::meta::Int2Type<0>,
		 tfel::meta::Int2Type<0>,
		 tfel::meta::Int2Type<0>,
		 tfel::meta::Int2Type<0>,
		 tfel::meta::Int2Type<0>,
		 tfel::meta::Int2Type<0>,
		 tfel::meta::Int2Type<0>,
		 tfel::meta::UInt2Type<0u>,
		 tfel::meta::UInt2Type<0u>,
		 tfel::meta::UInt2Type<0u>,
		 tfel::meta::UInt2Type<0u>,
		 tfel::meta::UInt2Type<0u>,
		 tfel::meta::UInt2Type<0u>,
		 tfel::meta::UInt2Type<0u> > NoUnit;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_QT_H */

