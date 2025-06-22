/*!
 * \file  mfront/include/MFront/CalculiX/CalculiXException.hxx
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

#ifndef LIB_MFRONT_CALCULIXEXCEPTION_HXX
#define LIB_MFRONT_CALCULIXEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/CalculiX/CalculiXConfig.hxx"

namespace calculix {

  /*!
   * \brief base class for all exceptions that are thrown in the
   * CalculiX or CalculiX/Explicit interfaces
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXException
      : public std::exception {
    CalculiXException(const std::string&);
    //! move constructor
    CalculiXException(CalculiXException&&);
    //! copy constructor
    CalculiXException(const CalculiXException&);

    virtual const char* what() const noexcept override final;

    virtual std::string getMsg() const noexcept final;
    //! destructor
    virtual ~CalculiXException() noexcept;

   private:
    CalculiXException() = delete;
    CalculiXException& operator=(const CalculiXException&) = delete;
    CalculiXException& operator=(CalculiXException&&) = delete;
    //! error message
    const std::string msg;
  };  // end of struct CalculiXException
  /*!
   * \brief exception thrown when an invalid modelling hypothesis is
   * to be used
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXInvalidModellingHypothesis
      : public CalculiXException {
    /*!
     * \param[in] b : behaviour name
     */
    CalculiXInvalidModellingHypothesis(const char*);
    //! move constructor
    CalculiXInvalidModellingHypothesis(CalculiXInvalidModellingHypothesis&&);
    //! copy constructor
    CalculiXInvalidModellingHypothesis(
        const CalculiXInvalidModellingHypothesis&);
    //! destructor
    virtual ~CalculiXInvalidModellingHypothesis() noexcept;
    CalculiXInvalidModellingHypothesis& operator=(
        const CalculiXInvalidModellingHypothesis&) = delete;
  };  // end of struct CalculiXInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXInvalidNTENSValue
      : public CalculiXException {
    CalculiXInvalidNTENSValue(const unsigned short);
    //! move constructor
    CalculiXInvalidNTENSValue(CalculiXInvalidNTENSValue&&);
    //! copy constructor
    CalculiXInvalidNTENSValue(const CalculiXInvalidNTENSValue&);
    //! desctructor
    virtual ~CalculiXInvalidNTENSValue() noexcept;

   private:
    CalculiXInvalidNTENSValue() = delete;
    CalculiXInvalidNTENSValue& operator=(const CalculiXInvalidNTENSValue&) =
        delete;
  };  // end of struct CalculiXInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXInvalidDimension
      : public CalculiXException {
    CalculiXInvalidDimension(const std::string&, const unsigned short);
    //! move constructor
    CalculiXInvalidDimension(CalculiXInvalidDimension&&);
    //! copy constructor
    CalculiXInvalidDimension(const CalculiXInvalidDimension&);
    //! destructor
    virtual ~CalculiXInvalidDimension() noexcept;

   private:
    CalculiXInvalidDimension() = delete;
    CalculiXInvalidDimension& operator=(const CalculiXInvalidDimension&) =
        delete;
  };  // end of struct CalculiXInvalidDimension

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXEXCEPTION_HXX */
