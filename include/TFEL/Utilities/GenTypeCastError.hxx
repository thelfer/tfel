/*!
 * \file   include/TFEL/Utilities/GenTypeCastError.hxx
 * \brief  This file implements the GenTypeCastError class
 * \see    GenTypeBase
 * \author Thomas Helfer
 * \date   29/01/2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_GENTYPECASTERROR_HXX
#define LIB_TFEL_GENTYPECASTERROR_HXX

#include <stdexcept>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::utilities {

  /*!
   * exception thrown when requesting a bad type out of a
   * GenTypeBase class
   */
  struct TFELUTILITIES_VISIBILITY_EXPORT GenTypeCastError
      : public std::runtime_error {
    //! \brief default consutructor
    GenTypeCastError();
    GenTypeCastError(const GenTypeCastError&) = default;
    GenTypeCastError(GenTypeCastError&&) = default;
    //! \brief destructor
    ~GenTypeCastError() noexcept override;
  };  // end of struct GenTypeCastErrro

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_GENTYPECASTERROR_HXX */
