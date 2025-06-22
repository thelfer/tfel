/*!
 * \file   mfront/include/MFront/SupportedTypes.hxx
 * \brief  This file declares the SupportedTypes class
 * \author Thomas Helfer
 * \date   12 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_SUPPORTEDTYPES_HXX
#define LIB_MFRONT_SUPPORTEDTYPES_HXX

#include <map>
#include <string>
#include <vector>

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  //! class handling all type variables types supported by MFront
  struct MFRONT_VISIBILITY_EXPORT SupportedTypes {
    //! limit for small array support
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    static const int ArraySizeLimit = 10u;
#else
    static constexpr int ArraySizeLimit = 10u;
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_HXX */
    //! type of variable supported
    enum TypeFlag { Scalar, TVector, Stensor, Tensor };
    /*!
     * class handling the size of a variable or a set of variables as
     * a function of the variables' type and the modelling hypothesis
     */
    struct MFRONT_VISIBILITY_EXPORT TypeSize {
      //! a simple alias
      using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
      //! constructor
      TypeSize();
      //! move constructor
      TypeSize(TypeSize&&);
      //! copy constructor
      TypeSize(const TypeSize&);
      /*!
       * \brief constructor
       * \param[in] a: scalar size
       * \param[in] b: vector size
       * \param[in] c: symmetric tensor size
       * \param[in] d: (unsymmetric) tensor size
       */
      TypeSize(const int, const int, const int, const int);
      //! assignement
      TypeSize& operator=(const TypeSize&);
      //! move assignement
      TypeSize& operator=(TypeSize&&);
      /*!
       * plus-equal operator
       * \param[in] rhs: right hand side
       */
      TypeSize& operator+=(const TypeSize&);
      /*!
       * minus-equal operator
       * \param[in] rhs: right hand side
       */
      TypeSize& operator-=(const TypeSize&);
      //! comparision operator
      bool operator!=(const TypeSize&) const;
      //! comparision operator
      bool operator==(const TypeSize&) const;
      //! return the scalar part of the size
      int getScalarSize() const;
      //! return the vector part of the size
      int getTVectorSize() const;
      //! return the symmetric tensor part of the size
      int getStensorSize() const;
      //! return the (un)symmetric tensor part of the size
      int getTensorSize() const;

      int getValueForDimension(const unsigned short) const;

      int getValueForModellingHypothesis(const Hypothesis) const;
      //! \return true if all components of the TipeSize are null
      bool isNull() const;
      /*!
       * \return true if only the scalar component is not null and is
       * equal to one
       */
      bool isOne() const;

     private:
      //! ouptut operator
      friend std::ostream& operator<<(std::ostream&, const TypeSize&);
      //! scalar part
      int scalarSize = 0;
      //! vector part
      int tvectorSize = 0;
      //! symmetric tensor part
      int stensorSize = 0;
      //! (un)symmetric tensor part
      int tensorSize = 0;
    };  // end of class SupportedTypes::TypeSize
    //! \return a list of type names associated with type flags
    static const std::map<std::string, TypeFlag>& getTypeFlags();
    /*!
     * \return the flag associated with the given type
     * \param[in] t : type
     */
    static TypeFlag getTypeFlag(const std::string&);
    //! default constructor
    SupportedTypes();
    //! move constructor
    SupportedTypes(SupportedTypes&&) = default;
    //! copy constructor
    SupportedTypes(const SupportedTypes&) = default;
    //! move assignement
    SupportedTypes& operator=(SupportedTypes&&) = default;
    //! copy assignement
    SupportedTypes& operator=(const SupportedTypes&) = default;
    /*!
     * \return true if the given type is supported
     * \param[in] t : type
     */
    bool isSupportedType(const std::string&) const;
    //! return a map associating a supported type to is type flag
    /*!
     * \param[in] t : variable type
     * \param[in] a : array size
     */
    static TypeSize getTypeSize(const std::string&, const unsigned short);

    std::string getTimeDerivativeType(const std::string&) const;
    //! desctructor
    virtual ~SupportedTypes();

  };  // end of class SupportedTypes

  /*!
   * \brief convert to string
   * \param[in] s: type size
   */
  MFRONT_VISIBILITY_EXPORT std::string to_string(
      const SupportedTypes::TypeSize&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_SUPPORTEDTYPES_HXX */
