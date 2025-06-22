/*!
 * \file   include/TFEL/System/ExternalBehaviourDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 June 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_EXTERNALBEHAVIOURDESCRIPTION_HXX
#define LIB_TFEL_SYSTEM_EXTERNALBEHAVIOURDESCRIPTION_HXX

#include <vector>
#include <string>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalMaterialKnowledgeDescription.hxx"

namespace tfel::system {

  /*!
   * \brief a structure containing the information that could be retrieved
   * by the `ExternalLibraryManger` class about a given behaviour for
   * a given hypothesis.
   */
  struct TFELSYSTEM_VISIBILITY_EXPORT ExternalBehaviourDescription
      : public ExternalMaterialKnowledgeDescription {
    /*!
     * \param[in] l : library
     * \param[in] f : function
     * \param[in] h : hypothesis
     */
    ExternalBehaviourDescription(const std::string&,
                                 const std::string&,
                                 const std::string&);
    //! \brief default constructor
    ExternalBehaviourDescription();
    //! \brief move constructor
    ExternalBehaviourDescription(ExternalBehaviourDescription&&);
    //! \brief copy constructor
    ExternalBehaviourDescription(const ExternalBehaviourDescription&);
    //! \brief move assignement
    ExternalBehaviourDescription& operator=(ExternalBehaviourDescription&&);
    //! \brief standard assignement
    ExternalBehaviourDescription& operator=(
        const ExternalBehaviourDescription&);
    //! \brief destructor
    ~ExternalBehaviourDescription();
    /*!
     * \brief get the default value of a double parameter
     * \param[in] p: parameter name
     */
    double getRealParameterDefaultValue(const std::string&) const;
    /*!
     * \brief get the default value of an integer parameter
     * \param[in] p: parameter name
     */
    int getIntegerParameterDefaultValue(const std::string&) const;
    /*!
     * \brief get the default value of an unsigned short parameter
     * \param[in] p: parameter name
     */
    unsigned short getUnsignedShortParameterDefaultValue(
        const std::string&) const;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    bool hasBounds(const std::string&) const;
    /*!
     * \return true if the given variable has a lower bound
     * \param[in] v: variable name
     */
    bool hasLowerBound(const std::string&) const;
    /*!
     * \return true if the given variable has a upper bound
     * \param[in] v: variable name
     */
    bool hasUpperBound(const std::string&) const;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    long double getLowerBound(const std::string&) const;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    long double getUpperBound(const std::string&) const;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    bool hasPhysicalBounds(const std::string&) const;
    /*!
     * \return true if the given variable has a lower physical bound
     * \param[in] v: variable name
     */
    bool hasLowerPhysicalBound(const std::string&) const;
    /*!
     * \return true if the given variable has a upper physical bound
     * \param[in] v: variable name
     */
    bool hasUpperPhysicalBound(const std::string&) const;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    long double getLowerPhysicalBound(const std::string&) const;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    long double getUpperPhysicalBound(const std::string&) const;
    //! \brief name of the behavior
    std::string behaviour;
    //! \brief name of the hypothesis
    std::string hypothesis;
    /*!
     * \brief list of tangent operator blocks
     *
     * \note this entry is most of the time empty for finite strain
     * behaviours, because the tangent operators used by most interfaces
     * are not directly the derivative of the stress with respect to
     * the deformation gradient.
     */
    std::vector<std::pair<std::string, std::string>> tangent_operator_blocks;
    //! \brief names of the gradients
    std::vector<std::string> gnames;
    /*!
     * \brief names of the gradients (provided for backward
     * compatibility, gradients were initially called driving variables)
     */
    std::vector<std::string> dvnames;
    /*!
     * \brief types of the gradients (reference provided for backward
     * compatibility, gradients were initially called driving variables)
     *
     * \note A type is associated to an integer as follows:
     * - 0: scalar
     * - 1: symmetric tensor
     * - 2: vector (size of the space dimension)
     * - 3: unsymmetric tensor
     */
    std::vector<int> gtypes;
    //! \brief types of the gradients (copy of dvtypes)
    std::vector<int> dvtypes;
    //! \brief names of the thermodynamic forces
    std::vector<std::string> thnames;
    /*!
     * \brief types of the thermodynamic forces
     * \note A type is associated to an integer as follows:
     * - 0: scalar
     * - 1: symmetric tensor
     * - 2: vector (size of the space dimension)
     * - 3: unsymmetric tensor
     */
    std::vector<int> thtypes;
    /*!
     * \brief names of entry points associated with elastic material
     * properties exported by the behaviour, if any. This is currently only
     * supported by a few interfaces (`Cyrano`, `Cast3M`).
     *
     * The size of this member is either:
     * - 0: which means that the behaviour does not export its elastic
     *      properties.
     * - 2, which means that the elastic behaviour is isotropic.
     * - 9, which means that the elastic behaviour is orthotropic.
     */
    std::vector<std::string> elastic_material_properties_epts;
    /*!
     * \brief names of entry points associated with linear thermal expansion
     * coefficients properties exported by the behaviour, if any. This is
     * currently only supported by a few interfaces (`Cyrano`, `Cast3M`).
     *
     * The size of this member is either:
     * - 0: which means that the behaviour does not export its elastic
     *      properties.
     * - 1, which means that the thermal expansion is isotropic.
     * - 3, which means that the thermal expansion is orthotropic.
     */
    std::vector<std::string> linear_thermal_expansion_coefficients_epts;
    /*!
     * \brief names of the material properties that must be given to the
     * behaviour by the calling solver
     */
    std::vector<std::string> mpnames;
    //! \brief names of the internal state variables
    std::vector<std::string> ivnames;
    /*!
     * \brief types of the internal state variables
     * - 0 means that the variable is a scalar
     * - 1 means that the variable is a symmetric tensor
     * - 2 means that the variable is a vector
     * - 3 means that the variable is an unsymmetric tensor
     */
    std::vector<int> ivtypes;
    //! \brief names of the external state variables
    std::vector<std::string> evnames;
    /*!
     * \brief types of the external state variables
     * - 0 means that the variable is a scalar
     * - 1 means that the variable is a symmetric tensor
     * - 2 means that the variable is a vector
     * - 3 means that the variable is an unsymmetric tensor
     */
    std::vector<int> evtypes;
    //! \brief names of the parameters
    std::vector<std::string> pnames;
    //! \brief names of the integer parameters
    std::vector<std::string> ipnames;
    //! \brief names of the unsigned short parameters
    std::vector<std::string> upnames;
    /*!
     * \brief behaviour type
     * 0 : general behaviour
     * 1 : small strain behaviour
     * 2 : finite strain behaviour
     * 3 : cohesive zone model
     */
    unsigned short btype = 0;
    /*!
     * \brief behaviour kinematic
     * 0 : undefined kinematic
     * 1 : small strain kinematic
     * 2 : cohesive zone model kinematic
     * 3 : standard finite strain kinematic (F-Cauchy)
     * 4 : PTest,Alcyone, Cyrano 1D finite strain kinematic (eto-PK1)
     */
    unsigned short kinematic = 0;
    /*!
     * \brief symmetry type:
     * - If stype is equal to 0, the behaviour is isotropic.
     * - If stype is equal to 1, the behaviour is orthotropic.
     */
    unsigned short stype = 0;
    /*!
     * \brief elastic symmetry type:
     * - If etype is equal to 0, the behaviour elastic behaviour is isotropic.
     * - If etype is equal to 1, the behaviour elastic behaviour is
     * orthotropic.
     */
    unsigned short etype = 0;
    //! \brief if true, the behaviour may compute the internal energy
    bool computesInternalEnergy = false;
    //! \brief if true, the behaviour may compute the dissipated energy
    bool computesDissipatedEnergy = false;
    /*!
     * \brief If isUPUIR is true, the behaviour is usable in purely implicit
     * resolution.
     */
    bool isUPUIR = false;
    /*!
     * \brief If requiresStiffnessTensor is true, the caller must provide the
     * stiffness tensor.
     */
    bool requiresStiffnessTensor = false;
    /*!
     * \brief If requiresThermalExpansionCoefficientTensor is true, the
     * caller must provide the thermal expansion coefficients tensor
     */
    bool requiresThermalExpansionCoefficientTensor = false;
    /*!
     * \brief a boolean stating if the temperature was removed from the list of
     * declared external state variables.
     */
    bool hasTemperatureBeenRemovedFromExternalStateVariables = true;
  };

  //! \brief a simple alias
  using ExternalBehaviourData = ExternalBehaviourDescription;

}  // end of namespace tfel::system

extern "C" {

/*!
 * \author DeltaCAD
 * \brief build a new behaviour data
 * \note the behaviour data must be freed using
 * `freeExternalBehaviourData`
 */
TFELSYSTEM_VISIBILITY_EXPORT tfel::system::ExternalBehaviourData*
newExternalBehaviourData();
/*!
 * \author DeltaCAD
 * \brief build a new behaviour data
 * \note  the behaviour data
 */
TFELSYSTEM_VISIBILITY_EXPORT void freeExternalBehaviourData(
    tfel::system::ExternalBehaviourData*);
/*!
 * \author DeltaCAD
 * \brief retrieving the information from a behaviour
 * \param[in] d: data to be fetched
 * \param[in] l: library
 * \param[in] b: behaviour
 * \param[in] h: modelling hypothesis
 *
 * \return nullptr on success, an error message otherwise.\n
 * If an error message is set, it must be freed  using std::free
 * If the allocation of the error message fails, std::exit is called
 */
TFELSYSTEM_VISIBILITY_EXPORT char* getExternalBehaviourData(
    tfel::system::ExternalBehaviourData* const,
    const char* const,
    const char* const,
    const char* const);

}  // end of extern "C"

#endif /* LIB_TFEL_SYSTEM_EXTERNALBEHAVIOURDESCRIPTION_HXX */
