/*!
 * \file
 * mfront/include/MFront/GenericBehaviourFiniteStrainMTestFileGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19/03/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOURFINITESTRAINMTESTFILEGENERATOR_HXX
#define LIB_MFRONT_GENERICBEHAVIOURFINITESTRAINMTESTFILEGENERATOR_HXX

#include "MFront/MFrontConfig.hxx"
#include "MFront/MTestFileGeneratorBase.hxx"

namespace mfront {

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT
      GenericBehaviourFiniteStrainMTestFileGenerator final
      : public mfront::MTestFileGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] l : library
     * \param[in] b : behaviour
     */
    GenericBehaviourFiniteStrainMTestFileGenerator(const char* const,
                                                   const char* const);
    /*!
     * set the deformation gradient at the beginning of the time step
     * \param[in] F : deformation gradient
     */
    virtual void setDeformationGradientTensorAtTheBeginningOfTheTimeStep(
        const double* const);
    /*!
     * set the deformation gradient at the end of the time step
     * \param[in] F : deformation gradient
     */
    virtual void setDeformationGradientTensorAtTheEndOfTheTimeStep(
        const double* const);
    /*!
     * set stresses
     * \param[in] s : stresses
     */
    virtual void setStressTensor(const double* const);
    /*!
     * destructor
     */
    ~GenericBehaviourFiniteStrainMTestFileGenerator() override;

   protected:
    /*!
     * write behaviour declaration
     * \param[in] os : output stream
     */
    void writeBehaviourDeclaration(std::ostream&) const override;
    /*!
     * write driving variables
     * \param[in] os : output stream
     */
    void writeGradients(std::ostream&) const override;
    //! library name
    const char* const library;
    //! behaviour name
    const char* const behaviour;
    //! deformation gradient at the beginning of the time step
    double F0[9];
    //! deformation gradient at the end of the time step
    double F1[9];
    //! stress increment
    double stress[6];
  };  // end of struct GenericBehaviourFiniteStrainMTestFileGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOURFINITESTRAINMTESTFILEGENERATOR_HXX */
