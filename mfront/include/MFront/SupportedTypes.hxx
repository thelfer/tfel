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
#include <array>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <string_view>
#include "TFEL/Utilities/CxxTokenizer.hxx"
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
    //! \brief flag associaed with a variable type
    enum TypeFlag { SCALAR, TVECTOR, STENSOR, TENSOR };
    /*!
     * \brief class handling the size of a variable or a set of variables as
     * a function of the variables' type and the modelling hypothesis
     */
    struct MFRONT_VISIBILITY_EXPORT TypeSize {
      //! \brief a simple alias
      using ModellingHypothesis = tfel::material::ModellingHypothesis;
      //! \brief a simple alias
      using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
      /*!
       * \brief constructor
       * \param[in] i1: vector exponent
       * \param[in] i2: symmetric tensor exponent
       * \param[in] i3: (unsymmetric) tensor exponent
       *
       * \note if the three exponents i1, i2 and i3 are null, a scalar is
       * described.
       */
      static TypeSize buildFromExponents(const unsigned int,
                                         const unsigned int,
                                         const unsigned int);
      /*!
       * \return the type size of a derivative of a variable of size `s1`
       * with respect to a variable of size `s2`.
       * \param[in] s1: size of the first variable
       * \param[in] s2: size of the second variable
       * \note s1 and s2 must not describe describe a unique type
       * (`describesAnUniqueType` must return true) and not an array (`isArray`
       * must return false).
       */
      static TypeSize getDerivativeSize(const TypeSize&, const TypeSize&);
      /*!
       * \return the type size of a derivative of a variable of size `s1`
       * with respect to a variable of size `s2`.
       * \param[in] a: array size
       * \param[in] s1: size of the first variable
       * \param[in] s2: size of the second variable
       * \note s1 and s2 must not describe describe a unique type
       * (`describesAnUniqueType` must return true) and not an array (`isArray`
       * must return false).
       */
      static TypeSize getDerivativeSize(const unsigned int a,
                                        const TypeSize&,
                                        const TypeSize&);
      //! \brief constructor
      TypeSize();
      //! \brief move constructor
      TypeSize(TypeSize&&);
      //! \brief copy constructor
      TypeSize(const TypeSize&);
      /*!
       * \brief constructor
       * \param[in] a: array size
       * \param[in] f: type flag
       * \param[in] e: type exponent
       */
      TypeSize(const TypeFlag, const unsigned int = 1u);
      /*!
       * \brief constructor
       * \param[in] a: array size
       * \param[in] f: type flag
       * \param[in] e: type exponent
       */
      TypeSize(const unsigned int, const TypeFlag, const unsigned int = 1u);
      //! \brief assignement operator
      TypeSize& operator=(const TypeSize&);
      //! \brief move assignement operator
      TypeSize& operator=(TypeSize&&);
      /*!
       * \brief plus-equal operator
       * \param[in] rhs: right hand side
       */
      TypeSize& operator+=(const TypeSize&);
      /*!
       * \brief minus-equal operator
       * \param[in] rhs: right hand side
       */
      TypeSize& operator-=(const TypeSize&);
      //! \brief comparision operator
      bool operator!=(const TypeSize&) const;
      //! \brief comparision operator
      bool operator==(const TypeSize&) const;

      /*!
       * \return a string representation of the type size.
       *
       * - The size of a tiny vector is `TVectorSize`.
       * - The size of a symmetric tensor is `StensorSize`.
       * - The size of a (unsymmetric) tensor is `TensorSize`.
       */
      std::string asString() const;
      /*!
       * \brief return a string representation of the type size given
       * the string representation of a tiny vector, a symmetric tensor and a
       * non symmetric tensor.
       * \param[in] values: string representations of a tiny vector, a symmetric
       * tensor and a non symmetric tensor
       */
      std::string asString(const std::array<std::string, 3u>&) const;
      /*!
       *
       */
      int getValueForDimension(const unsigned short) const;
      /*!
       *
       */
      int getValueForModellingHypothesis(const Hypothesis) const;
      //! \return true if all components of the TipeSize are null
      bool isNull() const;
      /*!
       * \return true if only the scalar component is not null and is
       * equal to one
       */
      bool isOne() const;
      //! \return true if the type size describe an array of an unique type
      bool isArray() const;
      /*!
       * \brief multiply the array size by the given parameter
       * \param[in] a: array size
       */
      TypeSize& operator*=(const unsigned int);

     private:
      /*!
       * \brief constructor
       * \param[in] a: array size
       * \param[in] i1: vector exponent
       * \param[in] i2: symmetric tensor exponent
       * \param[in] i3: (unsymmetric) tensor exponent
       *
       * \note if the three exponents i1, i2 and i3 are null, a scalar is
       * described.
       */
      TypeSize(const unsigned int,
               const unsigned int,
               const unsigned int,
               const unsigned int);
      /*!
       * \brief structure representing the size of an array of a unique type
       */
      struct Monomial {
        //! \brief scalar exponent
        int array_size = 0;
        //! \brief vector exponent
        unsigned int tvector_exponent = 0;
        //! \brief symmetric tensor exponent
        unsigned int stensor_exponent = 0;
        //! \brie unsymmetric tensor exponent
        unsigned int tensor_exponent = 0;
      };
      /*!
       * \return true if two monomials match (i.e. both monomials have the
       * same exponents but not necessarily the same array sizes)
       * \param[in] m1: first monomial
       * \param[in] m2: second monomial
       */
      static bool matches(const Monomial& m1, const Monomial& m2);
      /*!
       * \return true if the type size is only described by an unique monomial.
       */
      bool isMonomial() const;
      /*!
       * \return an iterator to a matching monomial (i.e. a monomial with
       * the same exponent but not necessarily the same array size) if any
       * or an iterator past the last monomial otherwise.
       *
       * \param[in] m: monomial
       */
      std::vector<Monomial>::iterator findMatchingMonomial(const Monomial&);
      /*!
       * \return an iterator to a matching monomial (i.e. a monomial with the
       * same exponent but not necessarily the same array size) if any or an
       * iterator past the last monomial otherwise.
       *
       * \param[in] m: monomial
       */
      std::vector<Monomial>::const_iterator findMatchingMonomial(
          const Monomial&) const;

      //!  \brief list of monomials
      std::vector<Monomial> monomials;

      //
      friend std::ostream& operator<<(std::ostream&, const TypeSize&);
    };  // end of struct SupportedTypes::TypeSize

    /*!
     * \brief a small structure which allows to configure how type can be
     * parsed.
     */
    struct MFRONT_VISIBILITY_EXPORT TypeParsingOptions {
      TypeParsingOptions();
      TypeParsingOptions(TypeParsingOptions&&);
      TypeParsingOptions(const TypeParsingOptions&);
      TypeParsingOptions& operator=(TypeParsingOptions&&);
      TypeParsingOptions& operator=(const TypeParsingOptions&);
      ~TypeParsingOptions();
      /*!
       * \brief integer constants are static variables with the `int` type whose
       * size is known. Integer constants are replaced by their values during
       * the type evaluation.
       */
      std::map<std::string, int> integer_constants;
      /*!
       * \brief boolean stating if the usage of quantities is allowed
       */
      bool use_qt = true;
    };

    //! \return a list of type names associated with type flags
    static const std::map<std::string, TypeFlag, std::less<>>& getTypeFlags();
    //! \brief a simple alias
    using const_iterator = tfel::utilities::CxxTokenizer::const_iterator;
    /*!
     * \return if the given value designates one of the supported  constexpr
     * integer variables, i.e. `N`, `TVectorSize`, `StensorSize`, `TensorSize`.
     * \param[in] v: value
     */
    static bool isSupportedConstexprIntegerVariables(const std::string_view);
    /*!
     * \return a normalized representation of a type read from a list of tokens.
     * \param[in] current: current position.
     * \param[in] end: iterator past the last token.
     * \param[in] opts: parsing options
     */
    static std::string extractType(
        const_iterator&,
        const const_iterator&,
        const TypeParsingOptions& = TypeParsingOptions());
    /*!
     * \return a normalized representation of a type read from a string.
     * \param[in] s: string.
     * \param[in] opts: parsing options
     */
    static std::string extractType(
        const std::string_view,
        const TypeParsingOptions& = TypeParsingOptions());
    /*!
     * \return if the given type is associated with a flag
     * \param[in] t : type
     */
    static bool hasTypeFlag(const std::string_view);
    /*!
     * \return if the given type describes a scalar
     * \param[in] t : type
     */
    static bool isScalarType(const std::string_view);
    /*!
     * \return the flag associated with the given type
     * \param[in] t : type
     */
    static TypeFlag getTypeFlag(const std::string_view);
    /*!
     * \return a integer encoding the type of a tensorial variable
     * \param[in] t : type
     *
     * Those integers shall be intepreted as follows:
     *
     * - the three first bits, converted in an integer, gives the type of
     *   variable:
     *   - `0` denotes a scalar
     *   - `1` denotes a symmetric tensor
     *   - `2` denotes a vector
     *   - `3` denotes an unsymmetric tensor
     *   - `4` denotes a derivative function
     * - For tensorial objects, the two next bits, converted in an integer,
     *   gives the space dimension of the variable:
     *   - `0` indicates that the space dimension depends on the modelling
     *     hypothesis considered.
     *   - `1` indicates that the object has a space dimension of `1`,
     *     indepently of the modelling hypothesis considered.
     *   - `2` indicates that the object has a space dimension of `2`,
     *     indepently of the modelling hypothesis considered.
     *   - `3` indicates that the object has a space dimension of `3`,
     *     indepently of the modelling hypothesis considered.
     * - Concerning a derivative function, the next bits encodes the type of
     *   the function and the bits after the type of the variables with respect
     *   to which the function is derivated.
     */
    static int getTypeIdentifier(const std::string_view);
    /*!
     * \return the size of a type (i.e. the number of values hold)
     * \note this method is only meaningful for TFEL mathematical objects.
     * \param[in] t : variable type
     * \param[in] a : array size
     */
    static TypeSize getTypeSize(const std::string_view,
                                const unsigned short = 1);
    //! \return the time derivative of a variable
    static std::string getTimeDerivativeType(const std::string_view);
    //! \brief default constructor
    SupportedTypes();
    //! \brief move constructor
    SupportedTypes(SupportedTypes&&) = default;
    //! \brief copy constructor
    SupportedTypes(const SupportedTypes&) = default;
    //! \brief move assignement
    SupportedTypes& operator=(SupportedTypes&&) = default;
    //! \brief copy assignement
    SupportedTypes& operator=(const SupportedTypes&) = default;
    //! \brief desctructor
    virtual ~SupportedTypes();

   private:
    /*!
     * \brief a simple structure which represents a C++ type
     */
    struct TypeInformation {
      //! \brief a simple alias
      using IntegerTemplateArgument = std::variant<int, std::string>;
      //! \brief a simple alias
      using TemplateArgument =
          std::variant<TypeInformation, IntegerTemplateArgument>;
      //! \brief base type
      std::string type;
      //! \brief list of template arguments
      std::optional<std::vector<TemplateArgument>> template_arguments;
    };
    /*!
     * \return if the given type describes a scalar
     * \param[in] t : type
     */
    static bool isScalarType(const TypeInformation&);
    /*!
     * \return the flag associated with the given type
     * \param[in] t : type
     */
    static std::optional<TypeFlag> getTypeFlag(const TypeInformation&);
    /*!
     * \return a integer encoding the type of a tensorial variable and the
     * number of bits used for the encoding.
     * \param[in] t : type
     */
    static std::pair<int, int> getTypeIdentifier(const TypeInformation&);
    /*!
     * \return the size of a type (i.e. the number of values hold)
     * \param[in] t : type information
     * \param[in] a : array size
     */
    static TypeSize getTypeSize(const TypeInformation&, const unsigned short);
    //
    static void normalize(TypeInformation&, const TypeParsingOptions&);
    static bool matchesTFELMathType(const std::string_view, const std::string&);
    static void normalizeRawScalarType(TypeInformation&);
    static void normalizeQuantity(TypeInformation&, const TypeParsingOptions&);
    static void normalizeScalarType(TypeInformation&,
                                    const TypeParsingOptions&);
    static bool checkIntegerTemplateArgument(
        const TypeInformation::TemplateArgument&);
    static void normalizeTinyVectorTemplateArguments(TypeInformation&,
                                                     const TypeParsingOptions&);
    static void normalizeTinyMatrixTemplateArguments(TypeInformation&,
                                                     const TypeParsingOptions&);
    /*!
     * \brief check that the number of template arguments is the expected one
     * \param[in] t: type information
     * \param[in] n: expected number of template arguments
     */
    static void checkNumberOfTemplateArguments(const TypeInformation&,
                                               const std::size_t);
    /*!
     * \return the number of values hold by an object or the space dimension
     * \param[in] size: integer template argument
     */
    static int getSizeOrSpaceDimension(
        const TypeInformation::IntegerTemplateArgument&);
    /*!
     * \return the space dimension of the tensorial object (0 if it depends on
     * the context of instanciation, 1 for an 1D tensor, 2 for a 2D tensor, 3
     * for a 3D tensor)
     * \param[in] t: type description
     */
    static int getTensorialObjectSpaceDimension(const TypeInformation&);
    /*!
     * \brief analyse the template arguments of a tensorial object
     * \param[in] t: type information
     * \param[in] opts: parsing options
     */
    static void normalizeTensorialTypeTemplateArguments(
        TypeInformation&, const TypeParsingOptions&);
    static std::vector<std::string> getSupportedTFELMathTensorialTypes();
    static std::string encode(const TypeInformation&);
    /*!
     * \return the information about a type
     * \param[in] t: type
     * \param[in] opts: type parsing options
     */
    static TypeInformation getTypeInformation(const std::string_view,
                                              const TypeParsingOptions&);
    /*!
     * \brief extract a type from tokens
     * \return a pair whose for element is a type information and the second
     * element a boolean. This boolean is false if a template argument list
     * must be closed.
     * \param[in] current: current position.
     * \param[in] end: iterator past the last token.
     * \param[in] opts: type parsing options
     */
    static std::pair<TypeInformation, bool> parseType(
        const_iterator&, const const_iterator&, const TypeParsingOptions&);
    /*!
     * \brief check if given iterator is after the last valid iterator
     * \param[in] c: current position
     * \param[in] e: iterator past the last token.
     */
    static void checkIteratorValidity(const const_iterator,
                                      const const_iterator&);
    /*!
     * \brief check if the given iterator points to a token with the given
     * value and increments the iterator. \param[in] c: current position.
     * \param[in] e: iterator past the last token.
     * \param[in] v: expected value.
     */
    static void checkCurrentValueAndIncrementIterator(const_iterator&,
                                                      const const_iterator&,
                                                      const std::string_view);
    /*!
     * \return an optional value which contains the value of the integer
     * represented by the given string, if the string contains an integer.
     * \param[in] s: string
     */
    static std::optional<int> extractInteger(const std::string_view);
  };  // end of struct SupportedTypes

  /*!
   * \brief convert to string
   * \param[in] s: type size
   */
  MFRONT_VISIBILITY_EXPORT std::string to_string(
      const SupportedTypes::TypeSize&);
  /*!
   * \brief multiply a type size by a scalar
   * \param[in] a: factor
   * \param[in] s: type size
   */
  MFRONT_VISIBILITY_EXPORT SupportedTypes::TypeSize operator*(
      const SupportedTypes::TypeSize&, const unsigned int);
  /*!
   * \brief multiply a type size by a scalar
   * \param[in] a: factor
   * \param[in] s: type size
   */
  MFRONT_VISIBILITY_EXPORT SupportedTypes::TypeSize operator*(
      const unsigned int, const SupportedTypes::TypeSize&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_SUPPORTEDTYPES_HXX */
