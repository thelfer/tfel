/*!
 * \file   include/TFEL/Math/General/Compare.hxx
 * \brief
 * \author Antoine Martin
 * \date   05/09/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPARE_HXX
#define LIB_TFEL_MATH_COMPARE_HXX


namespace tfel::math {

  /*
   * \brief compare two values with a precision given by their base_type precision
   */
   template <ScalarConcept ScalarType>
   TFEL_HOST_DEVICE static constexpr bool areAlmostEqual(const base_type<ScalarType> a, const base_type<ScalarType> b){
 	constexpr auto eps = std::numeric_limits<base_type<ScalarType>>::epsilon();
 	return std::abs(a-b)< eps;
   }
   
   /*
   * \brief compare two quantities with a precision given by the quantity base_type precision
   */
   template <ScalarConcept ScalarType>
   TFEL_HOST_DEVICE static constexpr bool areAlmostEqual(const ScalarType& a, 
                                                         const ScalarType&b){
	constexpr auto eps = std::numeric_limits<base_type<ScalarType>>::epsilon();
	return std::abs(a.getValue()-b.getValue())< eps;
   }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_COMPARE_HXX */
