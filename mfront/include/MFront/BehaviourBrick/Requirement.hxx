/*!
 * \file   Requirement.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   25 juin 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 * <!-- Local IspellDict: english -->
 */

#ifndef _LIB_BEHAVIOURBRICKREQUIREMENT_HXX_
#define _LIB_BEHAVIOURBRICKREQUIREMENT_HXX_

#include<string>
#include<vector>

#include"MFront/MFrontConfig.hxx"
#include"MFront/BehaviourBrick/ProviderIdentifier.hxx"

namespace mfront{

  namespace bbrick{

    /*!
     * This structure describes a requirement of a behaviour brick.
     */
    struct MFRONT_VISIBILITY_EXPORT Requirement{
      /*!
       * \brief constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] s : array size
       * \param[in] a : allowed providers
       * \pre t must be a type be known by the SupportedTypes class.
       */
      Requirement(const std::string&,
		  const std::string&,
		  const unsigned short = 1u,
		  const std::vector<ProviderIdentifier>& = std::vector<ProviderIdentifier>{});
      //! copy constructor
      Requirement(const Requirement&);
      //! move constructor
      Requirement(Requirement&&);
      /*!
       * \brief variable type (StressStensor, etc..)
       */
      const std::string type;
      /*!
       * \brief external name of the requirement (generally a glossary name)
       */
      const std::string name;
      /*!
       * \brief array size
       */
      const unsigned short asize;
      /*!
       * \brief list of allowed variable types vor this requirement
       */
      const std::vector<ProviderIdentifier> aproviders;
    private:
      Requirement() = delete;
      Requirement& operator=(const Requirement&) = delete;
      Requirement& operator=(Requirement&&) = delete;
    }; // end of struct Requirement

  } // end of namespace bbrick
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENT_HXX_ */


