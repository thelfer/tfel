/*!
 * \file   RequirementManager.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   27 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_
#define LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_

#include <vector>
#include <memory>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct BehaviourData;
  // forward declaration
  struct VariableDescription;
  // forward declaration
  struct StaticVariableDescription;
  // forward declaration
  struct VariableDescriptionContainer;

  namespace bbrick {

    // forward declaration
    struct Provider;
    // forward declaration
    struct Requirement;

    /*!
     * structure used to gather and solve all the behaviour brick
     * requirement for a given modelling hypothesis
     */
    struct MFRONT_VISIBILITY_EXPORT RequirementManager {
      /*!
       * constructor
       * \param[in] b : check units
       */
      RequirementManager(const bool = false);
      /*!
       * \brief constructor from a behaviour data
       * \param[in] bd : behaviour data
       * \param[in] b : check units
       */
      RequirementManager(const mfront::BehaviourData&, const bool);
      /*!
       * add a requirement
       * \param[in] r : requirement
       */
      void addRequirement(const Requirement&);
      /*!
       * add a requirement
       * \param[in] r : requirement
       */
      void addRequirement(Requirement&&);
      /*!
       * \brief add a material property provider
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addMaterialPropertyProvider(const mfront::VariableDescription&,
                                       const std::string&);
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
       * \brief add an auxiliary state variable provider
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addAuxiliaryStateVariableProvider(const mfront::VariableDescription&,
                                             const std::string&);
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
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addExternalStateVariableProvider(const mfront::VariableDescription&,
                                            const std::string&);
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
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addIntegrationVariableProvider(const mfront::VariableDescription&,
                                          const std::string&);
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
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addStaticVariableProvider(const mfront::StaticVariableDescription&,
                                     const std::string&);
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
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addParameterProvider(const mfront::VariableDescription&,
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
       * \brief add a local variable provider
       * \param[in] v : variable description
       * \param[in] e : external name (glossary name or entry name)
       */
      void addLocalVariableProvider(const mfront::VariableDescription&,
                                    const std::string&);
      /*!
       * \brief add a local variable provider
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name (glossary name or entry name)
       * \param[in] s : array size
       */
      void addLocalVariableProvider(const std::string&,
                                    const std::string&,
                                    const std::string&,
                                    const unsigned short);
      /*!
       * \return a reference to the provider with the matching external name
       * \param[in] e : external name (glossary name or entry name)
       */
      const Provider& getProvider(const std::string&) const;
      /*!
       * \return all unresolved requirements
       */
      std::vector<std::string> getUnresolvedRequirements(void) const;
      /*!
       * \param[in] n : external name of the requirement
       * \return the requirement
       */
      const Requirement& getRequirement(const std::string&) const;
      /*!
       * \param[in] n : external name of a requirement
       * \return true if the given requirement has a provider
       */
      bool hasProvider(const std::string&) const;
      //! destructor
      ~RequirementManager();

     private:
      // copy assignement
      RequirementManager& operator=(const RequirementManager&) = delete;
      // move assignement
      RequirementManager& operator=(RequirementManager&&) = delete;
      /*!
       * \param[in] r : requirement
       * \return true if the given requirement has a provider
       */
      bool hasProvider(const Requirement&) const;
      /*!
       * \brief add a requirement
       * \param[in] r : requirement
       */
      void addRequirement(const std::shared_ptr<Requirement>&);
      /*!
       * \brief check if a provider with the same external name has
       * not been declared
       * \param[in] p : provider
       */
      void check(const Provider&) const;
      //! all registred requirements
      std::vector<std::shared_ptr<Requirement>> requirements;
      //! all registred providers
      std::vector<std::shared_ptr<Provider>> providers;
      /*!
       * \return an iterator to the provider with the matching external name
       * \param[in] e : external name (glossary name or entry name)
       */
      std::vector<std::shared_ptr<Provider>>::const_iterator
      getProviderIterator(const std::string&) const;
      /*!
       * \return an iterator to the provider with the matching external name
       * \param[in] e : external name (glossary name or entry name)
       */
      std::vector<std::shared_ptr<Provider>>::iterator getProviderIterator(
          const std::string&);
      //! check units
      const bool checkUnits;
    };

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTMANAGER_HXX_ */
