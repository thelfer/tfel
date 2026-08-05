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
    explicit DianaFEAException(std::string);
    //! move constructor
    DianaFEAException(DianaFEAException&&) noexcept;
    //! copy constructor
    DianaFEAException(const DianaFEAException&);
    //
    DianaFEAException() = delete;
    DianaFEAException& operator=(const DianaFEAException&) = delete;
    DianaFEAException& operator=(DianaFEAException&&) = delete;
    //
    [[nodiscard]] const char* what() const noexcept final;
    [[nodiscard]] virtual const std::string& getMsg() const noexcept final;
    //! destructor
    ~DianaFEAException() noexcept override;

   private:
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
    explicit DianaFEAInvalidModellingHypothesis(const char*);
    //! move constructor
    DianaFEAInvalidModellingHypothesis(
        DianaFEAInvalidModellingHypothesis&&) noexcept;
    //! copy constructor
    DianaFEAInvalidModellingHypothesis(
        const DianaFEAInvalidModellingHypothesis&);
    //
    DianaFEAInvalidModellingHypothesis& operator=(
        const DianaFEAInvalidModellingHypothesis&) = delete;
    //! \brief destructor
    ~DianaFEAInvalidModellingHypothesis() noexcept override;
  };  // end of struct DianaFEAInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAInvalidNTENSValue final
      : public DianaFEAException {
    explicit DianaFEAInvalidNTENSValue(const unsigned short);
    //! move constructor
    DianaFEAInvalidNTENSValue(DianaFEAInvalidNTENSValue&&) noexcept;
    //! copy constructor
    DianaFEAInvalidNTENSValue(const DianaFEAInvalidNTENSValue&);
    //
    DianaFEAInvalidNTENSValue() = delete;
    DianaFEAInvalidNTENSValue& operator=(const DianaFEAInvalidNTENSValue&) =
        delete;
    //! desctructor
    ~DianaFEAInvalidNTENSValue() noexcept override;
  };  // end of struct DianaFEAInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_DIANAFEA_VISIBILITY_EXPORT DianaFEAInvalidDimension final
      : public DianaFEAException {
    DianaFEAInvalidDimension(const std::string&, const unsigned short);
    //! move constructor
    DianaFEAInvalidDimension(DianaFEAInvalidDimension&&) noexcept;
    //! copy constructor
    DianaFEAInvalidDimension(const DianaFEAInvalidDimension&);
    //
    DianaFEAInvalidDimension() = delete;
    DianaFEAInvalidDimension& operator=(const DianaFEAInvalidDimension&) =
        delete;
    //! destructor
    ~DianaFEAInvalidDimension() noexcept override;
  };  // end of struct DianaFEAInvalidDimension

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEAEXCEPTION_HXX */
