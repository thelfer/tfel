/*!
 * \file  mfront/include/MFront/Aster/AsterException.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTEREXCEPTION_HXX
#define LIB_MFRONT_ASTEREXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/Aster/AsterConfig.hxx"

namespace aster {

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterException : public std::exception {
    AsterException(const std::string&);

    AsterException(const AsterException&);

    const char* what() const noexcept override final;

    virtual std::string getMsg() const noexcept final;

    ~AsterException() noexcept override;

   private:
    AsterException() = delete;

    AsterException& operator=(const AsterException&) = delete;

    const std::string msg;

  };  // end of struct AsterException

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInvalidNTENSValue final
      : public AsterException {
    AsterInvalidNTENSValue(const unsigned short);
    AsterInvalidNTENSValue(const AsterInvalidNTENSValue&);
    ~AsterInvalidNTENSValue() noexcept override;

   private:
    AsterInvalidNTENSValue();
    AsterInvalidNTENSValue& operator=(const AsterInvalidNTENSValue&);
  };  // end of struct AsterInvalidNTENSValue

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInvalidModellingHypothesis final
      : public AsterException {
    AsterInvalidModellingHypothesis();
    AsterInvalidModellingHypothesis(const AsterInvalidModellingHypothesis&);
    ~AsterInvalidModellingHypothesis() noexcept override;

   private:
    AsterInvalidModellingHypothesis& operator=(
        const AsterInvalidModellingHypothesis&) = delete;
    AsterInvalidModellingHypothesis& operator=(
        AsterInvalidModellingHypothesis&&) = delete;
  };  // end of struct AsterInvalidModellingHypothesis

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInvalidDimension final
      : public AsterException {
    AsterInvalidDimension(const std::string&, const unsigned short);
    AsterInvalidDimension(const AsterInvalidDimension&);
    ~AsterInvalidDimension() noexcept override;

   private:
    AsterInvalidDimension();
    AsterInvalidDimension& operator=(const AsterInvalidDimension&);
  };  // end of struct AsterInvalidDimension

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTEREXCEPTION_HXX */
