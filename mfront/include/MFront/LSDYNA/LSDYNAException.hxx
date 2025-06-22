/*!
 * \file  mfront/include/MFront/LSDYNA/LSDYNAException.hxx
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

#ifndef LIB_MFRONT_LSDYNAEXCEPTION_HXX
#define LIB_MFRONT_LSDYNAEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/LSDYNA/LSDYNAConfig.hxx"

namespace lsdyna {

  /*!
   * \brief base class for all exceptions that are thrown in the
   * LSDYNA or LSDYNA/Explicit interfaces
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAException
      : public std::exception {
    LSDYNAException(const std::string&);
    //! move constructor
    LSDYNAException(LSDYNAException&&);
    //! copy constructor
    LSDYNAException(const LSDYNAException&);
    const char* what() const noexcept override final;
    virtual std::string getMsg() const noexcept final;
    //! destructor
    ~LSDYNAException() noexcept override;

   private:
    LSDYNAException() = delete;
    LSDYNAException& operator=(const LSDYNAException&) = delete;
    LSDYNAException& operator=(LSDYNAException&&) = delete;
    //! error message
    const std::string msg;
  };  // end of struct LSDYNAException
  /*!
   * \brief exception thrown when an invalid modelling hypothesis is
   * to be used
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAInvalidModellingHypothesis
      : public LSDYNAException {
    /*!
     * \param[in] b : behaviour name
     */
    LSDYNAInvalidModellingHypothesis(const char*);
    //! move constructor
    LSDYNAInvalidModellingHypothesis(LSDYNAInvalidModellingHypothesis&&);
    //! copy constructor
    LSDYNAInvalidModellingHypothesis(const LSDYNAInvalidModellingHypothesis&);
    //! destructor
    virtual ~LSDYNAInvalidModellingHypothesis() noexcept;
    LSDYNAInvalidModellingHypothesis& operator=(
        const LSDYNAInvalidModellingHypothesis&) = delete;
  };  // end of struct LSDYNAInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAInvalidNTENSValue
      : public LSDYNAException {
    LSDYNAInvalidNTENSValue(const unsigned short);
    //! move constructor
    LSDYNAInvalidNTENSValue(LSDYNAInvalidNTENSValue&&);
    //! copy constructor
    LSDYNAInvalidNTENSValue(const LSDYNAInvalidNTENSValue&);
    //! desctructor
    virtual ~LSDYNAInvalidNTENSValue() noexcept;

   private:
    LSDYNAInvalidNTENSValue() = delete;
    LSDYNAInvalidNTENSValue& operator=(const LSDYNAInvalidNTENSValue&) = delete;
  };  // end of struct LSDYNAInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAInvalidDimension
      : public LSDYNAException {
    LSDYNAInvalidDimension(const std::string&, const unsigned short);
    //! move constructor
    LSDYNAInvalidDimension(LSDYNAInvalidDimension&&);
    //! copy constructor
    LSDYNAInvalidDimension(const LSDYNAInvalidDimension&);
    //! destructor
    virtual ~LSDYNAInvalidDimension() noexcept;

   private:
    LSDYNAInvalidDimension() = delete;
    LSDYNAInvalidDimension& operator=(const LSDYNAInvalidDimension&) = delete;
  };  // end of struct LSDYNAInvalidDimension

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNAEXCEPTION_HXX */
