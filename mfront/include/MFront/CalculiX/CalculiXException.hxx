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
    explicit CalculiXException(std::string);
    //! \brief move constructor
    CalculiXException(CalculiXException&&) noexcept;
    //! \brief copy constructor
    CalculiXException(const CalculiXException&);
    //
    CalculiXException() = delete;
    CalculiXException& operator=(const CalculiXException&) = delete;
    CalculiXException& operator=(CalculiXException&&) = delete;
    //
    [[nodiscard]] const char* what() const noexcept final;
    [[nodiscard]] virtual const std::string& getMsg() const noexcept final;
    //! \brief destructor
    ~CalculiXException() noexcept override;

   private:
    //! \brief error message
    const std::string msg;
  };  // end of struct CalculiXException
  /*!
   * \brief exception thrown when an invalid modelling hypothesis is
   * to be used
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXInvalidModellingHypothesis
      final : public CalculiXException {
    /*!
     * \param[in] b : behaviour name
     */
    explicit CalculiXInvalidModellingHypothesis(const char*);
    //! \brief move constructor
    CalculiXInvalidModellingHypothesis(
        CalculiXInvalidModellingHypothesis&&) noexcept;
    //! \brief copy constructor
    CalculiXInvalidModellingHypothesis(
        const CalculiXInvalidModellingHypothesis&);
    //
    CalculiXInvalidModellingHypothesis& operator=(
        const CalculiXInvalidModellingHypothesis&) = delete;
    //! \brief destructor
    ~CalculiXInvalidModellingHypothesis() noexcept override;
  };  // end of struct CalculiXInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXInvalidNTENSValue final
      : public CalculiXException {
    explicit CalculiXInvalidNTENSValue(const unsigned short);
    //! \brief move constructor
    CalculiXInvalidNTENSValue(CalculiXInvalidNTENSValue&&) noexcept;
    //! \brief copy constructor
    CalculiXInvalidNTENSValue(const CalculiXInvalidNTENSValue&);
    //
    CalculiXInvalidNTENSValue() = delete;
    CalculiXInvalidNTENSValue& operator=(const CalculiXInvalidNTENSValue&) =
        delete;
    //! \brief desctructor
    ~CalculiXInvalidNTENSValue() noexcept override;
  };  // end of struct CalculiXInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXInvalidDimension final
      : public CalculiXException {
    CalculiXInvalidDimension(const std::string&, const unsigned short);
    //! \brief move constructor
    CalculiXInvalidDimension(CalculiXInvalidDimension&&) noexcept;
    //! \brief copy constructor
    CalculiXInvalidDimension(const CalculiXInvalidDimension&);
    //
    CalculiXInvalidDimension() = delete;
    CalculiXInvalidDimension& operator=(const CalculiXInvalidDimension&) =
        delete;
    //! \brief destructor
    ~CalculiXInvalidDimension() noexcept override;
  };  // end of struct CalculiXInvalidDimension

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXEXCEPTION_HXX */
