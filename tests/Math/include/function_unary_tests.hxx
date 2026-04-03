/*!
 * \file   tests/Math/include/function_unary_tests.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FUNCTION_HXX
#define LIB_TFEL_FUNCTION_HXX

#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

void function(const tfel::math::VectorConcept auto&);

template <tfel::math::VectorConcept VectorType>
void function(const VectorType&,
              const tfel::math::numeric_type<VectorType>,
              const tfel::math::numeric_type<VectorType>,
              const tfel::math::numeric_type<VectorType>,
              const tfel::math::numeric_type<VectorType>,
              const unsigned int);

template <tfel::math::MatrixConcept MatrixType>
void function(const MatrixType&,
              const tfel::math::numeric_type<MatrixType>,
              const tfel::math::numeric_type<MatrixType>,
              const tfel::math::numeric_type<MatrixType>,
              const tfel::math::numeric_type<MatrixType>,
              const tfel::math::numeric_type<MatrixType>,
              const unsigned int);

void function(const tfel::math::StensorConcept auto&);

#include "function_unary_tests.ixx"

#endif /* LIB_TFEL_FUNCTION_HXX */
