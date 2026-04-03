/*!
 * \file  mfront/include/MFront/MTestFileGeneratorBase.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 09 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MTESTFILEGENERATORBASE_HXX
#define LIB_MFRONT_MTESTFILEGENERATORBASE_HXX

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iosfwd>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/SupportedTypes.hxx"

namespace mfront {

  /*!
   * \brief a base class for mtest files generation
   * \see UMATSmallStrainMTestFileGenerator
   */
  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT MTestFileGeneratorBase {
    //! a simple alias
    using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
    //! a simple alias
    using real = double;
    //! \return an uniq identifier
    static unsigned int getIdentifier();
    //! \brief default constructor
    MTestFileGeneratorBase();
    /*!
     * \see the modelling hypothesis class
     * \param[in] h : modelling hypothesis
     */
    virtual void setModellingHypothesis(const Hypothesis);
    /*!
     * \brief add a computed time
     * \param[in] t : time
     */
    virtual void addTime(const real);
    /*!
     * \param[in] b : boolean
     */
    virtual void setHandleThermalExpansion(const bool);
    /*!
     * \brief set the rotation matrix
     * \param[in] m00 : m(0,0)
     * \param[in] m01 : m(0,1)
     * \param[in] m02 : m(0,2)
     * \param[in] m10 : m(1,0)
     * \param[in] m11 : m(1,1)
     * \param[in] m12 : m(1,2)
     * \param[in] m20 : m(2,0)
     * \param[in] m21 : m(2,1)
     * \param[in] m22 : m(2,2)
     */
    virtual void setRotationMatrix(const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real,
                                   const real);
    /*!
     * \brief add a material property
     * \param[in] n : material property name
     * \param[in] v : value
     */
    virtual void addMaterialProperty(const std::string&, const real);
    /*!
     * \brief add internal state variable
     * \param[in] n : internal state variable name
     * \param[in] f : variable type
     * \param[in] v : value
     */
    virtual void addInternalStateVariable(const std::string&,
                                          const SupportedTypes::TypeFlag,
                                          const real* const);
    /*!
     * \brief add the value of an external state variable
     * \param[in] n: external state variable name
     * \param[in] f: variable type
     * \param[in] t: initial time
     * \param[in] v0: initial values
     * \param[in] t: final time
     * \param[in] v1: final values or the increment values
     * \param[in] increment_provided: boolean stating if v1 holds the final
     * values and the increment values
     */
    virtual void addExternalStateVariable(const std::string&,
                                          const SupportedTypes::TypeFlag,
                                          const real,
                                          const real* const,
                                          const real,
                                          const real* const,
                                          const bool);
    virtual void generate(const std::string&) const;
    //! \return the size of a variable
    virtual unsigned short getVariableSize(
        const SupportedTypes::TypeFlag&) const;
    //! \return the size of an tiny vector
    virtual unsigned short getTVectorSize() const;
    //! \return the size of an stensor
    virtual unsigned short getStensorSize() const;
    //! \return the size of an tensor
    virtual unsigned short getTensorSize() const;
    //! \brief destructor
    virtual ~MTestFileGeneratorBase();

   protected:
    /*!
     * \return the names of the components of a vector
     * \param[in] n: name of the vector
     */
    virtual std::vector<std::string> getTVectorComponentsNames(
        const std::string& = "") const;
    /*!
     * \return the names of the components of a symmetric tensor
     * \param[in] n: name of the symmetric tensor
     */
    virtual std::vector<std::string> getStensorComponentsNames(
        const std::string& = "") const;
    /*!
     * \return the names of the components of a tensor
     * \param[in] n: name of the tensor
     */
    virtual std::vector<std::string> getTensorComponentsNames(
        const std::string& = "") const;
    //! \return the names of the components of the strain tensor
    virtual std::vector<std::string> getStrainComponentsNames() const;
    //! \return the names of the components of the deformation gradient
    virtual std::vector<std::string> getDeformationGradientComponentsNames()
        const;
    //! \return the names of the components of the stress tensor
    virtual std::vector<std::string> getStressComponentsNames() const;
    /*!
     * write modelling hypothesis
     * \param[in] os : output stream
     */
    virtual void writeModellingHypothesis(std::ostream&) const;
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    virtual void writeBehaviourDeclaration(std::ostream&) const = 0;
    /*!
     * write rotation matrix
     * \param[in] os : output stream
     */
    virtual void writeRotationMatrix(std::ostream&) const;
    /*!
     * write material properties
     * \param[in] os : output stream
     */
    virtual void writeMaterialProperties(std::ostream&) const;
    /*!
     * write material properties
     * \param[in] os : output stream
     */
    virtual void writeInternalStateVariables(std::ostream&) const;
    /*!
     * write external state variable
     * \param[in] os : output stream
     */
    virtual void writeExternalStateVariables(std::ostream&) const;
    /*!
     * write times
     * \param[in] os : output stream
     */
    virtual void writeTimes(std::ostream&) const;
    /*!
     * write driving variables
     * \param[in] os : output stream
     */
    virtual void writeGradients(std::ostream&) const = 0;
    //! \brief structure representing an internal state variable
    struct InternalStateVariable {
      InternalStateVariable() = default;
      InternalStateVariable(InternalStateVariable&&) = default;
      InternalStateVariable(const InternalStateVariable&) = default;
      InternalStateVariable& operator=(InternalStateVariable&&) = default;
      InternalStateVariable& operator=(const InternalStateVariable&) = default;
      ~InternalStateVariable();
      //! \brief name
      std::string name;
      //! \brief type of the variable
      SupportedTypes::TypeFlag type;
      //! \brief initial value
      std::vector<real> values;
    };  // end of struct InternalStateVariable
    //! \brief structure representing an internal state variable
    struct ExternalStateVariable {
      ExternalStateVariable() = default;
      ExternalStateVariable(ExternalStateVariable&&) = default;
      ExternalStateVariable(const ExternalStateVariable&) = default;
      ExternalStateVariable& operator=(ExternalStateVariable&&) = default;
      ExternalStateVariable& operator=(const ExternalStateVariable&) = default;
      ~ExternalStateVariable();
      //! \brief name
      std::string name;
      //! \brief type of the variable
      SupportedTypes::TypeFlag type;
      //! \brief initial value
      std::pair<real, std::vector<real>> initial_values;
      //! \brief final value
      std::pair<real, std::vector<real>> final_values;
    };  // end of struct ExternalStateVariable
    //! \brief modelling hypothesis
    Hypothesis hypothesis;
    //! \brief real time step
    std::set<real> times;
    //! \brief values of the material properties
    std::map<std::string, real> mps;
    //! \brief internal state variable
    std::vector<InternalStateVariable> ivs;
    //! \brief values of the external state variables
    std::vector<ExternalStateVariable> evs;
    //! \brief if true handles thermal expansion
    bool handleThermalExpansion;
    //! \brief flag true if the rotation matrix is defined
    bool hasRotationMatrix;
    //! \brief rotation matrix
    real m[9u];
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MTESTFILEGENERATORBASE_HXX */
