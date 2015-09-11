/*!
 * \file   mfront/include/MFront/SupportedTypes.hxx
 * \brief  This file declares the SupportedTypes class
 * \author Helfer Thomas
 * \date   12 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_SUPPORTEDTYPES_H_
#define LIB_MFRONT_SUPPORTEDTYPES_H_ 

#include<map>
#include<string>
#include<vector>
#include<ostream>

#include"MFront/MFrontConfig.hxx"
#include"MFront/VariableDescription.hxx"

namespace mfront
{

  struct MFRONT_VISIBILITY_EXPORT SupportedTypes
  {

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    static const int ArraySizeLimit = 10u;
#else 
    static constexpr int ArraySizeLimit = 10u;
#endif /* LIB_TFEL_SYSTEM_EXTERNALFUNCTIONSPROTOTYPES_H_ */


    enum TypeFlag{Scalar,TVector,Stensor,Tensor};

    struct MFRONT_VISIBILITY_EXPORT TypeSize
    {

      TypeSize();

      TypeSize(const TypeSize&);

      TypeSize(const int,const int,
	       const int,const int);

      TypeSize&
      operator=(const TypeSize&);
    
      TypeSize&
      operator+=(const TypeSize&);

      TypeSize&
      operator-=(const TypeSize&);
    
      bool
      operator!=(const TypeSize&) const;

      bool
      operator==(const TypeSize&) const;

      int
      getScalarSize() const;
    
      int
      getTVectorSize() const;

      int
      getStensorSize() const;

      int
      getTensorSize() const;

      int
      getValueForDimension(const unsigned short) const;

      bool isNull(void) const;
      
    private:

      friend std::ostream& 
      operator<< (std::ostream&, const TypeSize&);

      int scalarSize;
      int tvectorSize;
      int stensorSize;
      int tensorSize;

    }; // end of class SupportedTypes::TypeSize
    //! \return a list of type names associated with type flags
    static const std::map<std::string,TypeFlag>&
    getTypeFlags(void);
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
    TypeSize
    getTypeSize(const std::string&,
		const unsigned short) const;

    std::string
    getTimeDerivativeType(const std::string&) const;

    /*!
     * \return true of the parser shall declare a dynamically
     * allocated vector for the given array size
     */
    bool
    useDynamicallyAllocatedVector(const unsigned short) const;

    /*!
     * \return true if dynamically allocated vectors are allowed
     */
    bool
    areDynamicallyAllocatedVectorsAllowed(void) const;

    /*!
     * write the given variables declaration
     * \param[out] f                 : output file
     * \param[in]  v                 : variables to be declared
     * \param[in]  prefix            : prefix added to variable's names
     * \param[in]  suffix            : suffix added to variable's names
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     */
    virtual void
    writeVariablesDeclarations(std::ostream&,
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
    virtual void
    writeVariableDeclaration(std::ostream&,
			     const VariableDescription&,
			     const std::string&,
			     const std::string&,
			     const std::string&,
			     const bool) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are initialized
     */
    virtual void
    writeVariableInitializersInBehaviourDataConstructorI(std::ostream&,
							 const VariableDescriptionContainer&,
							 const std::string&,
							 const std::string&,
							 const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are initialized
     */
    virtual void
    writeVariableInitializersInBehaviourDataConstructorII(std::ostream&,
							  const VariableDescriptionContainer&,
							  const std::string&,
							  const std::string&,
							  const std::string&) const;
    /*!
     * \param[in]  v                 : variables to be initialized
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     */
    virtual std::string
    getIntegrationVariablesIncrementsInitializers(const VariableDescriptionContainer&,
						  const bool) const;

    /*!
     * \param[out] f     : output file
     * \param[in]  v     : variables to be initialized
     * \param[in]  src   : name of the array to which the variables are exported
     * \param[in]  useQt : true if quantities are used
     */
    virtual void
    exportResults(std::ostream&,
		  const VariableDescriptionContainer&,
		  const std::string&,
		  const bool) const;
    
    /*!
     * \param[in]  v   : variables
     */
    TypeSize
    getTotalSize(const VariableDescriptionContainer&) const;

    /*!
     * \param[in]  v   : variables
     */
    unsigned short
    getNumberOfVariables(const VariableDescriptionContainer&) const;

    void
    writeResultsArrayResize(std::ostream&,
			    const std::string&,
			    const VariableDescriptionContainer&) const;
    
    void
    writeResultsArrayResize(std::ostream&,
			    const std::string&,
			    const SupportedTypes::TypeSize&) const;

    virtual ~SupportedTypes();

  protected:

    /*!
     * reset the class members
     */
    void reset(void);

    /*!
     * Support for dynamically allocated vectors is not allowed in all
     * parsers. Parsers may change this value to disable the use of
     * dynamically allocated vectors
     */
    bool areDynamicallyAllocatedVectorsAllowed_;
  }; // end of class SupportedTypes

} // end of namespace mfront

#endif /* LIB_MFRONT_SUPPORTEDTYPES_H_ */

