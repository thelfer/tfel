/*!
 * \file  mfront/include/MFront/Ansys/AnsysException.hxx
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

#ifndef LIB_MFRONT_ANSYSEXCEPTION_HXX
#define LIB_MFRONT_ANSYSEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/Ansys/AnsysConfig.hxx"

namespace ansys {

  /*!
   * \brief base class for all exceptions that are thrown in the
   * Ansys or Ansys/Explicit interfaces
   */
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysException : public std::exception {
    //! \brief constructor
    AnsysException(std::string);
    //! \brief move constructor
    AnsysException(AnsysException&&);
    //! \brief copy constructor
    AnsysException(const AnsysException&);

    const char* what() const noexcept override final;
    virtual std::string getMsg() const noexcept final;
    //! \brief destructor
    ~AnsysException() noexcept override;

   private:
    AnsysException() = delete;
    AnsysException& operator=(const AnsysException&) = delete;
    AnsysException& operator=(AnsysException&&) = delete;
    //! \brief error message
    const std::string msg;
  };  // end of struct AnsysException
  /*!
   * \brief exception thrown when an invalid modelling hypothesis is
   * to be used
   */
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysInvalidModellingHypothesis final
      : public AnsysException {
    /*!
     * \param[in] b : behaviour name
     */
    AnsysInvalidModellingHypothesis(const char*);
    //! \brief move constructor
    AnsysInvalidModellingHypothesis(AnsysInvalidModellingHypothesis&&);
    //! \brief copy constructor
    AnsysInvalidModellingHypothesis(const AnsysInvalidModellingHypothesis&);
    //! \brief destructor
    ~AnsysInvalidModellingHypothesis() noexcept override;

   private:
    AnsysInvalidModellingHypothesis& operator=(
        const AnsysInvalidModellingHypothesis&) = delete;
  };  // end of struct AnsysInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysInvalidNTENSValue final
      : public AnsysException {
    AnsysInvalidNTENSValue(const unsigned short);
    //! \brief move constructor
    AnsysInvalidNTENSValue(AnsysInvalidNTENSValue&&);
    //! \brief copy constructor
    AnsysInvalidNTENSValue(const AnsysInvalidNTENSValue&);
    //! \brief desctructor
    ~AnsysInvalidNTENSValue() noexcept override;

   private:
    AnsysInvalidNTENSValue() = delete;
    AnsysInvalidNTENSValue& operator=(const AnsysInvalidNTENSValue&) = delete;
  };  // end of struct AnsysInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_ANSYS_VISIBILITY_EXPORT AnsysInvalidDimension final
      : public AnsysException {
    AnsysInvalidDimension(const std::string&, const unsigned short);
    //! \brief move constructor
    AnsysInvalidDimension(AnsysInvalidDimension&&);
    //! \brief copy constructor
    AnsysInvalidDimension(const AnsysInvalidDimension&);
    //! \brief destructor
    ~AnsysInvalidDimension() noexcept override;

   private:
    AnsysInvalidDimension() = delete;
    AnsysInvalidDimension& operator=(const AnsysInvalidDimension&) = delete;
  };  // end of struct AnsysInvalidDimension

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYSEXCEPTION_HXX */
