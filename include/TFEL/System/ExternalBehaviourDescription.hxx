/*!
 * \file   include/TFEL/System/ExternaBehaviourDescription.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   15 June 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_SYSTEM_EXTERNALBEHAVIOURDESCRIPTION_H_
#define LIB_TFEL_SYSTEM_EXTERNALBEHAVIOURDESCRIPTION_H_

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace system
  {

    /*!
     * A structure containing the information that could be retrieved
     * by the ExternalLibraryManger class about a given behaviour for
     * a given hypothesis.
     */
    struct ExternalBehaviourDescription
    {
      /*!
       * a small enumeration telling which interface was used
       */
      enum Interface{
	UMAT,
	ASTER,
	CYRANO
      };
      /*!
       * \param[in] l : library
       * \param[in] f : function
       * \param[in] h : hypothesis
       */
      TFELSYSTEM_VISIBILITY_EXPORT
      ExternalBehaviourDescription(const std::string&,
				   const std::string&,
				   const std::string&);
      //! default constructor
      TFELSYSTEM_VISIBILITY_EXPORT
      ExternalBehaviourDescription();
      //! assignement (disabled)
      TFELSYSTEM_VISIBILITY_EXPORT ExternalBehaviourDescription&
      operator=(const ExternalBehaviourDescription&);
      //! move assignement (disabled)
      TFELSYSTEM_VISIBILITY_EXPORT ExternalBehaviourDescription&
      operator=(ExternalBehaviourDescription&&);
      //! copy constructor
      TFELSYSTEM_VISIBILITY_EXPORT
      ExternalBehaviourDescription(const ExternalBehaviourDescription&);
      //! move constructor
      TFELSYSTEM_VISIBILITY_EXPORT
      ExternalBehaviourDescription(ExternalBehaviourDescription&&);
      //! names of the material properties
      std::vector<std::string> mpnames;
      //! names of the internal state variables
      std::vector<std::string> ivnames;
      //! types of the internal state variables
      std::vector<int> ivtypes;
      //! names of the external state variables
      std::vector<std::string> evnames;
      //! source file
      std::string source;
      /*!
       * \brief behaviourDescription type
       * 0 : general behaviourDescription
       * 1 : small strain behaviourDescription
       * 2 : finite strain behaviourDescription
       * 3 : cohesive zone model
       */
      unsigned short btype;
      /*!
       * symmetry type:
       * - If stype is equal to 0, the behaviour is isotropic.
       * - If stype is equal to 1, the behaviour is orthotropic.
       */
      unsigned short stype;
      /*!
       * elastic symmetry type:
       * - If etype is equal to 0, the behaviour elastic behaviour is isotropic.
       * - If etype is equal to 1, the behaviour elastic behaviour is orthotropic.
       */
      unsigned short etype;
      //! If isUPUIR is true, the behaviour is usable in purely implicit resolution
      bool isUPUIR;
      //! If requiresStiffnessTensor is true, the caller must provide the stiffness tensor
      bool requiresStiffnessTensor;
      /*!
       * If requiresThermalExpansionCoefficientTensor is true, the
       * caller must provide the thermal expansion coefficients tensor
       */
      bool requiresThermalExpansionCoefficientTensor;
      //! destructor
      TFELSYSTEM_VISIBILITY_EXPORT ~ExternalBehaviourDescription();
    };
    
  } // end of namespace system

} // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_EXTERNALBEHAVIOURDESCRIPTION_H_ */
