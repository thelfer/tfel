/*!
 * \file   MFront/BehaviourBrick/OptionDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   23/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_OPTIONDESCRIPTION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_OPTIONDESCRIPTION_HXX

#include <map>
#include <vector>
#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace tfel::utilities {
  // forward declaration
  struct Data;
}  // end of namespace tfel::utilities

namespace tfel::glossary {
  // forward declaration
  struct GlossaryEntry;
}  // end of namespace tfel::glossary

namespace mfront::bbrick {

  /*!
   * \brief class describing the option expected by a stress potential, an
   * inelastic flow, etc.
   */
  struct MFRONT_VISIBILITY_EXPORT OptionDescription {
    //! \brief allowed types for an option
    enum Type {
      MATERIALPROPERTY,           //<! \brief material property
      ARRAYOFMATERIALPROPERTIES,  //<! \brief array of material properties
      BOOLEAN,                    //<! \brief boolean value
      REAL,                       //<! \brief floating point value
      STRING,                     //<! \brief string value
      DATAMAP,                    //<! \brief a map of data
      DATASTRUCTURE,              //<! \brief data structure
      DATASTRUCTURES /*! \brief the option must be a data structure or an
                      *  array of data structures.
                      */
    };
    /*!
     * \brief constructor
     * \param[in] n: name
     * \param[in] d: description
     * \param[in] t: type
     */
    OptionDescription(const std::string&, const std::string&, const Type);
    /*!
     * \brief constructor
     * \param[in] n: name
     * \param[in] d: description
     * \param[in] t: type
     * \param[in] ro: required options
     * \param[in] eo: incompatible options
     */
    OptionDescription(const std::string&,
                      const std::string&,
                      const Type,
                      const std::vector<std::string>&,
                      const std::vector<std::string>&);
    /*!
     * \brief constructor
     * \param[in] n: name
     * \param[in] d: description
     * \param[in] t: type
     */
    OptionDescription(const std::string&,
                      const tfel::glossary::GlossaryEntry&,
                      const Type);
    /*!
     * \brief constructor
     * \param[in] n: name
     * \param[in] d: description
     * \param[in] t: type
     * \param[in] ro: required options
     * \param[in] eo: incompatible options
     */
    OptionDescription(const std::string&,
                      const tfel::glossary::GlossaryEntry&,
                      const Type,
                      const std::vector<std::string>&,
                      const std::vector<std::string>&);
    //! \brief default constructor
    OptionDescription();
    //! \brief copy constructor
    OptionDescription(const OptionDescription&);
    //! \brief move constructor
    OptionDescription(OptionDescription&&);
    //! \brief standard assignement
    OptionDescription& operator=(const OptionDescription&);
    //! \brief move assignement
    OptionDescription& operator=(OptionDescription&&);
    //! \brief destructor
    ~OptionDescription();
    //! name of the option
    std::string name;
    //! description of the option
    std::string description;
    //! type of the option
    Type type;
    /*!
     * \brief list of the options that must be set if the current option is
     * set.
     */
    std::vector<std::string> required_options;
    /*!
     * \brief list the options that must not be set if the current option is
     * set.
     */
    std::vector<std::string> incompatible_options;
  };  // end of struct OptionDescription

  /*!
   * \brief check that the given data matches the given options. The following
   * points are checked:
   * - a key associated with a data must be a valid option name
   * - the type of the data must be the one of the associated data
   * - for every option matching a data, we check that for all required
   *   dependencies of this are associated with a data.
   * - for every option matching a data, we check that there is no data
   *   associated with an incompatible option.
   *
   * \note Here is a typical use case: the data are what was provided by the
   * user in the input file and the list of options describes what is expected
   * by the behaviour brick, the stress potential, etc...
   *
   * \param[in] dv: list of data
   * \param[in] ov: list of options
   */
  MFRONT_VISIBILITY_EXPORT void check(
      const std::map<std::string, tfel::utilities::Data>&,
      const std::vector<OptionDescription>&);
  /*!
   * \brief check that the given data has a type compatible with the given
   * option type.
   *
   * \note Here is a typical use case: the data is what was provided by the
   * user in the input file and the option type is what is expected by the
   * behaviour brick, the stress potential, etc...
   *
   * \param[in] d: data
   * \param[in] o: option
   */
  MFRONT_VISIBILITY_EXPORT void check(const tfel::utilities::Data&,
                                      const OptionDescription&);

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_OPTIONDESCRIPTION_HXX */
