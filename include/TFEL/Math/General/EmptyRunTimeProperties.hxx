/*!
 * \file   EmptyRunTimeProperties.hxx
 * \brief  This file describes the EmptyRunTimeProperties class
 * \author Helfer Thomas
 * \date   03 oct 2006
 */

#ifndef _LIB_TFEL_EMPTYRUNTIMEPROPERTIES_HXX_
#define _LIB_TFEL_EMPTYRUNTIMEPROPERTIES_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace math{

    /*!
     * \class EmptyRunTimeProperties
     * \brief This class is aimed to used for mathematical objects whose 
     * size are known at compile-tme.
     * \see tvector, tmatrix, stensor
     */
    struct EmptyRunTimeProperties
    {

      TFEL_MATH_INLINE
      EmptyRunTimeProperties()
      {}

      TFEL_MATH_INLINE
      EmptyRunTimeProperties(const EmptyRunTimeProperties&)
      {}

      TFEL_MATH_INLINE
      EmptyRunTimeProperties(const EmptyRunTimeProperties&,
			     const EmptyRunTimeProperties&)
      {}

      TFEL_MATH_INLINE EmptyRunTimeProperties&
      operator=(const EmptyRunTimeProperties&)
      {
	return *this;
      }

    };

    inline bool 
    operator == (const EmptyRunTimeProperties&,const EmptyRunTimeProperties&)
    {
      return true;
    }

    inline bool 
    operator != (const EmptyRunTimeProperties&,const EmptyRunTimeProperties&)
    {
      return false;
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_EMPTYRUNTIMEPROPERTIES_HXX */

