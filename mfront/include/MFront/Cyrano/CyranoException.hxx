/*!
 * \file   mfront/include/MFront/Cyrano/CyranoException.hxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANOEXCEPTION_HXX
#define LIB_MFRONT_CYRANOEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/Cyrano/CyranoConfig.hxx"

namespace cyrano {

  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoException
      : public std::exception {
    CyranoException(const std::string&);

    CyranoException(const CyranoException&);

    virtual const char* what() const noexcept override final;

    virtual std::string getMsg() const noexcept final;

    ~CyranoException() noexcept override;

   private:
    CyranoException();

    CyranoException& operator=(const CyranoException&);

    const std::string msg;

  };  // end of struct CyranoException

  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoIntegrationFailed final
      : public CyranoException {
    CyranoIntegrationFailed();
    CyranoIntegrationFailed(const std::string&);
    CyranoIntegrationFailed(const CyranoIntegrationFailed&);
    ~CyranoIntegrationFailed() noexcept override;

   private:
    CyranoIntegrationFailed& operator=(const CyranoIntegrationFailed&);
  };  // end of struct CyranoIntegrationFailed

  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoInvalidNTENSValue final
      : public CyranoException {
    CyranoInvalidNTENSValue(const unsigned short);
    CyranoInvalidNTENSValue(const CyranoInvalidNTENSValue&);
    ~CyranoInvalidNTENSValue() noexcept override;

   private:
    CyranoInvalidNTENSValue();
    CyranoInvalidNTENSValue& operator=(const CyranoInvalidNTENSValue&);
  };  // end of struct CyranoInvalidNTENSValue

  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoInvalidDimension final
      : public CyranoException {
    CyranoInvalidDimension(const std::string&, const unsigned short);
    CyranoInvalidDimension(const CyranoInvalidDimension&);
    ~CyranoInvalidDimension() noexcept override;

   private:
    CyranoInvalidDimension();
    CyranoInvalidDimension& operator=(const CyranoInvalidDimension&);
  };  // end of struct CyranoInvalidDimension

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANOEXCEPTION_HXX */
