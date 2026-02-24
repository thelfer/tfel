/*!
 * \file   BehaviourData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   07/10/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_BEHAVIOURDATA_HXX
#define LIB_MFM_TEST_GENERATOR_BEHAVIOURDATA_HXX

#include <map>
#include <string>
#include "MFMTestGenerator/Config.hxx"
#include "MFMTestGenerator/Forward.hxx"

namespace mfmtg {

  //! \brief data associated with a behaviour
  struct MFMTG_VISIBILITY_EXPORT BehaviourData {
    /*!
     * \brief default constructor
     * \param[in] p: parameters
     * \param[in] h: modelling hypothesis
     */
    BehaviourData(const TestCaseParameters&, const std::string&);
    //! \brief copy constructor
    BehaviourData(BehaviourData&&);
    //! \brief copy constructor
    BehaviourData(const BehaviourData&);
    //! \brief interface used to generator the behaviour (optional)
    const std::string interface;
    //! \brief path of the library in which the behaviour is implemented
    const std::string library;
    //! \brief function which implements the behaviour
    const std::string function;
    //! \brief modelling hypothesis
    const std::string hypothesis;
    //! \brief material properties
    const std::map<std::string, Evolution> material_properties;
    //! \brief external state variables
    const std::map<std::string, Evolution> external_state_variables;
    //! \brief destructor
    ~BehaviourData();

   private:
    //! move assignement
    BehaviourData& operator=(BehaviourData&&);
    //! copy assignement
    BehaviourData& operator=(const BehaviourData&);
  };  // end of struct BehaviourData

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_BEHAVIOURDATA_HXX */
