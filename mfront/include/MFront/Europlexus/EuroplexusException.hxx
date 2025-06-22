/*!
 * \file  mfront/include/MFront/Europlexus/EuroplexusException.hxx
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

#ifndef LIB_MFRONT_EPXEXCEPTION_HXX
#define LIB_MFRONT_EPXEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/Europlexus/EuroplexusConfig.hxx"

namespace epx {

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusException
      : public std::exception {
    EuroplexusException(const std::string&);

    EuroplexusException(const EuroplexusException&);

    const char* what() const noexcept override final;

    virtual std::string getMsg() const noexcept final;

    ~EuroplexusException() noexcept override;

   private:
    EuroplexusException() = delete;

    EuroplexusException& operator=(const EuroplexusException&) = delete;

    const std::string msg;

  };  // end of struct EuroplexusException

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInvalidModellingHypothesis final
      : public EuroplexusException {
    /*!
     * \param[in] b : behaviour name
     */
    EuroplexusInvalidModellingHypothesis(const char*);
    //! destructor
    ~EuroplexusInvalidModellingHypothesis() noexcept override;

   private:
    EuroplexusInvalidModellingHypothesis& operator=(
        const EuroplexusInvalidModellingHypothesis&) = delete;
  };  // end of struct EuroplexusInvalidModellingHypothesis

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInvalidNTENSValue final
      : public EuroplexusException {
    EuroplexusInvalidNTENSValue(const unsigned short);
    EuroplexusInvalidNTENSValue(const EuroplexusInvalidNTENSValue&);
    ~EuroplexusInvalidNTENSValue() noexcept override;

   private:
    EuroplexusInvalidNTENSValue();
    EuroplexusInvalidNTENSValue& operator=(const EuroplexusInvalidNTENSValue&);
  };  // end of struct EuroplexusInvalidNTENSValue

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInvalidDimension final
      : public EuroplexusException {
    EuroplexusInvalidDimension(const std::string&, const unsigned short);
    EuroplexusInvalidDimension(const EuroplexusInvalidDimension&);
    ~EuroplexusInvalidDimension() noexcept override;

   private:
    EuroplexusInvalidDimension();
    EuroplexusInvalidDimension& operator=(const EuroplexusInvalidDimension&);
  };  // end of struct EuroplexusInvalidDimension

}  // end of namespace epx

#endif /* LIB_MFRONT_EPXEXCEPTION_HXX */
