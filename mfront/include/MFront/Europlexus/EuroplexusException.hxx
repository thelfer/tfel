/*!
 * \file  mfront/include/MFront/Europlexus/EuroplexusException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EPXEXCEPTION_H_
#define LIB_MFRONT_EPXEXCEPTION_H_

#include <string>
#include <exception>

#include "MFront/Europlexus/EuroplexusConfig.hxx"

namespace epx {

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusException
      : public std::exception {
    EuroplexusException(const std::string&);

    EuroplexusException(const EuroplexusException&);

    virtual const char* what(void) const noexcept override final;

    virtual std::string getMsg(void) const noexcept final;

    virtual ~EuroplexusException() noexcept;

   private:
    EuroplexusException() = delete;

    EuroplexusException& operator=(const EuroplexusException&) = delete;

    const std::string msg;

  };  // end of struct EuroplexusException

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInvalidModellingHypothesis
      : public EuroplexusException {
    /*!
     * \param[in] b : behaviour name
     */
    EuroplexusInvalidModellingHypothesis(const char*);
    //! destructor
    virtual ~EuroplexusInvalidModellingHypothesis() noexcept;
    EuroplexusInvalidModellingHypothesis& operator=(
        const EuroplexusInvalidModellingHypothesis&) = delete;
  };  // end of struct EuroplexusInvalidModellingHypothesis

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInvalidNTENSValue
      : public EuroplexusException {
    EuroplexusInvalidNTENSValue(const unsigned short);
    EuroplexusInvalidNTENSValue(const EuroplexusInvalidNTENSValue&);
    virtual ~EuroplexusInvalidNTENSValue() noexcept;

   private:
    EuroplexusInvalidNTENSValue();
    EuroplexusInvalidNTENSValue& operator=(const EuroplexusInvalidNTENSValue&);
  };  // end of struct EuroplexusInvalidNTENSValue

  struct MFRONT_EPX_VISIBILITY_EXPORT EuroplexusInvalidDimension
      : public EuroplexusException {
    EuroplexusInvalidDimension(const std::string&, const unsigned short);
    EuroplexusInvalidDimension(const EuroplexusInvalidDimension&);
    virtual ~EuroplexusInvalidDimension() noexcept;

   private:
    EuroplexusInvalidDimension();
    EuroplexusInvalidDimension& operator=(const EuroplexusInvalidDimension&);
  };  // end of struct EuroplexusInvalidDimension

}  // end of namespace epx

#endif /* LIB_MFRONT_EPXEXCEPTION_H_ */
