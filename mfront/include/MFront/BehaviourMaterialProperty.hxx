/*!
 * \file   mfront/include/MFront/BehaviourMaterialProperty.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURMATERIALPROPERTY_HXX
#define LIB_MFRONT_BEHAVIOURMATERIALPROPERTY_HXX

#include <set>
#include <string>
#include <vector>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  // forward declaration
  struct BehaviourDescription;

  /*!
   * Some interfaces may require additionnal material properties,
   * to compute the stiffness matrix or to compute the thermal
   * expansions. Some additional material properties may also be
   * required by the solver (mass density, orthotropic basis for the
   * Cast3M solver) which are of no interest for the behaviour. On
   * the other hand, some material properties are highly needed by
   * the behaviour (Young modulus and Poisson ratio for example).
   * This structure allows the computation of an indirection map
   * between the data given by the solver and the ones needed by the
   * interface.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourMaterialProperty
      : VariableDescription {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    /*!
     * constructor
     * \param[in] v : variable description
     * \param[in] o : offset
     * \param[in] d : dummy variable
     */
    BehaviourMaterialProperty(const VariableDescription&,
                              const SupportedTypes::TypeSize,
                              const bool);
    BehaviourMaterialProperty(const BehaviourMaterialProperty&);
    BehaviourMaterialProperty(BehaviourMaterialProperty&&);
    BehaviourMaterialProperty& operator=(const BehaviourMaterialProperty&);
    BehaviourMaterialProperty& operator=(BehaviourMaterialProperty&&);
    ~BehaviourMaterialProperty();
    //! \return if the material property is scalar
    bool isScalar() const;
    /*!
     * offset of the variable in the indirection map
     */
    SupportedTypes::TypeSize offset;
    /*!
     * If true, this variable is not considered in the computation
     * of the indirection map. For example, the Cast3M solver may
     * require a variable called 'V1X' for the first component of
     * the first direction of the orthotropic basis.  The variable
     * shall not prevent the user of defining a material property
     * named 'V1X'.
     */
    bool dummy;
  };  // end of struct BehaviourMaterialProperty

  /*!
   * \brief help structure defining the options
   * of the buildMaterialPropertiesList
   * function
   */
  struct BuildMaterialPropertiesListOptions {
    /*!
     * \brief boolean stating if material properties are used to build the
     * stiffness tensor
     */
    const bool useMaterialPropertiesToBuildStiffnessTensor;
    /*!
     * \brief boolean stating if material properties are used to build the
     * thermal expansion coefficient tensor
     */
    const bool useMaterialPropertiesToBuildThermalExpansionCoefficientTensor;
  };

  /*!
   * \return a pair which first member gives the position of the
   * material properties in the values given through the interface
   * and whose second members is an offset giving the number of
   * imposed material properties.
   * \param[in] mb: behaviour description
   * \param[in] mh: modelling hypotheses
   * \param[in] opts: options
   */
  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  buildMaterialPropertiesList(
      const BehaviourDescription&,
      const std::set<BehaviourMaterialProperty::Hypothesis>&,
      const BuildMaterialPropertiesListOptions&);
  /*!
   * \return a pair which first member gives the position of the
   * material properties in the values given through the interface
   * and whose second members is an offset giving the number of
   * imposed material properties.
   * \param[in] mb: behaviour description
   * \param[in] h:  modelling hypothesis
   * \param[in] opts: options
   */
  MFRONT_VISIBILITY_EXPORT
  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  buildMaterialPropertiesList(
      const BehaviourDescription&,
      const tfel::material::ModellingHypothesis::Hypothesis,
      const BuildMaterialPropertiesListOptions&);
  /*!
   * \brief append a variable to material property list
   * \param[out] l: list of material properties
   * \param[in]  t: type of the variable
   * \param[in]  n: name of the variable
   * \param[in]  v: variable name
   * \param[in]  b: if true, this is a dummy variable
   */
  MFRONT_VISIBILITY_EXPORT
  void appendToMaterialPropertiesList(std::vector<BehaviourMaterialProperty>&,
                                      const std::string&,
                                      const std::string&,
                                      const std::string&,
                                      const bool);
  /*!
   * \brief complete the list of material properties.
   * This method shall be called by the buildMaterialPropertiesList
   * after that the mandatory material properties list have been
   * filled.
   * \param[out] mprops: list of material properties
   * \param[in]  mb:     behaviour description
   * \param[in]  h :    modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT
  void completeMaterialPropertiesList(
      std::vector<BehaviourMaterialProperty>&,
      const BehaviourDescription&,
      const BehaviourMaterialProperty::Hypothesis);
  /*!
   * an helper function returning a material property
   * \param[in] mprops : material properties
   * \param[in] n      : glossary name
   * \return the umat material property with the given material
   * property name
   */
  MFRONT_VISIBILITY_EXPORT
  const BehaviourMaterialProperty& findBehaviourMaterialProperty(
      const std::vector<BehaviourMaterialProperty>&, const std::string&);

}  // end of namespace mfront

#endif /* LIB_BEHAVIOURMATERIALPROPERTY_HXX */
