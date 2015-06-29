/*!
 * \file   RequirementManager.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   27 juin 2015
 */

#ifndef _LIB__MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_
#define _LIB__MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_

#include<vector>

namespace mfront{

  namespace bbrick{

    //! forward declaration
    struct Provider;
    
    /*!
     * structure used to gather and solve all the behaviour brick
     * requirement for a given modelling hypothesis
     */
    struct RequirementManager{
      /*!
       * \brief add a static provider
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] n : variable name 
       */
      void addStaticVariableProvider(const std::string&,
				     const std::string&);
    };
    
  } // end of namespace bbrick

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_ */


