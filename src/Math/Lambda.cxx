/*!
 * \file   src/Math/Lambda.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   12 déc 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/lambda.hxx"

namespace tfel{

  namespace math{
    
    TFELMATH_VISIBILITY_EXPORT Lambda<1u> x_;
    TFELMATH_VISIBILITY_EXPORT Lambda<2u> y_;
    TFELMATH_VISIBILITY_EXPORT Lambda<3u> z_;

  } // end of namespace math

} // end of namespace tfel
