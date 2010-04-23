/*!
 * \file   UnaryLoadingDefinition.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 fév 2007
 */

#ifndef _LIB_MFRONT_UNARYLOADINGDEFINITION_HXX_
#define _LIB_MFRONT_UNARYLOADINGDEFINITION_HXX_ 

#include<iostream>
#include<map>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/SupportedTypes.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT UnaryLoadingDefinition
  {

    typedef tfel::utilities::CxxTokenizer::TokensContainer::const_iterator TokensIterator;

    UnaryLoadingDefinition(const std::string&,
			   const unsigned short,
			   const std::string&,
			   const bool);

    const std::string&
    getTestType(void) const;

    unsigned short
    getDimension(void) const;

    const std::string&
    getNumericType(void) const;

    bool 
    getUseQt(void) const;

    const std::vector<long double>&
    getTimes(void) const;

    const std::map<std::string,long double>&
    getCoefficientsValues(void) const;

    const std::map<std::string,long double>&
    getInternalStateVariablesValues(void) const;

    TokensIterator
    treatTimes(TokensIterator,
	       const TokensIterator);
    
    TokensIterator
    treatCoef(const std::string&,
	      const SupportedTypes::TypeFlag,
	      TokensIterator,
	      const TokensIterator);

    TokensIterator
    treatStateVar(const std::string&,
		  const SupportedTypes::TypeFlag,
		  TokensIterator,
		  const TokensIterator);

    TokensIterator
    treatExternalStateVar(const std::string&,
			  const SupportedTypes::TypeFlag,
			  TokensIterator,
			  const TokensIterator);

    TokensIterator
    treatParticularInput(const std::string&,
			 TokensIterator,
			 const TokensIterator);

    void 
    writeTestData(std::ostream&) const;

  private:

    template<typename T>
    T
    read(TokensIterator&,
	 const TokensIterator);

    static std::string
    toString(const unsigned short src);

    enum InterpolationFlag{ConstantPerInterval,LinearPerInterval};

    static const long double Cst_1_Sqrt2;

    std::string testType;
    std::string testVariable;
    unsigned short dimension;
    std::string numericType;
    bool useQt;

    std::vector<long double> times;
    std::map<std::string,long double> coefValues;
    std::map<std::string,long double> stateVarValues;

    std::map<std::string,InterpolationFlag> externalStateVarInterpolationType;
    std::map<std::string,std::map<long double,long double> > externalStateVarValues;

    std::map<long double,long double> temperatureValues;
    InterpolationFlag temperatureInterpolationType;
    bool isTemperatureInterpolationTypeDefined;

    std::map<long double,long double> loadingValues;
    InterpolationFlag loadingInterpolationType;
    bool isLoadingInterpolationTypeDefined;

  }; // end of class UnaryLoadingDefinition
  
} // end of namespace mfront  

#endif /* _LIB_MFRONT_UNARYLOADINGDEFINITION_HXX */

