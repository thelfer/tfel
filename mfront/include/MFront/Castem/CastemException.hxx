/*!
 * \file   mfront/include/MFront/Castem/CastemException.hxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMEXCEPTION_HXX
#define LIB_MFRONT_CASTEMEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/Castem/CastemConfig.hxx"

namespace castem {

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemException
      : public std::exception {
    CastemException(const std::string&);
    CastemException(const CastemException&) = default;
    CastemException(CastemException&&) = default;
    virtual const char* what() const noexcept override final;
    ~CastemException() noexcept override;

   private:
    const std::string msg;
  };  // end of struct CastemException

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemIntegrationFailed final
      : public CastemException {
    CastemIntegrationFailed();
    CastemIntegrationFailed(const std::string&);
    CastemIntegrationFailed(const CastemIntegrationFailed&);
    ~CastemIntegrationFailed() noexcept override;

   private:
    CastemIntegrationFailed& operator=(const CastemIntegrationFailed&);
  };  // end of struct CastemIntegrationFailed

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemInvalidNTENSValue final
      : public CastemException {
    CastemInvalidNTENSValue(const unsigned short);
    CastemInvalidNTENSValue(const CastemInvalidNTENSValue&);
    ~CastemInvalidNTENSValue() noexcept override;

   private:
    CastemInvalidNTENSValue();
    CastemInvalidNTENSValue& operator=(const CastemInvalidNTENSValue&);
  };  // end of struct CastemInvalidNTENSValue

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemInvalidDimension final
      : public CastemException {
    CastemInvalidDimension(const std::string&, const unsigned short);
    CastemInvalidDimension(const CastemInvalidDimension&);
    ~CastemInvalidDimension() noexcept override;

   private:
    CastemInvalidDimension();
    CastemInvalidDimension& operator=(const CastemInvalidDimension&);
  };  // end of struct CastemInvalidDimension

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMEXCEPTION_HXX */
