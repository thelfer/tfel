/*!
 * \file   mfront/include/MFront/SupportedTypes.hxx
 * \brief  This file declares the SupportedTypes class
 * \author Helfer Thomas
 * \date   12 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_SUPPORTEDTYPES_H_
#define LIB_MFRONT_SUPPORTEDTYPES_H_

#include <map>
#include <iosfwd>
#include <string>
#include <vector>

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct VariableDescription;
  // forward declaration
  struct VariableDescriptionContainer;

  //! class handling all type variables types supported by MFront
  struct MFRONT_VISIBILITY_EXPORT SupportedTypes {
    //! limit for small array support
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    static const int ArraySizeLimit = 10u;
#else
    static constexpr int ArraySizeLimit = 10u;
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H_ */
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
      //! \return true is all components of the TipeSize are null
      bool isNull(void) const;

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
    static const std::map<std::string, TypeFlag>& getTypeFlags(void);
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
    /*!
     * \return the flag associated with the given type
     * \param[in] t : type
     */
    TypeFlag getTypeFlag(const std::string&) const;
    //! return a map associating a supported type to is type flag
    /*!
     * \param[in] t : variable type
     * \param[in] a : array size
     */
    TypeSize getTypeSize(const std::string&, const unsigned short) const;

    std::string getTimeDerivativeType(const std::string&) const;
    /*!
     * \return true of the parser shall declare a dynamically
     * allocated vector for the given array size
     */
    bool useDynamicallyAllocatedVector(const unsigned short) const;
    /*!
     * \return true if dynamically allocated vectors are allowed
     */
    bool areDynamicallyAllocatedVectorsAllowed(void) const;
    /*!
     * write the given variables declaration
     * \param[out] f                 : output file
     * \param[in]  v                 : variables to be declared
     * \param[in]  prefix            : prefix added to variable's names
     * \param[in]  suffix            : suffix added to variable's names
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     */
    virtual void writeVariablesDeclarations(std::ostream&,
                                            const VariableDescriptionContainer&,
                                            const std::string&,
                                            const std::string&,
                                            const std::string&,
                                            const bool) const;
    /*!
     * write the given variable declaration
     * \param[out] f                 : output file
     * \param[in]  v                 : variable to be declared
     * \param[in]  prefix            : prefix added to variable's names
     * \param[in]  suffix            : suffix added to variable's names
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     */
    virtual void writeVariableDeclaration(std::ostream&,
                                          const VariableDescription&,
                                          const std::string&,
                                          const std::string&,
                                          const std::string&,
                                          const bool) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are
     * initialized
     */
    virtual void writeVariableInitializersInBehaviourDataConstructorI(
        std::ostream&,
        const VariableDescriptionContainer&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are
     * initialized
     */
    virtual void writeVariableInitializersInBehaviourDataConstructorII(
        std::ostream&,
        const VariableDescriptionContainer&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[in]  v                 : variables to be initialized
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     */
    virtual std::string getIntegrationVariablesIncrementsInitializers(
        const VariableDescriptionContainer&, const bool) const;

    /*!
     * \param[out] f     : output file
     * \param[in]  v     : variables to be initialized
     * \param[in]  src   : name of the array to which the variables are exported
     * \param[in]  useQt : true if quantities are used
     */
    virtual void exportResults(std::ostream&,
                               const VariableDescriptionContainer&,
                               const std::string&,
                               const bool) const;
    /*!
     * \param[in]  v   : variables
     */
    TypeSize getTotalSize(const VariableDescriptionContainer&) const;
    /*!
     * \param[in]  vc : variables
     */
    unsigned short getNumberOfVariables(
        const VariableDescriptionContainer&) const;

    void writeResultsArrayResize(std::ostream&,
                                 const std::string&,
                                 const VariableDescriptionContainer&) const;

    void writeResultsArrayResize(std::ostream&,
                                 const std::string&,
                                 const SupportedTypes::TypeSize&) const;
    //! desctructor
    virtual ~SupportedTypes();

   protected:
    //! reset the class members
    void reset(void);
    /*!
     * Support for dynamically allocated vectors is not allowed in all
     * parsers. Parsers may change this value to disable the use of
     * dynamically allocated vectors
     */
    bool areDynamicallyAllocatedVectorsAllowed_;
  };  // end of class SupportedTypes

}  // end of namespace mfront

#endif /* LIB_MFRONT_SUPPORTEDTYPES_H_ */
