/*!
 * \file  mfront/include/MFront/UmatFiniteStrainMTestFileGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_UMATFINITESTRAINMTESTFILEGENERATOR_HXX
#define LIB_MFRONT_UMATFINITESTRAINMTESTFILEGENERATOR_HXX

#include "MFront/MFrontConfig.hxx"
#include "MFront/MTestFileGeneratorBase.hxx"

namespace mfront {

  struct MTESTFILEGENERATOR_VISIBILITY_EXPORT UmatFiniteStrainMTestFileGenerator
      final : public mfront::MTestFileGeneratorBase {
    /*!
     * constructor
     * \param[in] i : interface
     * \param[in] l : library
     * \param[in] b : behaviour
     */
    UmatFiniteStrainMTestFileGenerator(const std::string&,
                                       const std::string&,
                                       const std::string&);
    /*!
     * set the deformation gradient at the beginning of the time step
     * \param[in] F : deformation gradient
     */
    virtual void setDeformationGradientAtTheBeginningOfTheStimeStep(
        const double* const);
    /*!
     * set the deformation gradient at the end of the time step
     * \param[in] F : deformation gradient
     */
    virtual void setDeformationGradientAtTheEndOfTheStimeStep(
        const double* const);
    /*!
     * set stresses
     * \param[in] s : stresses
     */
    virtual void setStressTensor(const double* const);
    /*!
     * destructor
     */
    ~UmatFiniteStrainMTestFileGenerator() override;

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
    //! interface name
    std::string interface;
    //! library name
    std::string library;
    //! behaviour name
    std::string behaviour;
    //! deformation gradient at the beginning of the time step
    double F0[36];
    //! deformation gradient at the end of the time step
    double F1[36];
    //! stress increment
    double stress[6];
  };  // end of struct UmatFiniteStrainMTestFileGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_UMATFINITESTRAINMTESTFILEGENERATOR_HXX */
