/*!
 * \file  mfront/include/MFront/DianaFEA/DianaFEAException.hxx
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

#ifndef LIB_MFRONT_DIANAFEAEXCEPTION_HXX
#define LIB_MFRONT_DIANAFEAEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/DianaFEA/DianaFEAConfig.hxx"

namespace dianafea {

  /*!
   * \brief base class for all exceptions that are thrown in the
   * DianaFEA or DianaFEA/Explicit interfaces
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAException
      : public std::exception {
    DianaFEAException(const std::string&);
    //! move constructor
    DianaFEAException(DianaFEAException&&);
    //! copy constructor
    DianaFEAException(const DianaFEAException&);

    const char* what() const noexcept override final;
    virtual std::string getMsg() const noexcept final;
    //! destructor
    ~DianaFEAException() noexcept override;

   private:
    DianaFEAException() = delete;
    DianaFEAException& operator=(const DianaFEAException&) = delete;
    DianaFEAException& operator=(DianaFEAException&&) = delete;
    //! error message
    const std::string msg;
  };  // end of struct DianaFEAException
  /*!
   * \brief exception thrown when an invalid modelling hypothesis is
   * to be used
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAInvalidModellingHypothesis
      final : public DianaFEAException {
    /*!
     * \param[in] b : behaviour name
     */
    DianaFEAInvalidModellingHypothesis(const char*);
    //! move constructor
    DianaFEAInvalidModellingHypothesis(DianaFEAInvalidModellingHypothesis&&);
    //! copy constructor
    DianaFEAInvalidModellingHypothesis(
        const DianaFEAInvalidModellingHypothesis&);
    //! destructor
    ~DianaFEAInvalidModellingHypothesis() noexcept override;

   private:
    DianaFEAInvalidModellingHypothesis& operator=(
        const DianaFEAInvalidModellingHypothesis&) = delete;
  };  // end of struct DianaFEAInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAInvalidNTENSValue final
      : public DianaFEAException {
    DianaFEAInvalidNTENSValue(const unsigned short);
    //! move constructor
    DianaFEAInvalidNTENSValue(DianaFEAInvalidNTENSValue&&);
    //! copy constructor
    DianaFEAInvalidNTENSValue(const DianaFEAInvalidNTENSValue&);
    //! desctructor
    ~DianaFEAInvalidNTENSValue() noexcept override;

   private:
    DianaFEAInvalidNTENSValue() = delete;
    DianaFEAInvalidNTENSValue& operator=(const DianaFEAInvalidNTENSValue&) =
        delete;
  };  // end of struct DianaFEAInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAInvalidDimension final
      : public DianaFEAException {
    DianaFEAInvalidDimension(const std::string&, const unsigned short);
    //! move constructor
    DianaFEAInvalidDimension(DianaFEAInvalidDimension&&);
    //! copy constructor
    DianaFEAInvalidDimension(const DianaFEAInvalidDimension&);
    //! destructor
    ~DianaFEAInvalidDimension() noexcept override;

   private:
    DianaFEAInvalidDimension() = delete;
    DianaFEAInvalidDimension& operator=(const DianaFEAInvalidDimension&) =
        delete;
  };  // end of struct DianaFEAInvalidDimension

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEAEXCEPTION_HXX */
