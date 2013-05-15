/*!
 * \file   SupportedTypes.hxx
 * \brief  This file declares the SupportedTypes class
 * \author Helfer Thomas
 * \date   12 Jan 2007
 */

#ifndef _LIB_MFRONT_SUPPORTEDTYPES_H_
#define _LIB_MFRONT_SUPPORTEDTYPES_H_ 

#include<string>
#include<ostream>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"

#include"MFront/VarHandler.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT SupportedTypes
  {

    static const unsigned short ArraySizeLimit;

    enum TypeFlag{Scalar,Stensor};

    struct TypeSize;

    struct TFEL_VISIBILITY_EXPORT TypeSize
    {

      typedef unsigned short ushort;

      TypeSize();

      TypeSize(const TypeSize&);

      TypeSize(const ushort,const ushort);

      TypeSize&
	operator=(const TypeSize&);
    
      TypeSize&
	operator+=(const TypeSize&);
    
      ushort
	getScalarSize() const;
    
      ushort
	getStensorSize() const;

    private:

      friend std::ostream& 
	operator<< (std::ostream&, const TypeSize&);

      ushort scalarSize;
      ushort stensorSize;

    }; // end of class SupportedTypes::TypeSize

    SupportedTypes();

    TypeFlag
      getTypeFlag(const std::string&) const;

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
     * write the variables declaration
     * \param[out] f                 : output file
     * \param[in]  v                 : variables to be declared
     * \param[in]  prefix            : prefix added to variable's names
     * \param[in]  suffix            : suffix added to variable's names
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     * \param[in]  b                 : debug mode
     */
    void
    writeVariablesDeclarations(std::ostream&,
			       const VarContainer&,
			       const std::string&,
			       const std::string&,
			       const std::string&,
			       const bool,
			       const bool);
    
    /*!
     * \param[out] f      : output file
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are initialized
     */
    virtual TypeSize
    writeVariableInitializersInBehaviourDataConstructorI(std::ostream&,
							 const VarContainer&,
							 const std::string&,
							 const std::string&,
							 const std::string&,
							 const TypeSize&  = TypeSize());
    
    /*!
     * \param[out] f      : output file
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are initialized
     */
    virtual TypeSize
    writeVariableInitializersInBehaviourDataConstructorII(std::ostream&,
							  const VarContainer&,
							  const std::string&,
							  const std::string&,
							  const std::string&,
							  const TypeSize& = TypeSize());


    /*!
     * \param[out] f                 : output file
     * \param[in]  v                 : variables to be initialized
     * \param[in]  useTimeDerivative : declare time derivative of the variables
     */
    void
    writeStateVariableIncrementsInitializers(std::ostream&,
					     const VarContainer&,
					     const bool);

    /*!
     * \param[out] f     : output file
     * \param[in]  v     : variables to be initialized
     * \param[in]  src   : name of the array to which the variables are exported
     * \param[in]  useQt : true if quantities are used
     * \param[in]  o     : offset in the array to which the variables are exported
     */
    virtual TypeSize
    exportResults(std::ostream&,
		  const VarContainer&,
		  const std::string&,
		  const bool,
		  const TypeSize&  = TypeSize());
    
    /*!
     * \param[in]  v   : variables
     */
    TypeSize
    getTotalSize(const VarContainer&);

    /*!
     * \param[in]  v   : variables
     */
    unsigned short
    getNumberOfVariables(const VarContainer&);

    void
    writeResultsArrayResize(std::ostream&,
			    const std::string&,
			    const VarContainer&);
    
    void
    writeResultsArrayResize(std::ostream&,
			    const std::string&,
			    const SupportedTypes::TypeSize&);
    
    virtual ~SupportedTypes();

  private:
    
    std::map<std::string,TypeFlag> flags;
    
    TFEL_VISIBILITY_LOCAL
      void registerTypes(void);

  }; // end of class SupportedTypes

} // end of namespace mfront

#endif /* _LIB_MFRONT_SUPPORTEDTYPES_H */

