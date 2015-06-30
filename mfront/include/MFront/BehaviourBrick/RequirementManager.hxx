/*!
 * \file   RequirementManager.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   27 juin 2015
 */

#ifndef _LIB__MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_
#define _LIB__MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_

#include<vector>
#include<memory>
#include"MFront/MFrontConfig.hxx"

namespace mfront{

  namespace bbrick{

    //! forward declaration
    struct Provider;
    
    /*!
     * structure used to gather and solve all the behaviour brick
     * requirement for a given modelling hypothesis
     */
    struct MFRONT_VISIBILITY_EXPORT RequirementManager{
      /*!
       * \brief add a material property provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] s : array size
       */
      void addMaterialPropertyProvider(const std::string&,
				       const std::string&,
				       const std::string&,
				       const unsigned short);
      /*!
       * \brief add a state variable provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] s : array size
       */
      void addStateVariableProvider(const std::string&,
				    const std::string&,
				    const std::string&,
				    const unsigned short);
      /*!
       * \brief add an auxiliary state variable provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] s : array size
       */
      void addAuxiliaryStateVariableProvider(const std::string&,
					     const std::string&,
					     const std::string&,
					     const unsigned short);
      /*!
       * \brief add an external state variable provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] s : array size
       */
      void addExternalStateVariableProvider(const std::string&,
					    const std::string&,
					    const std::string&,
					    const unsigned short);
      /*!
       * \brief add an integration variable provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] s : array size
       */
      void addIntegrationVariableProvider(const std::string&,
					  const std::string&,
					  const std::string&,
					  const unsigned short);
      /*!
       * \brief add a static provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       */
      void addStaticVariableProvider(const std::string&,
				     const std::string&,
				     const std::string&);
      /*!
       * \brief add a parameter provider
       * \param[in] t : variable type
       * \param[in] n : variable name 
       * \param[in] e : external name (glossary name or entry name)
       */
      void addParameterProvider(const std::string&,
				const std::string&,
				const std::string&);
      /*!
       * \return a reference to the provider with the matching external name
       * \param[in] e : external name (glossary name or entry name)
       */
      const Provider& getProvider(const std::string&) const;
    private:
      /*!  
       * \brief check if a provider with the same external name has
       * not been declared
       * \param[in] e : external name
       */
      void check(const std::string&) const;
      //! all registred providers
      std::vector<std::shared_ptr<Provider>> providers;
      /*!
       * \return an iterator to the provider with the matching external name
       * \param[in] e : external name (glossary name or entry name)
       */
      auto getProviderIterator(const std::string&) const
	-> decltype(this->providers.begin());
      /*!
       * \return an iterator to the provider with the matching external name
       * \param[in] e : external name (glossary name or entry name)
       */
      auto getProviderIterator(const std::string&)
	-> decltype(this->providers.begin());
    };
    
  } // end of namespace bbrick

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_ */


