/*! 
 * \file  src/Math/LUException.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Math/LU/LUException.hxx"

namespace tfel
{

  namespace math
  {

    LUException::LUException() = default;
    LUException::LUException(LUException&&) = default;
    LUException::LUException(const LUException&) = default;
    
    LUException::~LUException() noexcept
    {} // end of LUException::~LUException

    LUMatrixNotSquare::LUMatrixNotSquare() = default;
    LUMatrixNotSquare::LUMatrixNotSquare(LUMatrixNotSquare&&) = default;
    LUMatrixNotSquare::LUMatrixNotSquare(const LUMatrixNotSquare&) = default;

    const char*
    LUMatrixNotSquare::what() const noexcept 
    {
      return "LUDecomp::exe : matrix is not square";
    } // end of LUMatrixNotSquare::what

    LUMatrixNotSquare::~LUMatrixNotSquare() noexcept
    {} // end of LUMatrixNotSquare::~LUMatrixNotSquare

    LUUnmatchedSize::LUUnmatchedSize() = default;
    LUUnmatchedSize::LUUnmatchedSize(LUUnmatchedSize&&) = default;
    LUUnmatchedSize::LUUnmatchedSize(const LUUnmatchedSize&) = default;

    const char*
    LUUnmatchedSize::what() const noexcept 
    {
      return "LUDecomp::exe : matrix size and permutation size does not match";
    } // end of LUUnmatchedSize::what

    LUUnmatchedSize::~LUUnmatchedSize() noexcept
    {} // end of LUUnmatchedSize::~LUUnmatchedSize

    LUInvalidMatrixSize::LUInvalidMatrixSize() = default;
    LUInvalidMatrixSize::LUInvalidMatrixSize(LUInvalidMatrixSize&&) = default;
    LUInvalidMatrixSize::LUInvalidMatrixSize(const LUInvalidMatrixSize&) = default;

    const char*
    LUInvalidMatrixSize::what() const noexcept
    {
      return "LUDecomp : null matrix size";
    } // end of LUInvalidMatrixSize::what

    LUInvalidMatrixSize::~LUInvalidMatrixSize() noexcept
    {} // end of LUInvalidMatrixSize::~LUInvalidMatrixSize

    LUNullPivot::LUNullPivot() = default;
    LUNullPivot::LUNullPivot(LUNullPivot&&) = default;
    LUNullPivot::LUNullPivot(const LUNullPivot&) = default;

    const char*
    LUNullPivot::what() const noexcept
    {
      return "LUDecomp::back_substitute : null pivot";
    } // end of LUNullPivot::LUNullPivot

    LUNullPivot::~LUNullPivot() noexcept
    {} // end of LUNullPivot::~LUNullPivot

    LUNullDeterminant::LUNullDeterminant() = default;
    LUNullDeterminant::LUNullDeterminant(LUNullDeterminant&&) = default;
    LUNullDeterminant::LUNullDeterminant(const LUNullDeterminant&) = default;
    
    const char*
    LUNullDeterminant::what() const noexcept
    {
      return "LUDecomp::back_substitute : null pivot";
    } // end of LUNullDeterminant::LUNullDeterminant

    LUNullDeterminant::~LUNullDeterminant() noexcept
    {} // end of LUNullDeterminant::~LUNullDeterminant

  } // end of namespace math

} // end of namespace tfel
