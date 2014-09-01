/*!
 * \file   include/TFEL/Math/General/EmptyRunTimeProperties.hxx
 * \brief  This file describes the EmptyRunTimeProperties class
 * \author Helfer Thomas
 * \date   03 oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

