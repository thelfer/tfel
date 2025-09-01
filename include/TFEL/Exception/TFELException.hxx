/*!
 * \file   include/TFEL/Exception/TFELException.hxx
 * \brief  This file declares the TFELException class.
 * \author Thomas Helfer
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_EXCEPTIONS_HXX
#define LIB_TFEL_EXCEPTIONS_HXX

#include <stdexcept>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::exception {

  /*!
   * \struct TFELException
   * \brief  generic Exception, base class for all the other exception used in
   * the `TFEL` project.
   * \author Thomas Helfer
   * \date   02 Aug 2006
   */
  struct TFELEXCEPTION_VISIBILITY_EXPORT TFELException : public std::exception {
    //! default constructor
    TFELException() = default;
    //! copy constructor
    TFELException(const TFELException&) = default;
    //! copy constructor
    TFELException(TFELException&&) = default;
    //! destructor
    ~TFELException() noexcept override;
  };  // end of struct TFELException

}  // end of namespace tfel::exception

#endif /* LIB_TFEL_EXCEPTIONS_HXX */
