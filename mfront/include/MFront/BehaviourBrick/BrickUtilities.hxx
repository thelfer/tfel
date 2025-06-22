/*!
 * \file   BrickUtilities.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX

#include <array>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct IsotropicHardeningRule;

  /*!
   * \return a modifier suitable for evaluating a material property at
   * \(t+\theta\,dt\).
   * \param[in] bd: behaviour description
   */
  MFRONT_VISIBILITY_EXPORT std::function<
      std::string(const BehaviourDescription::MaterialPropertyInput &)>
  getMiddleOfTimeStepModifier(const BehaviourDescription &);
  /*!
   * \brief check that the options names are in a given set
   * of keys.
   * \param[in] d: options
   * \param[in] k: supported keys
   * \param[in] b: calling brick name
   */
  MFRONT_VISIBILITY_EXPORT void checkOptionsNames(
      const std::map<std::string, tfel::utilities::Data> &,
      const std::vector<std::string> &,
      const std::string &);
  /*!
   * \return true if all the elements of the array is constant.
   * \param[in] mps: array of material properties
   */
  MFRONT_VISIBILITY_EXPORT bool areAllConstantMaterialProperties(
      const std::vector<BehaviourDescription::MaterialProperty> &);
  /*!
   * \return true if all the elements of the array is constant.
   * \tparam N: size of the array of material properties
   * \param[in] mps: array of material properties
   */
  template <std::size_t N>
  bool areAllConstantMaterialProperties(
      const std::array<BehaviourDescription::MaterialProperty, N> &);
  /*!
   * \brief declare a parameter or a local variable used to store the
   * evaluation of the material property.
   * \param[out] bd: behaviour description
   * \param[in,out] mp: material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   */
  MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
      BehaviourDescription &,
      BehaviourDescription::MaterialProperty &,
      const std::string &,
      const std::string &);
  /*!
   * \brief declare an array of parameters or an array of local variables used
   * to store the evaluation of an array of material properties.
   * \param[out] bd: behaviour description
   * \param[in,out] mps: array of material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   */
  MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
      BehaviourDescription &,
      std::vector<BehaviourDescription::MaterialProperty> &,
      const std::string &,
      const std::string &);
  /*!
   * \brief declare an array of parameters or an array of local variables used
   * to store the evaluation of an array of material properties.
   * \tparam N: size of the array of material properties
   * \param[out] bd: behaviour description
   * \param[in,out] mps: array of material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   */
  template <std::size_t N>
  void declareParameterOrLocalVariable(
      BehaviourDescription &,
      std::array<BehaviourDescription::MaterialProperty, N> &,
      const std::string &,
      const std::string &);
  /*!
   * \brief declare a parameter or a local variable used to store the
   * evaluation of the material property.
   * \param[out] bd: behaviour description
   * \param[in,out] mp: material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   * \param[in] en: external name
   */
  MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
      BehaviourDescription &,
      BehaviourDescription::MaterialProperty &,
      const std::string &,
      const std::string &,
      const std::string &);
  /*!
   * \brief declare an array of parameters or an array of local variables used
   * to store the evaluation of an array of material properties.
   * \tparam N: size of the array of material properties
   * \param[out] bd: behaviour description
   * \param[in,out] mps: array of material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   * \param[in] en: external name
   */
  template <std::size_t N>
  void declareParameterOrLocalVariable(
      BehaviourDescription &,
      std::array<BehaviourDescription::MaterialProperty, N> &,
      const std::string &,
      const std::string &,
      const std::string &);
  /*!
   * \brief declare a parameter or a local variable used to store the
   * evaluation of the material property.
   * \param[out] bd: behaviour description
   * \param[in,out] mp: material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   * \param[in] g: glossary entry
   */
  MFRONT_VISIBILITY_EXPORT void declareParameterOrLocalVariable(
      BehaviourDescription &,
      BehaviourDescription::MaterialProperty &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &);
  /*!
   * \brief declare an array of parameters or an array of local variables used
   * to store the evaluation of an array of material properties.
   * \tparam N: size of the array of material properties
   * \param[out] bd: behaviour description
   * \param[in,out] mps: array of material property
   * \param[in] t: variable type
   * \param[in] n: variable name
   * \param[in] g: glossary entry
   */
  template <std::size_t N>
  void declareParameterOrLocalVariable(
      BehaviourDescription &,
      std::array<BehaviourDescription::MaterialProperty, N> &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &);
  /*!
   * \return the code initializing the variable containing the material
   * property value at \(t+\theta\,dt\).
   * \note If the material property is constant, an empty string is
   * returned,
   * as this material property is assumed to be associated with a parameter.
   * \param[in] dsl: abstract behaviour dsl.
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable storing the material property value.
   * \param[in] mp: material property description.
   */
  MFRONT_VISIBILITY_EXPORT std::string
  generateMaterialPropertyInitializationCode(
      const AbstractBehaviourDSL &,
      const BehaviourDescription &,
      const std::string &,
      const BehaviourDescription::MaterialProperty &);
  /*!
   * \return the code initializing the arrays of variables containing the
   * material properties' values at \(t+\theta\,dt\).
   * \note If all the material properties are constant, an empty string is
   * returned, as those material properties are assumed to be associated with
   * an array of parameters.
   * \param[in] dsl: abstract behaviour dsl.
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable storing the material property value.
   * \param[in] mps: array of material property description.
   */
  std::string generateMaterialPropertiesInitializationCode(
      const AbstractBehaviourDSL &,
      const BehaviourDescription &,
      const std::string &,
      const std::vector<BehaviourDescription::MaterialProperty> &);
  /*!
   * \return the code initializing the arrays of variables containing the
   * material properties' values at \(t+\theta\,dt\).
   * \note If all the material properties are constant, an empty string is
   * returned, as those material properties are assumed to be associated with
   * an array of parameters.
   * \param[in] dsl: abstract behaviour dsl.
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable storing the material property value.
   * \param[in] mps: array of material property description.
   */
  template <std::size_t N>
  std::string generateMaterialPropertiesInitializationCode(
      const AbstractBehaviourDSL &,
      const BehaviourDescription &,
      const std::string &,
      const std::array<BehaviourDescription::MaterialProperty, N> &);
  /*!
   * \brief extract a linear transformation from the given map
   * \param[in] dsl: abstract behaviour dsl.
   * \param[in] bd: behaviour description
   * \param[in] d: data map
   * \param[in] o: name of option containing the linear transformation
   * \param[in] n: name of the variable associated with the linear
   * transformation \param[in] en: external name of the variable associated with
   * the linear transformation
   */
  MFRONT_VISIBILITY_EXPORT
  std::array<BehaviourDescription::MaterialProperty, 9u>
  extractLinearTransformation(AbstractBehaviourDSL &,
                              BehaviourDescription &,
                              const tfel::utilities::DataMap &,
                              const std::string_view,
                              const std::string_view,
                              const std::string_view);
  /*!
   * \brief extract the coefficients describing a Hill tensor from the given map
   * \param[in] dsl: abstract behaviour dsl.
   * \param[in] bd: behaviour description
   * \param[in] d: data map
   * \param[in] o: name of option containing the linear transformation
   * \param[in] n: name of the variable associated with the linear
   * transformation \param[in] en: external name of the variable associated with
   * the linear transformation
   */
  MFRONT_VISIBILITY_EXPORT
  std::array<BehaviourDescription::MaterialProperty, 6u>
  extractHillTensorCoefficients(AbstractBehaviourDSL &,
                                BehaviourDescription &,
                                const tfel::utilities::DataMap &,
                                const std::string_view,
                                const std::string_view,
                                const std::string_view);
  /*!
   *\brief add a new material property
   *\param[in] bd: behaviour description
   *\param[in] t: type of the material property
   *\param[in] n: name of the variable
   *\param[in] g: glossary name
   *\param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addMaterialPropertyIfNotDefined(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short = 1u);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addMaterialPropertyIfNotDefined(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const std::string &,
      const unsigned short = 1u);
  /*!
   * \brief add a new state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addStateVariable(BehaviourDescription &,
                                                 const std::string &,
                                                 const std::string &,
                                                 const unsigned short = 1u);
  /*!
   * \brief add a new state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] g: glossary entry
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addStateVariable(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short = 1u);
  /*!
   * \brief add a new state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addStateVariable(BehaviourDescription &,
                                                 const std::string &,
                                                 const std::string &,
                                                 const std::string &,
                                                 const unsigned short = 1u);
  /*!
   * \brief add a new state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the state variable
   * \param[in] n: name of the variable
   * \param[in] g: glossary name
   * \param[in] s: array size
   * \param[in] bo: allow name shadowing. If true, allows that a state
   * variable
   * with the given glossary name but a different name has already been
   * defined.
   */
  MFRONT_VISIBILITY_EXPORT void addStateVariableIfNotDefined(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short = 1u,
      const bool bo = false);
  /*!
   * \brief add a new state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the state variable
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   * \param[in] bo: allow name shadowing. If true, allows that a state
   * variable
   * with the given entry name but a different name has already been defined.
   */
  MFRONT_VISIBILITY_EXPORT void addStateVariableIfNotDefined(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const std::string &,
      const unsigned short = 1u,
      const bool bo = false);
  /*!
   * \brief add a new auxiliary state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the state variable
   * \param[in] n: name of the variable
   * \param[in] g: glossary name
   * \param[in] s: array size
   * \param[in] bo: allow name shadowing. If true, allows that a state
   * variable
   * with the given glossary name but a different name has already been
   * defined.
   */
  MFRONT_VISIBILITY_EXPORT void addAuxiliaryStateVariableIfNotDefined(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short = 1u,
      const bool bo = false);
  /*!
   * \brief add a new auxiliary state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the state variable
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   * \param[in] bo: allow name shadowing. If true, allows that a state
   * variable with the given entry name but a different name has already been
   * defined.
   */
  MFRONT_VISIBILITY_EXPORT void addAuxiliaryStateVariableIfNotDefined(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const std::string &,
      const unsigned short = 1u,
      const bool bo = false);
  /*!
   * \brief add a new external state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const unsigned short = 1u);
  /*!
   * \brief add a new external state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] g: glossary entry
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short = 1u);
  /*!
   * \brief add a new external state variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addExternalStateVariable(
      BehaviourDescription &,
      const std::string &,
      const std::string &,
      const std::string &,
      const unsigned short = 1u);
  /*!
   * \brief add a new local variable
   * \param[in] bd: behaviour description
   * \param[in] t: type of the material property
   * \param[in] n: name of the variable
   * \param[in] s: array size
   */
  MFRONT_VISIBILITY_EXPORT void addLocalVariable(BehaviourDescription &,
                                                 const std::string &,
                                                 const std::string &,
                                                 const unsigned short = 1u);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] p: parameter default value
   */
  MFRONT_VISIBILITY_EXPORT void addParameter(
      BehaviourDescription &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &e,
      const double);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   * \param[in] p: parameter default value
   */
  MFRONT_VISIBILITY_EXPORT void addParameter(
      BehaviourDescription &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short,
      const double);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   * \param[in] p: parameter default values
   */
  MFRONT_VISIBILITY_EXPORT void addParameter(
      BehaviourDescription &,
      const std::string &,
      const tfel::glossary::GlossaryEntry &,
      const unsigned short,
      const std::vector<double> &);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] p: parameter default value
   */
  MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription &,
                                             const std::string &,
                                             const std::string &,
                                             const double);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   * \param[in] p: parameter default value
   */
  MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription &,
                                             const std::string &,
                                             const std::string &,
                                             const unsigned short,
                                             const double);
  /*!
   * \brief add a new material property
   * \param[in] bd: behaviour description
   * \param[in] n: name of the variable
   * \param[in] e: entry name
   * \param[in] s: array size
   * \param[in] p: parameter default values
   */
  MFRONT_VISIBILITY_EXPORT void addParameter(BehaviourDescription &,
                                             const std::string &,
                                             const std::string &,
                                             const unsigned short,
                                             const std::vector<double> &);
  /*!
   * \brief generate the code that evaluates the initial elastic limit.
   * \param[in] bd: behaviour description
   * \param[in] ihrs: list of isotropic hardening rules.
   * \param[in] fid: flow id
   */
  MFRONT_VISIBILITY_EXPORT std::string computeElasticLimitInitialValue(
      const BehaviourDescription &,
      const std::vector<std::shared_ptr<IsotropicHardeningRule>> &,
      const std::string &);
  /*!
   * \brief generate the code that evaluates the elastic limit.
   * \param[in] bd: behaviour description
   * \param[in] ihrs: list of isotropic hardening rules.
   * \param[in] fid: flow id
   */
  MFRONT_VISIBILITY_EXPORT std::string computeElasticLimit(
      const BehaviourDescription &,
      const std::vector<std::shared_ptr<IsotropicHardeningRule>> &,
      const std::string &);
  /*!
   * \brief generate the code that evaluates the elastic limit and its
   * derivative with respect to the equivalent strain.
   * \param[in] bd: behaviour description
   * \param[in] ihrs: list of isotropic hardening rules.
   * \param[in] fid: flow id
   */
  MFRONT_VISIBILITY_EXPORT std::string computeElasticLimitAndDerivative(
      const BehaviourDescription &,
      const std::vector<std::shared_ptr<IsotropicHardeningRule>> &,
      const std::string &);
  /*!
   * \brief return a string representation of the eigen solver enum associated
   * with this string according to the following map:
   * - `TFEL` or `default` -> `tfel::math::stensor_common::TFELEIGENSOLVER`
   * - `Jacobi` -> `tfel::math::stensor_common::FSESJACOBIEIGENSOLVER`
   * \param[in] e: eigen solver option
   */
  MFRONT_VISIBILITY_EXPORT std::string handleEigenSolverOption(
      const std::string &);
  /*!
   * \return a piece of code testing if the current integration point is
   * broken if a variable with the glossary name `Broken` as been defined. If
   * no such variable has been declared, an empty string is returned.
   * \param[in] bd: behaviour description
   * \param[in] b: boolean stating if the broken must be evaluated at the end
   * of the time step (`true`) or at the middle of the time step (`false`).
   */
  MFRONT_VISIBILITY_EXPORT std::string getBrokenTest(
      const BehaviourDescription &, const bool);
  /*!
   * \brief if a variable with the glossary name `Broken` has been defined,
   * this function includes the given piece of code evaluating the stress
   * tensor in a conditional block. If the material is broken, the stress
   * tensor is set to zero. If the material is not broken, the original code
   * is called. The given code is not modified if no variable with glossary
   * name `Broken` is defined.
   * \param[in,out] c: original code on input, modified code on output.
   * \param[in] bd: behaviour description.
   * \param[in] b: boolean stating if the broken must be evaluated at the end
   * of the time step (`true`) or at the middle of the time step (`false`).
   */
  MFRONT_VISIBILITY_EXPORT void addBrokenStateSupportToComputeStress(
      std::string &, const BehaviourDescription &, const bool);
  /*!
   * \return an array of material properties from the data read in
   * an input file.
   * \param[in] bsl: domain specific language
   * \param[in] n: name of the material properties
   * \param[in] d: data in which the material properties are stored.
   */
  MFRONT_VISIBILITY_EXPORT
  std::vector<BehaviourDescription::MaterialProperty>
  getArrayOfBehaviourDescriptionMaterialProperties(
      AbstractBehaviourDSL &,
      const std::string_view,
      const tfel::utilities::Data &);

  /*!
   * \return a fixed-sized array of material properties from the data read in
   * an input file.
   * \tparam N: number of material properties to be read
   * \param[in] bsl: domain specific language
   * \param[in] n: name of the material properties
   * \param[in] d: data in which the material properties are stored.
   */
  template <std::size_t N>
  std::array<BehaviourDescription::MaterialProperty, N>
  getArrayOfBehaviourDescriptionMaterialProperties(
      AbstractBehaviourDSL &,
      const std::string_view,
      const tfel::utilities::Data &);

}  // end of namespace mfront::bbrick

#include "MFront/BehaviourBrick/BrickUtilities.ixx"

#endif /* LIB_MFRONT_BEHAVIOURBRICK_BRICKUTILITIES_HXX */
