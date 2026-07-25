/*!
 * \file  mfront/include/MFront/Abaqus/AbaqusException.hxx
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

#ifndef LIB_MFRONT_ABAQUSEXCEPTION_HXX
#define LIB_MFRONT_ABAQUSEXCEPTION_HXX

#include <string>
#include <exception>

#include "MFront/Abaqus/AbaqusConfig.hxx"

namespace abaqus {

  /*!
   * \brief base class for all exceptions that are thrown in the
   * Abaqus or Abaqus/Explicit interfaces
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusException
      : public std::exception {
    AbaqusException(const std::string&);
    //! \brief move constructor
    AbaqusException(AbaqusException&&) noexcept;
    //! \brief copy constructor
    AbaqusException(const AbaqusException&);
    const char* what() const noexcept final;
    virtual const std::string& getMsg() const noexcept final;
    //! \brief destructor
    ~AbaqusException() noexcept override;

   private:
    AbaqusException() = delete;
    AbaqusException& operator=(const AbaqusException&) = delete;
    AbaqusException& operator=(AbaqusException&&) = delete;
    //! \brief error message
    const std::string msg;
  };  // end of struct AbaqusException
  /*!
   * \brief exception thrown when an invalid modelling hypothesis is
   * to be used
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInvalidModellingHypothesis
      : public AbaqusException {
    /*!
     * \param[in] b : behaviour name
     */
    AbaqusInvalidModellingHypothesis(const char*);
    //! \brief move constructor
    AbaqusInvalidModellingHypothesis(
        AbaqusInvalidModellingHypothesis&&) noexcept;
    //! \brief copy constructor
    AbaqusInvalidModellingHypothesis(const AbaqusInvalidModellingHypothesis&);
    //! \brief assignement operator
    AbaqusInvalidModellingHypothesis& operator=(
        const AbaqusInvalidModellingHypothesis&) = delete;
    //! \brief destructor
    ~AbaqusInvalidModellingHypothesis() noexcept override;
  };  // end of struct AbaqusInvalidModellingHypothesis
  /*!
   * \brief exception thrown when an invalid value of the *NTENS
   * parameter is given
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInvalidNTENSValue
      : public AbaqusException {
    AbaqusInvalidNTENSValue(const unsigned short);
    //! \brief move constructor
    AbaqusInvalidNTENSValue(AbaqusInvalidNTENSValue&&) noexcept;
    //! \brief copy constructor
    AbaqusInvalidNTENSValue(const AbaqusInvalidNTENSValue&);
    //! \brief desctructor
    ~AbaqusInvalidNTENSValue() noexcept override;

   private:
    AbaqusInvalidNTENSValue() = delete;
    AbaqusInvalidNTENSValue& operator=(const AbaqusInvalidNTENSValue&) = delete;
  };  // end of struct AbaqusInvalidNTENSValue
  /*!
   * \brief exception thrown when an invalid dimension is detected
   */
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInvalidDimension
      : public AbaqusException {
    AbaqusInvalidDimension(const std::string&, const unsigned short);
    //! \brief move constructor
    AbaqusInvalidDimension(AbaqusInvalidDimension&&) noexcept;
    //! \brief copy constructor
    AbaqusInvalidDimension(const AbaqusInvalidDimension&);
    //! \brief destructor
    ~AbaqusInvalidDimension() noexcept override;

   private:
    AbaqusInvalidDimension() = delete;
    AbaqusInvalidDimension& operator=(const AbaqusInvalidDimension&) = delete;
  };  // end of struct AbaqusInvalidDimension

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUSEXCEPTION_HXX */
