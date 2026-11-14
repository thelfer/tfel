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
    explicit LSDYNAException(std::string);
    //! \brief move constructor
    LSDYNAException(LSDYNAException&&) noexcept;
    //! \brief copy constructor
    LSDYNAException(const LSDYNAException&);
    //
    LSDYNAException() = delete;
    LSDYNAException& operator=(const LSDYNAException&) = delete;
    LSDYNAException& operator=(LSDYNAException&&) = delete;
    //
    [[nodiscard]] const char* what() const noexcept final;
    [[nodiscard]] virtual const std::string& getMsg() const noexcept final;
    //! \brief destructor
    ~LSDYNAException() noexcept override;

   private:
    //! \brief error message
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
    explicit LSDYNAInvalidModellingHypothesis(const char*);
    //! \brief move constructor
    LSDYNAInvalidModellingHypothesis(
        LSDYNAInvalidModellingHypothesis&&) noexcept;
    //! \brief copy constructor
    LSDYNAInvalidModellingHypothesis(const LSDYNAInvalidModellingHypothesis&);
    //! \brief destructor
    ~LSDYNAInvalidModellingHypothesis() noexcept override;
    LSDYNAInvalidModellingHypothesis& operator=(
        const LSDYNAInvalidModellingHypothesis&) = delete;
  };  // end of struct LSDYNAInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAInvalidNTENSValue
      : public LSDYNAException {
    explicit LSDYNAInvalidNTENSValue(const unsigned short);
    //! \brief move constructor
    LSDYNAInvalidNTENSValue(LSDYNAInvalidNTENSValue&&) noexcept;
    //! \brief copy constructor
    LSDYNAInvalidNTENSValue(const LSDYNAInvalidNTENSValue&);
    //
    LSDYNAInvalidNTENSValue() = delete;
    LSDYNAInvalidNTENSValue& operator=(const LSDYNAInvalidNTENSValue&) = delete;
    //! \brief destructor
    ~LSDYNAInvalidNTENSValue() noexcept override;
  };  // end of struct LSDYNAInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_LSDYNA_VISIBILITY_EXPORT LSDYNAInvalidDimension
      : public LSDYNAException {
    LSDYNAInvalidDimension(const std::string&, const unsigned short);
    //! \brief move constructor
    LSDYNAInvalidDimension(LSDYNAInvalidDimension&&) noexcept;
    //! \brief copy constructor
    LSDYNAInvalidDimension(const LSDYNAInvalidDimension&);
    //
    LSDYNAInvalidDimension() = delete;
    LSDYNAInvalidDimension& operator=(const LSDYNAInvalidDimension&) = delete;
    //! \brief destructor
    ~LSDYNAInvalidDimension() noexcept override;
  };  // end of struct LSDYNAInvalidDimension

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNAEXCEPTION_HXX */
