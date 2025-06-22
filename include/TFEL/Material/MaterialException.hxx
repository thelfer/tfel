/*!
 * \file   include/TFEL/Material/MaterialException.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIALEXCEPTION_HXX
#define LIB_TFEL_MATERIALEXCEPTION_HXX

#include <string>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Exception/TFELException.hxx"

namespace tfel {

  namespace material {

    struct TFELMATERIAL_VISIBILITY_EXPORT MaterialException
        : public tfel::exception::TFELException {
      MaterialException() = default;
      MaterialException(const char* const);
      MaterialException(const std::string&);
      MaterialException(MaterialException&&) = default;
      MaterialException(const MaterialException&) = default;
      virtual ~MaterialException() noexcept;
    };

    struct TFELMATERIAL_VISIBILITY_EXPORT DivergenceException
        : public MaterialException {
      DivergenceException() = default;
      DivergenceException(const char* const);
      DivergenceException(const std::string&);
      DivergenceException(DivergenceException&&) = default;
      DivergenceException(const DivergenceException&) = default;
      virtual const char* what() const noexcept override final;
      virtual ~DivergenceException() noexcept;
    };

    struct TFELMATERIAL_VISIBILITY_EXPORT OutOfBoundsException
        : public MaterialException {
      OutOfBoundsException(std::string);
      OutOfBoundsException(OutOfBoundsException&&) = default;
      OutOfBoundsException(const OutOfBoundsException&) = default;
      virtual const char* what() const noexcept override final;
      virtual ~OutOfBoundsException() noexcept;

     private:
      std::string msg;
    };

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIALEXCEPTION_HXX */
